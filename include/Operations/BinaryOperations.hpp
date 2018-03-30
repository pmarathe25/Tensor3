#pragma once
#include "../Expressions/ElemWiseBinaryExpr.hpp"
#include "../Functors/BinaryFunctors.hpp"

namespace StealthMath {
    // Helper to construct ElemWiseBinaryExpr expressions.
    template <typename BinaryOperation, typename LHS, typename RHS>
    constexpr auto apply(BinaryOperation&& op, LHS&& lhs, RHS&& rhs) noexcept {
        // These operations only apply to Tensor3 and its expression classes.
        static_assert(std::is_base_of<Tensor3Base<raw_type<LHS>>, raw_type<LHS>>::value
            and std::is_base_of<Tensor3Base<raw_type<RHS>>, raw_type<RHS>>::value,
            "Cannot perform binary operation on non-tensor3.");
        return ElemWiseBinaryExpr<LHS&&, BinaryOperation, RHS&&>{std::forward<LHS&&>(lhs), std::forward<BinaryOperation&&>(op), std::forward<RHS&&>(rhs)};
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator+(LHS&& lhs, RHS&& rhs) noexcept {
        return apply(
            internal::functors::add<scalar_element<LHS>, scalar_element<RHS>>{},
            std::forward<LHS&&>(lhs),
            std::forward<RHS&&>(rhs)
        );
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator-(LHS&& lhs, RHS&& rhs) noexcept {
        return apply(
            internal::functors::subtract<scalar_element<LHS>, scalar_element<RHS>>{},
            std::forward<LHS&&>(lhs),
            std::forward<RHS&&>(rhs)
        );
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto hadamard(LHS&& lhs, RHS&& rhs) noexcept {
        return apply(
            internal::functors::multiply<scalar_element<LHS>, scalar_element<RHS>>{},
            std::forward<LHS&&>(lhs),
            std::forward<RHS&&>(rhs)
        );
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator/(LHS&& lhs, RHS&& rhs) noexcept {
        return apply(
            internal::functors::divide<scalar_element<LHS>, scalar_element<RHS>>{},
            std::forward<LHS&&>(lhs),
            std::forward<RHS&&>(rhs)
        );
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator==(LHS&& lhs, RHS&& rhs) noexcept {
        return apply(
            internal::functors::eq<scalar_element<LHS>, scalar_element<RHS>>{},
            std::forward<LHS&&>(lhs),
            std::forward<RHS&&>(rhs)
        );
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator!=(LHS&& lhs, RHS&& rhs) noexcept {
        return apply(
            internal::functors::neq<scalar_element<LHS>, scalar_element<RHS>>{},
            std::forward<LHS&&>(lhs),
            std::forward<RHS&&>(rhs)
        );
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator<(LHS&& lhs, RHS&& rhs) noexcept {
        return apply(
            internal::functors::less<scalar_element<LHS>, scalar_element<RHS>>{},
            std::forward<LHS&&>(lhs),
            std::forward<RHS&&>(rhs)
        );
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator<=(LHS&& lhs, RHS&& rhs) noexcept {
        return apply(
            internal::functors::lessEq<scalar_element<LHS>, scalar_element<RHS>>{},
            std::forward<LHS&&>(lhs),
            std::forward<RHS&&>(rhs)
        );
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator>(LHS&& lhs, RHS&& rhs) noexcept {
        return apply(
            internal::functors::greater<scalar_element<LHS>, scalar_element<RHS>>{},
            std::forward<LHS&&>(lhs),
            std::forward<RHS&&>(rhs)
        );
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator>=(LHS&& lhs, RHS&& rhs) noexcept {
        return apply(
            internal::functors::greaterEq<scalar_element<LHS>, scalar_element<RHS>>{},
            std::forward<LHS&&>(lhs),
            std::forward<RHS&&>(rhs)
        );
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator&&(LHS&& lhs, RHS&& rhs) noexcept {
        return apply(
            internal::functors::andOp<scalar_element<LHS>, scalar_element<RHS>>{},
            std::forward<LHS&&>(lhs),
            std::forward<RHS&&>(rhs)
        );
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator||(LHS&& lhs, RHS&& rhs) noexcept {
        return apply(
            internal::functors::orOp<scalar_element<LHS>, scalar_element<RHS>>{},
            std::forward<LHS&&>(lhs),
            std::forward<RHS&&>(rhs)
        );
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto min(LHS&& lhs, RHS&& rhs) noexcept {
        return apply(
            internal::functors::min<scalar_element<LHS>, scalar_element<RHS>>{},
            std::forward<LHS&&>(lhs),
            std::forward<RHS&&>(rhs)
        );
    }

    template <typename LHS, typename RHS>
    constexpr STEALTH_ALWAYS_INLINE auto max(LHS&& lhs, RHS&& rhs) noexcept {
        return apply(
            internal::functors::max<scalar_element<LHS>, scalar_element<RHS>>{},
            std::forward<LHS&&>(lhs),
            std::forward<RHS&&>(rhs)
        );
    }
} /* StealthMath */
