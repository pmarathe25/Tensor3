#pragma once
#include "../core/ForwardDeclarations.hpp"
#include "../core/Tensor3Base.hpp"
#include "../utils.hpp"

namespace Stealth::Tensor {
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
            static constexpr ExpressionType exprType = ExpressionType::BlockExpr;
            using ScalarType = typename internal::traits<LHS>::ScalarType;
            // Dimensions
            static constexpr int length = lengthAtCompileTime,
                width = widthAtCompileTime,
                height = heightAtCompileTime,
                area = length * width,
                size = area * height,
                indexingMode = std::max(optimal_indexing_mode<width, length, height, LHS>(),
                    internal::traits<LHS>::indexingMode);
            using StoredLHS = expr_ref<LHS>;
            static constexpr bool is_scalar = size == 1;
            static constexpr bool is_vector = !is_scalar and (width == size or length == size or height == size);
            static constexpr bool is_matrix = !is_vector and (width == 1 or length == 1 or height == 1);
        };
    } /* internal */

    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename LHS>
    class BlockExpr : public Tensor3Base<BlockExpr<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, LHS>> {
        public:
            using StoredLHS = typename internal::traits<BlockExpr>::StoredLHS;

            constexpr STEALTH_ALWAYS_INLINE BlockExpr(LHS&& otherTensor3, int x = 0, int y = 0, int z = 0) noexcept
                : tensor3{otherTensor3}, minX{x}, minY{y}, minZ{z},
                offset{minX + minY * tensor3.width() + minZ * tensor3.area()},
                offsetXZ{minX + minZ * tensor3.area()} {
                #ifdef DEBUG
                    debugType<StoredLHS>();
                #endif
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y, int z)
                -> typename std::invoke_result<StoredLHS, int, int, int>::type {
                return tensor3(x + minX, y + minY, z + minZ);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y, int z) const
                -> typename std::invoke_result<StoredLHS, int, int, int>::type {
                return tensor3(x + minX, y + minY, z + minZ);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y)
                -> typename std::invoke_result<StoredLHS, int, int>::type {
                return tensor3(x + offsetXZ, y + minY);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y) const
                -> typename std::invoke_result<StoredLHS, int, int>::type {
                return tensor3(x + offsetXZ, y + minY);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x)
                -> typename std::invoke_result<StoredLHS, int>::type {
                return tensor3(x + offset);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x) const
                -> typename std::invoke_result<StoredLHS, int>::type {
                return tensor3(x + offset);
            }

            constexpr STEALTH_ALWAYS_INLINE auto data() const noexcept {
                return &(this -> operator()(0));
            }

            constexpr STEALTH_ALWAYS_INLINE auto data() noexcept {
                return &(this -> operator()(0));
            }

            constexpr STEALTH_ALWAYS_INLINE auto& underlyingTensor3() noexcept {
                return tensor3;
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& underlyingTensor3() const noexcept {
                return tensor3;
            }

        private:
            const int minX, minY, minZ;
            const int offset, offsetXZ;
            StoredLHS tensor3;
    };

} /* Stealth::Tensor */
