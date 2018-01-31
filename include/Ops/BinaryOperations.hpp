#ifndef BINARY_OPS_H
#define BINARY_OPS_H
#include "../OpStructs/BinaryOp.hpp"

namespace StealthTileMap {
    namespace internal::ops {
        // Internal Binary Operations
        template <typename LHS, typename RHS>
        struct add {
            constexpr STEALTH_ALWAYS_INLINE auto operator()(optimal_scalar_type<LHS> lhs, optimal_scalar_type<RHS> rhs) const noexcept {
                return lhs + rhs;
            }
        };

        template <typename LHS, typename RHS>
        struct subtract {
            constexpr STEALTH_ALWAYS_INLINE auto operator()(optimal_scalar_type<LHS> lhs, optimal_scalar_type<RHS> rhs) const noexcept {
                return lhs - rhs;
            }
        };

        template <typename LHS, typename RHS>
        struct multiply {
            constexpr STEALTH_ALWAYS_INLINE auto operator()(optimal_scalar_type<LHS> lhs, optimal_scalar_type<RHS> rhs) const noexcept {
                return lhs * rhs;
            }
        };

        template <typename LHS, typename RHS>
        struct divide {
            constexpr STEALTH_ALWAYS_INLINE auto operator()(optimal_scalar_type<LHS> lhs, optimal_scalar_type<RHS> rhs) const noexcept {
                return lhs / rhs;
            }
        };

        template <typename LHS, typename RHS>
        struct eq {
            constexpr STEALTH_ALWAYS_INLINE auto operator()(optimal_scalar_type<LHS> lhs, optimal_scalar_type<RHS> rhs) const noexcept {
                return lhs == rhs;
            }
        };

        template <typename LHS, typename RHS>
        struct neq {
            constexpr STEALTH_ALWAYS_INLINE auto operator()(optimal_scalar_type<LHS> lhs, optimal_scalar_type<RHS> rhs) const noexcept {
                return lhs != rhs;
            }
        };

        template <typename LHS, typename RHS>
        struct less {
            constexpr STEALTH_ALWAYS_INLINE auto operator()(optimal_scalar_type<LHS> lhs, optimal_scalar_type<RHS> rhs) const noexcept {
                return lhs < rhs;
            }
        };

        template <typename LHS, typename RHS>
        struct lessEq {
            constexpr STEALTH_ALWAYS_INLINE auto operator()(optimal_scalar_type<LHS> lhs, optimal_scalar_type<RHS> rhs) const noexcept {
                return lhs <= rhs;
            }
        };

        template <typename LHS, typename RHS>
        struct greater {
            constexpr STEALTH_ALWAYS_INLINE auto operator()(optimal_scalar_type<LHS> lhs, optimal_scalar_type<RHS> rhs) const noexcept {
                return lhs > rhs;
            }
        };

        template <typename LHS, typename RHS>
        struct greaterEq {
            constexpr STEALTH_ALWAYS_INLINE auto operator()(optimal_scalar_type<LHS> lhs, optimal_scalar_type<RHS> rhs) const noexcept {
                return lhs >= rhs;
            }
        };

        template <typename LHS, typename RHS>
        struct andOp {
            constexpr STEALTH_ALWAYS_INLINE auto operator()(optimal_scalar_type<LHS> lhs, optimal_scalar_type<RHS> rhs) const noexcept {
                return lhs && rhs;
            }
        };

        template <typename LHS, typename RHS>
        struct orOp {
            constexpr STEALTH_ALWAYS_INLINE auto operator()(optimal_scalar_type<LHS> lhs, optimal_scalar_type<RHS> rhs) const noexcept {
                return lhs || rhs;
            }
        };

        template <typename LHS, typename RHS>
        struct min {
            constexpr STEALTH_ALWAYS_INLINE auto operator()(optimal_scalar_type<LHS> lhs, optimal_scalar_type<RHS> rhs) const noexcept {
                return std::min(lhs, rhs);
            }
        };

        template <typename LHS, typename RHS>
        struct max {
            constexpr STEALTH_ALWAYS_INLINE auto operator()(optimal_scalar_type<LHS> lhs, optimal_scalar_type<RHS> rhs) const noexcept {
                return std::max(lhs, rhs);
            }
        };
    } /* StealthTileMap::internal::ops */

    template <typename BinaryOperation, typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto apply(const BinaryOperation& op, const LHS& lhs, const RHS& rhs) noexcept {
        return BinaryOp{op, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator+(const LHS& lhs, const RHS& rhs) noexcept {
        return BinaryOp{internal::ops::add<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator-(const LHS& lhs, const RHS& rhs) noexcept {
        return BinaryOp{internal::ops::subtract<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator*(const LHS& lhs, const RHS& rhs) noexcept {
        return BinaryOp{internal::ops::multiply<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator/(const LHS& lhs, const RHS& rhs) noexcept {
        return BinaryOp{internal::ops::divide<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator==(const LHS& lhs, const RHS& rhs) noexcept {
        return BinaryOp{internal::ops::eq<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator!=(const LHS& lhs, const RHS& rhs) noexcept {
        return BinaryOp{internal::ops::neq<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator<(const LHS& lhs, const RHS& rhs) noexcept {
        return BinaryOp{internal::ops::less<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator<=(const LHS& lhs, const RHS& rhs) noexcept {
        return BinaryOp{internal::ops::lessEq<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator>(const LHS& lhs, const RHS& rhs) noexcept {
        return BinaryOp{internal::ops::greater<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator>=(const LHS& lhs, const RHS& rhs) noexcept {
        return BinaryOp{internal::ops::greaterEq<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator&&(const LHS& lhs, const RHS& rhs) noexcept {
        return BinaryOp{internal::ops::andOp<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator||(const LHS& lhs, const RHS& rhs) noexcept {
        return BinaryOp{internal::ops::orOp<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto min(const LHS& lhs, const RHS& rhs) noexcept {
        return BinaryOp{internal::ops::min<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto max(const LHS& lhs, const RHS& rhs) noexcept {
        return BinaryOp{internal::ops::max<LHS, RHS>{}, lhs, rhs};
    }
} /* StealthTileMap */

#endif
