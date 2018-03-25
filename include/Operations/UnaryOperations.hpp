#pragma once
#include "../Expressions/UnaryExpr.hpp"
#include "../Functors/UnaryFunctors.hpp"

namespace Stealth {
    // Helper to construct UnaryExpr expressions.
    template <typename UnaryOperation, typename LHS>
    constexpr auto apply(UnaryOperation&& op, LHS&& lhs) noexcept {
        return UnaryExpr<UnaryOperation, LHS&&>{std::forward<UnaryOperation&&>(op), std::forward<LHS&&>(lhs)};
    }

    template <typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator!(LHS&& lhs) noexcept {
        return apply(internal::functors::notOp<scalar_element<LHS>>{}, std::forward<LHS&&>(lhs));
    }
} /* Stealth */
