#pragma once
#include "../Expressions/ElemWiseBinaryExpr.hpp"
#include "../Functors/BinaryFunctors.hpp"
#include "../Expressions/ForwardDeclarations.hpp"

namespace Stealth::Tensor {
    // Helper to construct ElemWiseBinaryExpr expressions.
    template <typename BinaryOperation, typename _LHS, typename _RHS>
    constexpr auto apply(BinaryOperation&& op, _LHS&& lhs, _RHS&& rhs) noexcept {
        // If there's a scalar, we can create a Stealth::Tensor::Scalar from it.
        using LHS = tensor3_type<_LHS>;
        using RHS = tensor3_type<_RHS>;
        return ElemWiseBinaryExpr<LHS&&, BinaryOperation&&, RHS&&>{std::forward<LHS&&>(lhs), std::forward<BinaryOperation&&>(op), std::forward<RHS&&>(rhs)};
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
} /* Stealth::Tensor */
