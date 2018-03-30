#pragma once

namespace StealthMath::internal::functors {
    // Internal Unary Operations
    template <typename LHS>
    struct notOp {
        constexpr STEALTH_ALWAYS_INLINE auto operator()(LHS lhs) const noexcept {
            return !lhs;
        }
    };
} /* StealthMath::internal::ops */
