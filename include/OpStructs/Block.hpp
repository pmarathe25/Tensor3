#ifndef TILE_MAP_VIEW_H
#define TILE_MAP_VIEW_H
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"

#include <iostream>

namespace StealthTileMap {
    namespace internal {
        template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename InternalTileMap, typename dat, typename writable>
        struct traits<Block<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, dat, writable>> {
            typedef typename internal::traits<InternalTileMap>::ScalarType ScalarType;
            // Dimensions
            static constexpr int length = lengthAtCompileTime,
                width = widthAtCompileTime,
                height = heightAtCompileTime,
                area = length * width,
                size = area * height;
            typedef dat containsData;
            typedef writable isWritable;
            typedef typename internal::traits<InternalTileMap>::UnderlyingTileMapType UnderlyingTileMapType;
        };
    } /* internal */

    namespace {
        template <typename BlockType, typename InternalTileMap>
        constexpr STEALTH_ALWAYS_INLINE auto optimal_indexing_mode() noexcept {
            if constexpr (internal::traits<BlockType>::height == 1 && internal::traits<BlockType>::length == 1) {
                // 1D Views always use 1D indexing.
                return 1;
            } else if constexpr (internal::traits<BlockType>::height == 1) {
                // 2D Views can potentially use either 1D or 2D indexing.
                if constexpr (internal::traits<BlockType>::width == internal::traits<InternalTileMap>::width) {
                    // If we can treat it as a single row, just use 1D accesses.
                    return 1;
                } else {
                    // Otherwise use 2D accesses.
                    return 2;
                }
            } else {
                // 3D Views can potentially use 1D, 2D or 3D indexing.
                if constexpr (internal::traits<BlockType>::width == internal::traits<InternalTileMap>::width
                    && internal::traits<BlockType>::length == internal::traits<InternalTileMap>::length) {
                    // Completely continuous - use 1D indexing.
                    return 1;
                } else if constexpr (internal::traits<BlockType>::length == internal::traits<InternalTileMap>::length) {
                    // Treat as a long 2D block.
                    return 2;
                } else {
                    // No usable patterns detected.
                    return 3;
                }
            }
        }

        template <int indexingMode, typename BlockType>
        constexpr STEALTH_ALWAYS_INLINE auto indexSpecial(int hintX, int hintY, int x, int y, int z, BlockType block)
            -> typename std::invoke_result<BlockType, int>::type {
            if constexpr (indexingMode == 1) {
                // hintX and hintY are valid, just add offsets
                return block.underlyingTileMap()(hintX + block.offset, hintY + block.minY, x, y, z);
            } else if constexpr (indexingMode == 2) {
                // hintY is valid, but hintX must be recalculated. Then add offsets
                hintX = (x + block.minX) + (hintY + block.minY) * block.underlyingTileMap().width();
                return block.underlyingTileMap()(hintX, hintY + block.minY, x, y, z);
            } else {
                // hintY and hintX are invalid for 3D accesses
                hintY = (y + block.minY) + (z + block.minZ) * block.underlyingTileMap().length();
                hintX = (x + block.minX) + hintY * block.underlyingTileMap().width();
                return block.underlyingTileMap()(hintX, hintY, x, y, z);
            }
        }
    }

    // Writable view
    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename InternalTileMap>
    class Block<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::true_type, std::true_type>
        : public TileMapBase<Block<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::true_type, std::true_type>> {
        public:
            typedef typename internal::traits<Block>::ScalarType ScalarType;
            typedef typename internal::traits<Block>::UnderlyingTileMapType UnderlyingTileMapType;

            constexpr STEALTH_ALWAYS_INLINE Block(InternalTileMap& tileMap, int x = 0, int y = 0, int z = 0) noexcept
                : tileMap{tileMap.underlyingTileMap()}, minX{x + tileMap.minX}, minY{y + tileMap.minY}, minZ{z + tileMap.minZ},
                offset{this -> minX + this -> minY * widthAtCompileTime + this -> minZ * widthAtCompileTime * lengthAtCompileTime},
                offsetXZ{this -> minX + this -> minZ * widthAtCompileTime * lengthAtCompileTime},
                minYOffset{this -> minY * tileMap.width()} { }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int hintX, int hintY, int x, int y, int z) const {
                constexpr int indexingMode = optimal_indexing_mode<Block, UnderlyingTileMapType>();
                return indexSpecial<indexingMode, const Block&>(hintX, hintY, x, y, z, *this);
            }

            constexpr STEALTH_ALWAYS_INLINE auto& operator()(int hintX, int hintY, int x, int y, int z) {
                constexpr int indexingMode = optimal_indexing_mode<Block, UnderlyingTileMapType>();
                return indexSpecial<indexingMode, Block&>(hintX, hintY, x, y, z, *this);
            }

            constexpr STEALTH_ALWAYS_INLINE auto& operator()(int x, int y, int z) {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int x, int y, int z) const {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr STEALTH_ALWAYS_INLINE auto& operator()(int x, int y) {
                return tileMap(x + offsetXZ, y + minY);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int x, int y) const {
                return tileMap(x + offsetXZ, y + minY);
            }

            constexpr STEALTH_ALWAYS_INLINE auto& operator()(int x) {
                return tileMap(x + offset);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int x) const {
                return tileMap(x + offset);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto* data() const noexcept {
                return &(this -> operator()(0));
            }

            constexpr STEALTH_ALWAYS_INLINE auto* data() noexcept {
                return &(this -> operator()(0));
            }

            constexpr STEALTH_ALWAYS_INLINE auto& underlyingTileMap() {
                return tileMap;
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& underlyingTileMap() const {
                return tileMap;
            }

            const int minX, minY, minZ;
            const int offset, offsetXZ, minYOffset;
        private:
            UnderlyingTileMapType& tileMap;
    };

    // Const view
    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename InternalTileMap>
    class Block<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::true_type, std::false_type>
        : public TileMapBase<Block<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::true_type, std::false_type>> {
        public:
            typedef typename internal::traits<Block>::ScalarType ScalarType;
            typedef typename internal::traits<Block>::UnderlyingTileMapType UnderlyingTileMapType;

            constexpr STEALTH_ALWAYS_INLINE Block(const InternalTileMap& tileMap, int x = 0, int y = 0, int z = 0) noexcept
                : tileMap{tileMap.underlyingTileMap()}, minX{x + tileMap.minX}, minY{y + tileMap.minY}, minZ{z + tileMap.minZ},
                offset{this -> minX + this -> minY * widthAtCompileTime + this -> minZ * widthAtCompileTime * lengthAtCompileTime},
                offsetXZ{this -> minX + this -> minZ * widthAtCompileTime * lengthAtCompileTime},
                minYOffset{this -> minY * tileMap.width()} { }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int hintX, int hintY, int x, int y, int z) const {
                constexpr int indexingMode = optimal_indexing_mode<Block, UnderlyingTileMapType>();
                return indexSpecial<indexingMode, const Block&>(hintX, hintY, x, y, z, *this);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int x, int y, int z) const {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int x, int y) const {
                return tileMap(x + offsetXZ, y + minY);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int x) const {
                return tileMap(x + offset);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto* data() const noexcept {
                return &(this -> operator()(0));
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& underlyingTileMap() const {
                return tileMap;
            }

            const int minX, minY, minZ;
            const int offset, offsetXZ, minYOffset;
        private:
            const UnderlyingTileMapType& tileMap;
    };

    // A view of a temporary object. Cannot be modified.
    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename InternalTileMap>
    class Block<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::false_type>
        : public TileMapBase<Block<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::false_type>> {
        public:
            typedef typename internal::traits<Block>::ScalarType ScalarType;
            typedef typename internal::traits<Block>::UnderlyingTileMapType UnderlyingTileMapType;

            constexpr STEALTH_ALWAYS_INLINE Block(const InternalTileMap& tileMap, int x = 0, int y = 0, int z = 0) noexcept
                : tileMap{tileMap.underlyingTileMap()}, minX{x + tileMap.minX}, minY{y + tileMap.minY}, minZ{z + tileMap.minZ},
                offset{this -> minX + this -> minY * widthAtCompileTime + this -> minZ * widthAtCompileTime * lengthAtCompileTime},
                offsetXZ{this -> minX + this -> minZ * widthAtCompileTime * lengthAtCompileTime},
                minYOffset{this -> minY * tileMap.width()} { }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int hintX, int hintY, int x, int y, int z) const {
                constexpr int indexingMode = optimal_indexing_mode<Block, UnderlyingTileMapType>();
                return indexSpecial<indexingMode, const Block&>(hintX, hintY, x, y, z, *this);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y, int z) const {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y) const {
                return tileMap(x + offsetXZ, y + minY);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x) const {
                return tileMap(x + offset);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& underlyingTileMap() const {
                return tileMap;
            }

            const int minX, minY, minZ;
            const int offset, offsetXZ, minYOffset;
        private:
            const UnderlyingTileMapType& tileMap;
    };

} /* StealthTileMap */

#endif /* end of include guard: TILE_MAP_VIEW_H */
