#pragma once
#include "../ForwardDeclarations.hpp"
#include "../Tensor3Base.hpp"
#include "../utils.hpp"

namespace StealthMath {
    namespace {
        template <typename LHS, typename RHS>
        constexpr STEALTH_ALWAYS_INLINE auto optimal_indexing_mode() noexcept {
            // Figure out what the operands require
            constexpr int intrinsicIndexingMode = std::max(internal::traits<LHS>::indexingMode,
                internal::traits<RHS>::indexingMode);
            // Now figure out what broadcasting would require.
            constexpr int lhsLength = internal::traits<LHS>::length,
                lhsSize = internal::traits<LHS>::size;
            constexpr bool lhs_is_scalar = internal::traits<LHS>::is_scalar;
            // RHS
            constexpr int rhsLength = internal::traits<RHS>::length,
                rhsSize = internal::traits<RHS>::size;
            constexpr bool rhs_is_scalar = internal::traits<RHS>::is_scalar;
            // If dimensions match or either value is a scalar, we can index in 1D.
            if constexpr (lhs_is_scalar or rhs_is_scalar or lhsSize == rhsSize) {
                return std::max(1, intrinsicIndexingMode);
            } else if constexpr (lhsLength == 1 or rhsLength == 1) {
                // Otherwise, if one is a row vector, index in 2D.
                return std::max(2, intrinsicIndexingMode);
            } else {
                // Otherwise, we must index in 3D.
                return std::max(3, intrinsicIndexingMode);
            }
        }

        template <typename LHS, typename RHS>
        constexpr STEALTH_ALWAYS_INLINE auto assert_compatibility() noexcept {
            // Check if these Tensor3s can be operated on correctly.
            constexpr int lhsWidth = internal::traits<LHS>::width,
                lhsLength = internal::traits<LHS>::length,
                lhsHeight = internal::traits<LHS>::height,
                lhsSize = internal::traits<LHS>::size;
            constexpr bool lhs_is_scalar = internal::traits<LHS>::is_scalar,
                lhs_is_vector = internal::traits<LHS>::is_vector,
                lhs_is_matrix = internal::traits<LHS>::is_matrix;
            // RHS
            constexpr int rhsWidth = internal::traits<RHS>::width,
                rhsLength = internal::traits<RHS>::length,
                rhsHeight = internal::traits<RHS>::height,
                rhsSize = internal::traits<RHS>::size;
            constexpr bool rhs_is_scalar = internal::traits<RHS>::is_scalar,
                rhs_is_vector = internal::traits<RHS>::is_vector,
                rhs_is_matrix = internal::traits<RHS>::is_matrix;
            if constexpr (rhsSize != lhsSize) {
                // If their sizes do not match, either one is a scalar...
                if constexpr (lhs_is_scalar or rhs_is_scalar)
                    return true;
                // ...or one must be a vector whose third dimension matches the other's...
                else if constexpr (lhs_is_vector or rhs_is_vector)
                    return (lhsWidth == rhsWidth or lhsLength == rhsLength or lhsHeight == rhsHeight);
                // ...or one must be a matrix for which 2 dimensions match the other's.
                else if constexpr (lhs_is_matrix or rhs_is_matrix)
                    return ((lhsWidth == rhsWidth and lhsLength == rhsLength)
                        or (lhsWidth == rhsWidth and lhsHeight == rhsHeight)
                        or (lhsLength == rhsLength and lhsHeight == rhsHeight));
            } else {
                // Same size Tensor3s are automatically compatible!
                return true;
            }
        }
    }

    namespace internal {
        template <typename LHS, typename BinaryOperation, typename RHS>
        struct traits<ElemWiseBinaryExpr<LHS, BinaryOperation, RHS>> {
            static constexpr ExpressionType exprType = ExpressionType::ElemWiseBinaryExpr;
            // Since the incoming LHS/RHS is either a reference or copy, we need to remove qualifiers to get size information.
            using ScalarType = typename std::invoke_result<BinaryOperation, scalar_element<LHS>,
                scalar_element<RHS>>::type;
            // Dimensions
            static constexpr int length = std::max(internal::traits<LHS>::length, internal::traits<RHS>::length),
                width = std::max(internal::traits<LHS>::width, internal::traits<RHS>::width),
                height = std::max(internal::traits<LHS>::height, internal::traits<RHS>::height),
                area = std::max(internal::traits<LHS>::area, internal::traits<RHS>::area),
                size = std::max(internal::traits<LHS>::size, internal::traits<RHS>::size),
                indexingMode = optimal_indexing_mode<LHS, RHS>();
            using StoredLHS = expr_ref<LHS>;
            using StoredRHS = expr_ref<RHS>;
            static constexpr bool is_scalar = size == 1;
            static constexpr bool is_vector = !is_scalar and (width == size or length == size or height == size);
            static constexpr bool is_matrix = !is_vector and (width == 1 or length == 1 or height == 1);
        };
    } /* internal */


    template <typename LHS, typename BinaryOperation, typename RHS>
    class ElemWiseBinaryExpr : public Tensor3Base<ElemWiseBinaryExpr<LHS, BinaryOperation, RHS>> {
        // Store either a reference or copy depending on what the operands are.
        using StoredLHS = typename internal::traits<ElemWiseBinaryExpr>::StoredLHS;
        using StoredRHS = typename internal::traits<ElemWiseBinaryExpr>::StoredRHS;

        public:
            constexpr STEALTH_ALWAYS_INLINE ElemWiseBinaryExpr(LHS&& lhs, BinaryOperation op, RHS&& rhs) noexcept
                : lhs{std::forward<LHS&&>(lhs)}, op{std::move(op)}, rhs{std::forward<RHS&&>(rhs)} {
                static_assert(assert_compatibility<LHS, RHS>(),
                    "Cannot perform element-wise binary operation on incompatible arguments");

                #ifdef DEBUG
                    std::cout << "Creating ElemWiseBinaryExpr" << '\n';
                    // LHS
                    std::cout << "LHS Information\nLHS Type: ";
                    debugType<LHS&&>();
                    std::cout << "LHS Stored Type: ";
                    debugType<StoredLHS>();
                    // RHS
                    std::cout << "RHS Information\nRHS Type: ";
                    debugType<RHS&&>();
                    std::cout << "RHS Stored Type: ";
                    debugType<StoredRHS>();
                #endif
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y, int z) const {
                // We can broadcast single points, 1D vectors, as well as 2D matrixs over 3D cubes.
                return op(
                    lhs((lhs.width() == 1) ? 0 : x, (lhs.length() == 1) ? 0 : y, (lhs.height() == 1) ? 0 : z),
                    rhs((rhs.width() == 1) ? 0 : x, (rhs.length() == 1) ? 0 : y, (rhs.height() == 1) ? 0 : z)
                );
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y) const {
                // We can broadcast single points and 1D vectors across 2D matrixs
                return op(
                    lhs((lhs.width() == 1) ? 0 : x, (lhs.length() == 1) ? 0 : y),
                    rhs((rhs.width() == 1) ? 0 : x, (rhs.length() == 1) ? 0 : y)
                );
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x) const {
                // We can broadcast scalars across 1D vectors.
                return op(
                    lhs((lhs.width() == 1) ? 0 : x),
                    rhs((rhs.width() == 1) ? 0 : x)
                );
            }
        private:
            StoredLHS lhs;
            BinaryOperation op;
            StoredRHS rhs;
    };
} /* StealthMath */
