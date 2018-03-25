#pragma once
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"

namespace Stealth {
    namespace internal {
        template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename InternalTileMap>
        struct traits<BlockExpr<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap>> {
            using ScalarType = typename internal::traits<InternalTileMap>::ScalarType;
            // Dimensions
            static constexpr int length = lengthAtCompileTime,
                width = widthAtCompileTime,
                height = heightAtCompileTime,
                area = length * width,
                size = area * height;
            using UnderlyingTileMap = InternalTileMap;
            using StoredLHS = expression_stored_type<UnderlyingTileMap>;
        };

        // Specialization for blocks of other blocks
        template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime,
            int otherWidth, int otherLength, int otherHeight, typename otherInternalTileMap>
        struct traits<BlockExpr<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime,
            BlockExpr<otherWidth, otherLength, otherHeight, otherInternalTileMap>>> {
            // FIXME: If the other block is rvalue, steal its data.
            using InternalTileMap = strip_qualifiers<otherInternalTileMap>;
            using ScalarType = typename internal::traits<InternalTileMap>::ScalarType;
            // Dimensions
            static constexpr int length = lengthAtCompileTime,
            width = widthAtCompileTime,
            height = heightAtCompileTime,
            area = length * width,
            size = area * height;
            using UnderlyingTileMap = typename internal::traits<InternalTileMap>::UnderlyingTileMap;
            using StoredLHS = UnderlyingTileMap;
        };

    } /* internal */

    namespace {
        template <typename BlockType, typename InternalTileMap>
        constexpr STEALTH_ALWAYS_INLINE auto optimal_hinting_mode() noexcept {
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

        template <int hintingMode, typename BlockType>
        constexpr STEALTH_ALWAYS_INLINE auto indexWithHints(int hintX, int hintY, int x, int y, int z, BlockType block)
            -> typename std::invoke_result<BlockType, int>::type {
            if constexpr (hintingMode == 1) {
                // hintX and hintY are valid - just add offsets.
                return block.underlyingTileMap().hintedIndex(hintX + block.offset, hintY + block.minY, x, y, z);
            } else if constexpr (hintingMode == 2) {
                // hintY is valid, but hintX must be recalculated.
                hintX = (x + block.minX) + (hintY + block.minY) * block.underlyingTileMap().width();
                return block.underlyingTileMap().hintedIndex(hintX, hintY + block.minY, x, y, z);
            } else {
                // hintY and hintX are invalid for 3D accesses
                hintY = (y + block.minY) + (z + block.minZ) * block.underlyingTileMap().length();
                hintX = (x + block.minX) + hintY * block.underlyingTileMap().width();
                return block.underlyingTileMap().hintedIndex(hintX, hintY, x, y, z);
            }
        }
    }

    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename InternalTileMap>
    class BlockExpr : public TileMapBase<BlockExpr<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap>> {
        public:
            using ScalarType = typename internal::traits<BlockExpr>::ScalarType;
            using StoredLHS = typename internal::traits<BlockExpr>::StoredLHS;
            static constexpr int hintingMode = optimal_hinting_mode<BlockExpr, strip_qualifiers<StoredLHS>>();

            constexpr STEALTH_ALWAYS_INLINE BlockExpr(InternalTileMap&& otherTileMap, int x = 0, int y = 0, int z = 0) noexcept
                : tileMap{otherTileMap}, minX{x + tileMap.minX}, minY{y + tileMap.minY}, minZ{z + tileMap.minZ},
                offset{minX + minY * tileMap.width() + minZ * tileMap.area()} { }

            template <int otherWidth, int otherLength, int otherHeight, typename otherInternalTileMap>
            constexpr STEALTH_ALWAYS_INLINE BlockExpr(BlockExpr<otherWidth, otherLength, otherHeight, otherInternalTileMap>&& otherTileMap, int x = 0, int y = 0, int z = 0) noexcept
                : tileMap{otherTileMap.underlyingTileMap()}, minX{x + tileMap.minX}, minY{y + tileMap.minY}, minZ{z + tileMap.minZ},
                offset{minX + minY * tileMap.width() + minZ * tileMap.area()} { }

            constexpr STEALTH_ALWAYS_INLINE auto hintedIndex(int hintX, int hintY, int x, int y, int z) const
                -> typename std::invoke_result<StoredLHS, int, int, int>::type {
                return indexWithHints<hintingMode, const BlockExpr&>(hintX, hintY, x, y, z, *this);
            }

            constexpr STEALTH_ALWAYS_INLINE auto hintedIndex(int hintX, int hintY, int x, int y, int z)
                -> typename std::invoke_result<StoredLHS, int, int, int>::type {
                return indexWithHints<hintingMode, BlockExpr&>(hintX, hintY, x, y, z, *this);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y, int z)
                -> typename std::invoke_result<StoredLHS, int, int, int>::type {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y, int z) const
                -> typename std::invoke_result<StoredLHS, int, int, int>::type {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y)
                -> typename std::invoke_result<StoredLHS, int, int>::type {
                return tileMap(x + minX + minZ * tileMap.area(), y + minY);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y) const
                -> typename std::invoke_result<StoredLHS, int, int>::type {
                return tileMap(x + minX + minZ * tileMap.area(), y + minY);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x)
                -> typename std::invoke_result<StoredLHS, int>::type {
                return tileMap(x + offset);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x) const
                -> typename std::invoke_result<StoredLHS, int>::type {
                return tileMap(x + offset);
            }

            constexpr STEALTH_ALWAYS_INLINE auto data() const noexcept {
                return &(this -> operator()(0));
            }

            constexpr STEALTH_ALWAYS_INLINE auto data() noexcept {
                return &(this -> operator()(0));
            }

            constexpr STEALTH_ALWAYS_INLINE auto underlyingTileMap() {
                return tileMap;
            }

            constexpr STEALTH_ALWAYS_INLINE auto underlyingTileMap() const {
                return tileMap;
            }

            const int minX, minY, minZ;
            const int offset;
        private:
            StoredLHS tileMap;
    };

} /* Stealth */
