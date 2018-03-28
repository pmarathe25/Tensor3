#pragma once
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"
#include "../utils.hpp"

namespace Stealth {
    namespace internal {
        template <typename UnaryOperation, typename LHS>
        struct traits<ElemWiseUnaryExpr<UnaryOperation, LHS>> {
            // Since the incoming LHS is either a reference or copy,
            // we need to remove qualifiers to get size information.
            using ScalarType = typename std::invoke_result<UnaryOperation, scalar_element<raw_type<LHS>>>::type;
            // Dimensions
            static constexpr int length = internal::traits<raw_type<LHS>>::length,
                width = internal::traits<raw_type<LHS>>::width,
                height = internal::traits<raw_type<LHS>>::height,
                area = internal::traits<raw_type<LHS>>::area,
                size = internal::traits<raw_type<LHS>>::size,
                indexingMode = internal::traits<raw_type<LHS>>::indexingMode;
            using StoredLHS = expression_stored_type<LHS>;
            static constexpr bool is_scalar = size == 1;
            static constexpr bool is_vector = !is_scalar and (width == size or length == size or height == size);
            static constexpr bool is_matrix = !is_vector and (width == 1 or length == 1 or height == 1);
        };
    } /* internal */

    template <typename UnaryOperation, typename LHS>
    class ElemWiseUnaryExpr : public TileMapBase<ElemWiseUnaryExpr<UnaryOperation, LHS>> {
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
} /* Stealth */
