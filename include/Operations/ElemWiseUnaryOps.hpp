#pragma once
#include "ElemWiseUnaryExpr.hpp"
#include "UnaryFunctors.hpp"

namespace Stealth::Tensor {
    // Helper to construct ElemWiseUnaryExpr expressions.
    template <typename UnaryOperation, typename LHS>
    constexpr auto apply(UnaryOperation&& op, LHS&& lhs) noexcept {
        return ElemWiseUnaryExpr<UnaryOperation&&, LHS&&>{std::forward<UnaryOperation&&>(op), std::forward<LHS&&>(lhs)};
    }

    template <typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator!(LHS&& lhs) noexcept {
        return apply(
            internal::functors::notOp<scalar_element<LHS>>{},
            std::forward<LHS&&>(lhs)
        );
    }
} /* Stealth::Tensor */
