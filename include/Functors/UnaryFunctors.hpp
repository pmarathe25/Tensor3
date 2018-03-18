#pragma once
#include "../utils.hpp"

namespace Stealth::internal::functors {
    // Internal Unary Operations
    template <typename LHS>
    struct notOp {
        constexpr STEALTH_ALWAYS_INLINE auto operator()(optimal_scalar_type<LHS> lhs) const noexcept {
            return !lhs;
        }
    };
} /* Stealth::internal::ops */
