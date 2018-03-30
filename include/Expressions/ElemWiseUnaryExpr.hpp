#pragma once
#include "../ForwardDeclarations.hpp"
#include "../Tensor3Base.hpp"
#include "../utils.hpp"

namespace Stealth::Math {
    namespace internal {
        template <typename UnaryOperation, typename LHS>
        struct traits<ElemWiseUnaryExpr<UnaryOperation, LHS>> {
            static constexpr ExpressionType exprType = ExpressionType::ElemWiseUnaryExpr;
            // Since the incoming LHS is either a reference or copy,
            // we need to remove qualifiers to get size information.
            using ScalarType = typename std::invoke_result<UnaryOperation, scalar_element<LHS>>::type;
            // Dimensions
            static constexpr int length = internal::traits<LHS>::length,
                width = internal::traits<LHS>::width,
                height = internal::traits<LHS>::height,
                area = internal::traits<LHS>::area,
                size = internal::traits<LHS>::size,
                indexingMode = internal::traits<LHS>::indexingMode;
            using StoredLHS = expr_ref<LHS>;
            static constexpr bool is_scalar = size == 1;
            static constexpr bool is_vector = !is_scalar and (width == size or length == size or height == size);
            static constexpr bool is_matrix = !is_vector and (width == 1 or length == 1 or height == 1);
        };
    } /* internal */

    template <typename UnaryOperation, typename LHS>
    class ElemWiseUnaryExpr : public Tensor3Base<ElemWiseUnaryExpr<UnaryOperation, LHS>> {
        public:
            // Store either a reference or copy depending on what the operand is.
            using StoredLHS = typename internal::traits<ElemWiseUnaryExpr>::StoredLHS;

            constexpr STEALTH_ALWAYS_INLINE ElemWiseUnaryExpr(UnaryOperation op, LHS&& lhs) noexcept
                : op{std::move(op)}, lhs{std::forward<LHS&&>(lhs)} { }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y, int z) const {
                return op(lhs(x, y, z));
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y) const {
                return op(lhs(x, y));
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x) const {
                return op(lhs(x));
            }

        private:
            StoredLHS lhs;
            UnaryOperation op;
    };
} /* Stealth::Math */
