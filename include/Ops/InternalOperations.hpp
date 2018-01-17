#ifndef INTERNAL_OPS_H
#define INTERNAL_OPS_H
#include <algorithm>
#include <type_traits>

namespace StealthTileMap::internal::ops {
    // Internal Binary Operations
    template <typename LHS, typename RHS>
    struct add {
        constexpr auto operator()(typename optimal_scalar_type<LHS>::type lhs, typename optimal_scalar_type<RHS>::type rhs) const noexcept {
            return lhs + rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct subtract {
        constexpr auto operator()(typename optimal_scalar_type<LHS>::type lhs, typename optimal_scalar_type<RHS>::type rhs) const noexcept {
            return lhs - rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct multiply {
        constexpr auto operator()(typename optimal_scalar_type<LHS>::type lhs, typename optimal_scalar_type<RHS>::type rhs) const noexcept {
            return lhs * rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct divide {
        constexpr auto operator()(typename optimal_scalar_type<LHS>::type lhs, typename optimal_scalar_type<RHS>::type rhs) const noexcept {
            return lhs / rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct eq {
        constexpr auto operator()(typename optimal_scalar_type<LHS>::type lhs, typename optimal_scalar_type<RHS>::type rhs) const noexcept {
            return lhs == rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct neq {
        constexpr auto operator()(typename optimal_scalar_type<LHS>::type lhs, typename optimal_scalar_type<RHS>::type rhs) const noexcept {
            return lhs != rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct less {
        constexpr auto operator()(typename optimal_scalar_type<LHS>::type lhs, typename optimal_scalar_type<RHS>::type rhs) const noexcept {
            return lhs < rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct lessEq {
        constexpr auto operator()(typename optimal_scalar_type<LHS>::type lhs, typename optimal_scalar_type<RHS>::type rhs) const noexcept {
            return lhs <= rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct greater {
        constexpr auto operator()(typename optimal_scalar_type<LHS>::type lhs, typename optimal_scalar_type<RHS>::type rhs) const noexcept {
            return lhs > rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct greaterEq {
        constexpr auto operator()(typename optimal_scalar_type<LHS>::type lhs, typename optimal_scalar_type<RHS>::type rhs) const noexcept {
            return lhs >= rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct andOp {
        constexpr auto operator()(typename optimal_scalar_type<LHS>::type lhs, typename optimal_scalar_type<RHS>::type rhs) const noexcept {
            return lhs && rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct orOp {
        constexpr auto operator()(typename optimal_scalar_type<LHS>::type lhs, typename optimal_scalar_type<RHS>::type rhs) const noexcept {
            return lhs || rhs;
        }
    };

    template <typename LHS, typename RHS>
    struct min {
        constexpr auto operator()(typename optimal_scalar_type<LHS>::type lhs, typename optimal_scalar_type<RHS>::type rhs) const noexcept {
            return std::min(lhs, rhs);
        }
    };

    template <typename LHS, typename RHS>
    struct max {
        constexpr auto operator()(typename optimal_scalar_type<LHS>::type lhs, typename optimal_scalar_type<RHS>::type rhs) const noexcept {
            return std::max(lhs, rhs);
        }
    };

    // Internal Unary Operations
    template <typename LHS>
    struct notOp {
        constexpr auto operator()(typename optimal_scalar_type<LHS>::type lhs) const noexcept {
            return !lhs;
        }
    };

} /* StealthTileMap::internal::ops */

#endif
