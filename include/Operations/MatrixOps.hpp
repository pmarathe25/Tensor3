#pragma once
#include "../Expressions/ForwardDeclarations.hpp"
#include "ElemWiseBinaryOps.hpp"

namespace Stealth::Tensor {
    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator*(LHS&& lhs, RHS&& rhs) {
        // If either one is a scalar, return a product.
        if constexpr (internal::traits<LHS>::is_scalar or internal::traits<RHS>::is_scalar) {
            return hadamard(std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
        } else {
            return 0;
        }
    }
} /* Stealth::Tensor */
