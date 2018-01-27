#ifndef TILE_MAP_VIEW_H
#define TILE_MAP_VIEW_H
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"

namespace StealthTileMap {
    template <typename BlockType, typename InternalTileMap>
    constexpr STEALTH_ALWAYS_INLINE auto optimal_indexing_mode() noexcept {
        if constexpr (internal::traits<BlockType>::width == internal::traits<InternalTileMap>::width
            && internal::traits<BlockType>::length == internal::traits<InternalTileMap>::length) {
            // No need to deduce anything, dimensions are the same.
            return 1;
        } else if constexpr (internal::traits<BlockType>::length == internal::traits<InternalTileMap>::length) {
            // Treat as a long 2D TileMap.
            return 2;
        } else if constexpr (internal::traits<BlockType>::height == 1) {
            // Optimizations for flat TileMaps.
            if constexpr (internal::traits<BlockType>::length == 1
                || internal::traits<BlockType>::width == internal::traits<InternalTileMap>::width) {
                // If we can treat it as a single row, just use 1D accesses.
                return 1;
            } else {
                // Otherwise use 2D accesses.
                return 2;
            }
        } else {
            // If all else fails, use 3D accesses.
            return 3;
        }
    }

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
        template <typename BlockType, typename InternalTileMapType>
        constexpr STEALTH_ALWAYS_INLINE auto computeSingleIndex(int offset, int offsetXZ, int minX, int minY, int minZ, int x, int y, int z) {
            if constexpr (internal::traits<BlockType>::width == internal::traits<InternalTileMapType>::width
                && internal::traits<BlockType>::length == internal::traits<InternalTileMapType>::length) {
                // No need to deduce anything, dimensions are the same
                return x + offset;
            } else if constexpr (internal::traits<BlockType>::length == internal::traits<InternalTileMapType>::length) {
                if constexpr (internal::traits<BlockType>::width == 1) {
                    return offsetXZ + (x + minY) * internal::traits<InternalTileMapType>::width;
                } else {
                    return x + offsetXZ + (y + minY) * internal::traits<InternalTileMapType>::width;
                }
            } else if constexpr (internal::traits<BlockType>::width == internal::traits<InternalTileMapType>::width) {
                if constexpr (internal::traits<BlockType>::height == 1) {
                    // Only 1 layer - all elements are continuous.
                    return x + offset;
                } else if constexpr (internal::traits<BlockType>::width == 1 && internal::traits<BlockType>::length == 1) {
                    // We're just going up
                    return minX + minY * internal::traits<InternalTileMapType>::width
                        + (x + minZ) * internal::traits<InternalTileMapType>::area;
                } else {
                    // Need to figure out what height is.
                    return x + minX + minY * internal::traits<InternalTileMapType>::width
                        + (z + minZ) * internal::traits<InternalTileMapType>::area;
                }
            } else {
                // Otherwise we need to figure out everything.
                if constexpr (internal::traits<BlockType>::length == 1 && internal::traits<BlockType>::width == 1) {
                    // Z is the only coordinate needed.
                    return minX + minY * internal::traits<InternalTileMapType>::width
                        + (x + minZ) * internal::traits<InternalTileMapType>::area;
                } if constexpr (internal::traits<BlockType>::length == 1) {
                    // No need to calculate y.
                    return x + minX + minY * internal::traits<InternalTileMapType>::width
                        + (z + minZ) * internal::traits<InternalTileMapType>::area;
                } else if constexpr (internal::traits<BlockType>::width == 1) {
                    // x is always 0
                    return minX + (y + minY) * internal::traits<InternalTileMapType>::width
                        + (z + minZ) * internal::traits<InternalTileMapType>::area;
                } else {
                    return x + minX + (y + minY) * internal::traits<InternalTileMapType>::width
                        + (z + minZ) * internal::traits<InternalTileMapType>::area;
                }
            }
        }
    }

    // Writable view
    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename InternalTileMap>
    class Block<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::true_type, std::true_type>
        : public TileMapBase<Block<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::true_type, std::true_type>> {
        public:
            typedef typename internal::traits<Block>::ScalarType ScalarType;

            constexpr STEALTH_ALWAYS_INLINE Block(InternalTileMap& tileMap, int minX = 0, int minY = 0, int minZ = 0) noexcept
                : tileMap{tileMap.underlyingTileMap()}, minX{minX + tileMap.minX}, minY{minY + tileMap.minY}, minZ{minZ + tileMap.minZ},
                offset{minX + minY * widthAtCompileTime + minZ * widthAtCompileTime * lengthAtCompileTime},
                offsetXZ{minX + minZ * widthAtCompileTime * lengthAtCompileTime} { }

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

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int index, int x, int y, int z) const {
                int newIndex = computeSingleIndex<Block, InternalTileMap>(offset, offsetXZ, minX, minY, minZ, x, y, z);
                return tileMap(newIndex, x + minX, y + minY, z + minZ);
            }

            constexpr STEALTH_ALWAYS_INLINE auto& operator()(int index, int x, int y, int z) {
                int newIndex = computeSingleIndex<Block, InternalTileMap>(offset, offsetXZ, minX, minY, minZ, x, y, z);
                return tileMap(newIndex, x + minX, y + minY, z + minZ);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto* data() const noexcept {
                return &(this -> operator()(0));
            }

            constexpr STEALTH_ALWAYS_INLINE auto* data() noexcept {
                return &(this -> operator()(0));
            }

            constexpr STEALTH_ALWAYS_INLINE auto& underlyingTileMap() {
                return tileMap.underlyingTileMap();
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& underlyingTileMap() const {
                return tileMap.underlyingTileMap();
            }

            const int minX, minY, minZ;
            const int offset, offsetXZ;
        private:
            typename internal::traits<Block>::UnderlyingTileMapType& tileMap;
    };

    // Const view
    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename InternalTileMap>
    class Block<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::true_type, std::false_type>
        : public TileMapBase<Block<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::true_type, std::false_type>> {
        public:
            typedef typename internal::traits<Block>::ScalarType ScalarType;

            constexpr STEALTH_ALWAYS_INLINE Block(const InternalTileMap& tileMap, int minX = 0, int minY = 0, int minZ = 0) noexcept
                : tileMap{tileMap.underlyingTileMap()}, minX{minX + tileMap.minX}, minY{minY + tileMap.minY}, minZ{minZ + tileMap.minZ},
                offset{minX + minY * widthAtCompileTime + minZ * widthAtCompileTime * lengthAtCompileTime},
                offsetXZ{minX + minZ * widthAtCompileTime * lengthAtCompileTime} { }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int x, int y, int z) const {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int x, int y) const {
                return tileMap(x + offsetXZ, y + minY);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int x) const {
                return tileMap(x + offset);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int index, int x, int y, int z) const {
                int newIndex = computeSingleIndex<Block, InternalTileMap>(offset, offsetXZ, minX, minY, minZ, x, y, z);
                return tileMap(newIndex, x + minX, y + minY, z + minZ);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto* data() const noexcept {
                return &(this -> operator()(0));
            }

            const int minX, minY, minZ;
            const int offset, offsetXZ;
        private:
            const typename internal::traits<Block>::UnderlyingTileMapType& tileMap;
    };

    // A view of a temporary object. Cannot be modified.
    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename InternalTileMap>
    class Block<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::false_type>
        : public TileMapBase<Block<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::false_type>> {
        public:
            typedef typename internal::traits<Block>::ScalarType ScalarType;

            constexpr STEALTH_ALWAYS_INLINE Block(const InternalTileMap& tileMap, int minX = 0, int minY = 0, int minZ = 0) noexcept
                : tileMap{tileMap.underlyingTileMap()}, minX{minX + tileMap.minX}, minY{minY + tileMap.minY}, minZ{minZ + tileMap.minZ},
                offset{minX + minY * widthAtCompileTime + minZ * widthAtCompileTime * lengthAtCompileTime},
                offsetXZ{minX + minZ * widthAtCompileTime * lengthAtCompileTime} { }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y, int z) const {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y) const {
                return tileMap(x + offsetXZ, y + minY);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x) const {
                return tileMap(x + offset);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int index, int x, int y, int z) const {
                int newIndex = computeSingleIndex<Block, InternalTileMap>(offset, offsetXZ, minX, minY, minZ, x, y, z);
                return tileMap(newIndex, x + minX, y + minY, z + minZ);
            }

            const int minX, minY, minZ;
            const int offset, offsetXZ;
        private:
            const typename internal::traits<Block>::UnderlyingTileMapType& tileMap;
    };

} /* StealthTileMap */

#endif /* end of include guard: TILE_MAP_VIEW_H */
