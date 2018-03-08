#ifndef BINARY_OPS_H
#define BINARY_OPS_H
#include "../Expressions/ElemWiseBinaryExpr.hpp"
#include "../Functors/BinaryFunctors.hpp"

namespace StealthTileMap {
    // Helper to construct ElemWiseBinaryExpr expressions.
    template <typename BinaryOperation, typename LHS, typename RHS>
    constexpr auto createBinaryOp(BinaryOperation&& op, LHS&& lhs, RHS&& rhs) noexcept {
        return ElemWiseBinaryExpr<BinaryOperation, LHS, RHS>{std::forward<BinaryOperation&&>(op), std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs)};
    }

    template <typename BinaryOperation, typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto apply(BinaryOperation&& op, LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(std::forward<BinaryOperation&&>(op), std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator+(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::functors::add<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator-(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::functors::subtract<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator*(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::functors::multiply<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator/(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::functors::divide<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator==(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::functors::eq<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator!=(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::functors::neq<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator<(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::functors::less<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator<=(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::functors::lessEq<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator>(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::functors::greater<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator>=(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::functors::greaterEq<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator&&(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::functors::andOp<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator||(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::functors::orOp<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto min(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::functors::min<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto max(LHS&& lhs, RHS&& rhs) noexcept {
        return createBinaryOp(internal::functors::max<LHS, RHS>{}, std::forward<LHS&&>(lhs), std::forward<RHS&&>(rhs));
    }
} /* StealthTileMap */

#endif
