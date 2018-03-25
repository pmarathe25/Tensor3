#pragma once
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"
#include "../utils.hpp"

namespace Stealth {
    namespace internal {
        template <typename UnaryOperation, typename LHS>
        struct traits<UnaryExpr<UnaryOperation, LHS>> {
            // Since the incoming LHS is either a reference or copy,
            // we need to remove qualifiers to get size information.
            using ScalarType = typename std::invoke_result<UnaryOperation, scalar_element<raw_type<LHS>>>::type;
            // Dimensions
            static constexpr int length = internal::traits<raw_type<LHS>>::length,
                width = internal::traits<raw_type<LHS>>::width,
                height = internal::traits<raw_type<LHS>>::height,
                area = internal::traits<raw_type<LHS>>::area,
                size = internal::traits<raw_type<LHS>>::size,
                indexingModeRequired = internal::traits<raw_type<LHS>>::indexingModeRequired;
            using StoredLHS = expression_stored_type<LHS>;
        };
    } /* internal */

    template <typename UnaryOperation, typename LHS>
    class UnaryExpr : public TileMapBase<UnaryExpr<UnaryOperation, LHS>> {
        public:
            // Store either a reference or copy depending on what the operand is.
            using StoredLHS = typename internal::traits<UnaryExpr>::StoredLHS;

            constexpr STEALTH_ALWAYS_INLINE UnaryExpr(UnaryOperation op, LHS&& lhs) noexcept
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
