#pragma once
#include "../Expressions/ForwardDeclarations.hpp"
#include <algorithm>

namespace Stealth::Tensor::internal::functors {
    // Internal Binary Operations
    template <typename LHS, typename RHS>
    struct add {
        constexpr STEALTH_ALWAYS_INLINE auto operator()(LHS lhs, RHS rhs) const noexcept {
            return lhs + rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct subtract {
        constexpr STEALTH_ALWAYS_INLINE auto operator()(LHS lhs, RHS rhs) const noexcept {
            return lhs - rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct multiply {
        constexpr STEALTH_ALWAYS_INLINE auto operator()(LHS lhs, RHS rhs) const noexcept {
            return lhs * rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct divide {
        constexpr STEALTH_ALWAYS_INLINE auto operator()(LHS lhs, RHS rhs) const noexcept {
            return lhs / rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct eq {
        constexpr STEALTH_ALWAYS_INLINE auto operator()(LHS lhs, RHS rhs) const noexcept {
            return lhs == rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct neq {
        constexpr STEALTH_ALWAYS_INLINE auto operator()(LHS lhs, RHS rhs) const noexcept {
            return lhs != rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct less {
        constexpr STEALTH_ALWAYS_INLINE auto operator()(LHS lhs, RHS rhs) const noexcept {
            return lhs < rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct lessEq {
        constexpr STEALTH_ALWAYS_INLINE auto operator()(LHS lhs, RHS rhs) const noexcept {
            return lhs <= rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct greater {
        constexpr STEALTH_ALWAYS_INLINE auto operator()(LHS lhs, RHS rhs) const noexcept {
            return lhs > rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct greaterEq {
        constexpr STEALTH_ALWAYS_INLINE auto operator()(LHS lhs, RHS rhs) const noexcept {
            return lhs >= rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct andOp {
        constexpr STEALTH_ALWAYS_INLINE auto operator()(LHS lhs, RHS rhs) const noexcept {
            return lhs && rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct orOp {
        constexpr STEALTH_ALWAYS_INLINE auto operator()(LHS lhs, RHS rhs) const noexcept {
            return lhs || rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct min {
        constexpr STEALTH_ALWAYS_INLINE auto operator()(LHS lhs, RHS rhs) const noexcept {
            return std::min(lhs, rhs);
        }
    };

    template <typename LHS, typename RHS>
    struct max {
        constexpr STEALTH_ALWAYS_INLINE auto operator()(LHS lhs, RHS rhs) const noexcept {
            return std::max(lhs, rhs);
        }
    };
} /* Stealth::Tensor::internal::ops */
