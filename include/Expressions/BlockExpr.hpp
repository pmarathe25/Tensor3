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
                if constexpr (width == internal::traits<raw_type<LHS>>::width) {
                    // If we can treat it as a single row, just use 1D accesses.
                    return 1;
                } else {
                    // Otherwise use 2D accesses.
                    return 2;
                }
            } else {
                // 3D Views can potentially use 1D, 2D or 3D indexing.
                if constexpr (width == internal::traits<raw_type<LHS>>::width
                    && length == internal::traits<raw_type<LHS>>::length) {
                    // Completely continuous - use 1D indexing.
                    return 1;
                } else if constexpr (length == internal::traits<raw_type<LHS>>::length) {
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
            using ScalarType = typename internal::traits<raw_type<LHS>>::ScalarType;
            // Dimensions
            static constexpr int length = lengthAtCompileTime,
                width = widthAtCompileTime,
                height = heightAtCompileTime,
                area = length * width,
                size = area * height,
                indexingMode = std::max(optimal_indexing_mode<width, length, height, LHS>(),
                    internal::traits<raw_type<LHS>>::indexingMode);
            using StoredLHS = expression_stored_type<LHS>;
            static constexpr bool is_scalar = size == 1;
            static constexpr bool is_vector = !is_scalar and (width == size or length == size or height == size);
            static constexpr bool is_matrix = !is_vector and (width == 1 or length == 1 or height == 1);
        };

        // TODO: Specialization for blocks of other blocks
        // template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime,
        //     int otherWidth, int otherLength, int otherHeight, typename otherInternalTileMap>
        // struct traits<BlockExpr<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime,
        //     BlockExpr<otherWidth, otherLength, otherHeight, otherInternalTileMap>>> {
        // };

    } /* internal */

    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename LHS>
    class BlockExpr : public TileMapBase<BlockExpr<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, LHS>> {
        public:
            using StoredLHS = typename internal::traits<BlockExpr>::StoredLHS;

            constexpr STEALTH_ALWAYS_INLINE BlockExpr(LHS&& otherTileMap, int x = 0, int y = 0, int z = 0) noexcept
                : tileMap{otherTileMap}, minX{x}, minY{y}, minZ{z},
                offset{minX + minY * tileMap.width() + minZ * tileMap.area()} { }

            // TODO: Constructor for blocks of blocks.

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

        private:
            const int minX, minY, minZ;
            const int offset;
            StoredLHS tileMap;
    };

} /* Stealth */
