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
    constexpr auto createBinaryOp(BinaryOperation&& op, LHS&& lhs, RHS&& rhs) noexcept {
        using StoredLHS = expression_stored_type<LHS>;
        using StoredRHS = expression_stored_type<RHS>;

        #ifdef DEBUG
            std::cout << "Creating BinaryOp" << '\n';
            // LHS
            std::cout << "LHS Information\nLHS Type: ";
            debugType<LHS&&>();
            std::cout << "LHS Stored Type: ";
            debugType<expression_stored_type<LHS&&>>();
            std::cout << "LHS Stored Type (Actual): ";
            debugType<StoredLHS>();
            // RHS
            std::cout << "RHS Information\nRHS Type: ";
            debugType<RHS&&>();
            std::cout << "RHS Stored Type: ";
            debugType<expression_stored_type<RHS&&>>();
            std::cout << "RHS Stored Type (Actual): ";
            debugType<StoredRHS>();
        #endif

        return BinaryOp<BinaryOperation, StoredLHS, StoredRHS>{std::forward<BinaryOperation&&>(op), std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs)};
    }

    template <typename BinaryOperation, typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto apply(BinaryOperation&& op, LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(std::forward<BinaryOperation&&>(op), std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator+(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::ops::add<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator-(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::ops::subtract<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator*(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::ops::multiply<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator/(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::ops::divide<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator==(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::ops::eq<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator!=(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::ops::neq<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator<(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::ops::less<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator<=(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::ops::lessEq<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator>(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::ops::greater<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator>=(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::ops::greaterEq<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator&&(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::ops::andOp<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator||(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::ops::orOp<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto min(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::ops::min<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto max(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::ops::max<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }
} /* StealthTileMap */

#endif
