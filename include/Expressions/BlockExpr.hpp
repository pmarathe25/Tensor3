#pragma once
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"

namespace Stealth {
    namespace {
        template <int width, int length, int height, typename LHS>
        constexpr STEALTH_ALWAYS_INLINE auto optimal_indexing_mode() noexcept {
            if constexpr (height == 1 && length == 1) {
                // 1D Views always use 1D indexing.
                return 1;
            } else if constexpr (height == 1) {
                // 2D Views can potentially use either 1D or 2D indexing.
                if constexpr (width == internal::traits<LHS>::width) {
                    // If we can treat it as a single row, just use 1D accesses.
                    return 1;
                } else {
                    // Otherwise use 2D accesses.
                    return 2;
                }
            } else {
                // 3D Views can potentially use 1D, 2D or 3D indexing.
                if constexpr (width == internal::traits<LHS>::width
                    && length == internal::traits<LHS>::length) {
                    // Completely continuous - use 1D indexing.
                    return 1;
                } else if constexpr (length == internal::traits<LHS>::length) {
                    // Treat as a long 2D block.
                    return 2;
                } else {
                    // No usable patterns detected.
                    return 3;
                }
            }
        }
    }

    namespace internal {
        template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename LHS>
        struct traits<BlockExpr<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, LHS>> {
            using InternalTileMapNoCV = strip_qualifiers<LHS>;
            using ScalarType = typename internal::traits<InternalTileMapNoCV>::ScalarType;
            // Dimensions
            static constexpr int length = lengthAtCompileTime,
                width = widthAtCompileTime,
                height = heightAtCompileTime,
                area = length * width,
                size = area * height,
                indexingModeRequired = std::max(optimal_indexing_mode<width, length, height, InternalTileMapNoCV>(),
                    internal::traits<InternalTileMapNoCV>::indexingModeRequired);
            using StoredLHS = expression_stored_type<LHS>;
        };

        // Specialization for blocks of other blocks
        // template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime,
        //     int otherWidth, int otherLength, int otherHeight, typename otherInternalTileMap>
        // struct traits<BlockExpr<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime,
        //     BlockExpr<otherWidth, otherLength, otherHeight, otherInternalTileMap>>> {
        //     // FIXME: If the other block is rvalue, steal its data.
        //     using LHS = strip_qualifiers<otherInternalTileMap>;
        //     using ScalarType = typename internal::traits<LHS>::ScalarType;
        //     // Dimensions
        //     static constexpr int length = lengthAtCompileTime,
        //         width = widthAtCompileTime,
        //         height = heightAtCompileTime,
        //         area = length * width,
        //         size = area * height;
        //     using UnderlyingTileMap = typename internal::traits<LHS>::UnderlyingTileMap;
        //     using StoredLHS = UnderlyingTileMap;
        // };

    } /* internal */

    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename LHS>
    class BlockExpr : public TileMapBase<BlockExpr<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, LHS>> {
        public:
            using ScalarType = typename internal::traits<BlockExpr>::ScalarType;
            using StoredLHS = typename internal::traits<BlockExpr>::StoredLHS;
            static constexpr int indexingModeRequired = internal::traits<BlockExpr>::indexingModeRequired;

            constexpr STEALTH_ALWAYS_INLINE BlockExpr(LHS&& otherTileMap, int x = 0, int y = 0, int z = 0) noexcept
                : tileMap{otherTileMap}, minX{x}, minY{y}, minZ{z},
                offset{minX + minY * tileMap.width() + minZ * tileMap.area()} { }

            // template <int otherWidth, int otherLength, int otherHeight, typename otherInternalTileMap>
            // constexpr STEALTH_ALWAYS_INLINE BlockExpr(BlockExpr<otherWidth, otherLength, otherHeight, otherInternalTileMap>&& otherTileMap, int x = 0, int y = 0, int z = 0) noexcept
            //     : tileMap{otherTileMap.underlyingTileMap()}, minX{x + tileMap.minX}, minY{y + tileMap.minY}, minZ{z + tileMap.minZ},
            //     offset{minX + minY * tileMap.width() + minZ * tileMap.area()} { }

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

            // constexpr STEALTH_ALWAYS_INLINE auto underlyingTileMap() {
            //     return tileMap;
            // }
            //
            // constexpr STEALTH_ALWAYS_INLINE auto underlyingTileMap() const {
            //     return tileMap;
            // }

            const int minX, minY, minZ;
            const int offset;
            StoredLHS tileMap;
        // private:
    };

} /* Stealth */
