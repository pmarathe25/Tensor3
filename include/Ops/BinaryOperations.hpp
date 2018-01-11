#ifndef BINARY_OPS_H
#define BINARY_OPS_H
#define DO_BINARY_OP(op) \
    checkTileMapCompatibility<LHS, RHS>(); \
    /* If the type is smaller than a pointer, return a copy rather than a reference */ \
    typedef typename std::conditional<sizeof(typename internal::traits<LHS>::ScalarType) <= sizeof(void*), \
        typename internal::traits<LHS>::ScalarType, const typename internal::traits<LHS>::ScalarType&>::type ScalarTypeLHS; \
    typedef typename std::conditional<sizeof(typename internal::traits<RHS>::ScalarType) <= sizeof(void*), \
        typename internal::traits<RHS>::ScalarType, const typename internal::traits<RHS>::ScalarType&>::type ScalarTypeRHS; \
    typedef decltype(&op<ScalarTypeLHS, ScalarTypeRHS>) FuncType; \
    typedef typename std::invoke_result<FuncType, ScalarTypeLHS, ScalarTypeRHS>::type ReturnType; \
    return BinaryOp<ReturnType, ScalarTypeLHS, ScalarTypeRHS, op, LHS, RHS>{lhs, rhs};
#include "../OpStructs/BinaryOp.hpp"
#include "./InternalOperations.hpp"

namespace StealthTileMap {
    template <typename LHS, typename RHS>
    constexpr void checkTileMapCompatibility() noexcept {
        static_assert(internal::traits<LHS>::size == internal::traits<RHS>::size
            || (internal::traits<LHS>::size == 1 || internal::traits<RHS>::size == 1),
            "Cannot operate on incompatible arguments");
    }

    template <typename LHS, typename RHS>
    constexpr auto operator+(const LHS& lhs, const RHS& rhs) noexcept {
        DO_BINARY_OP(internal::ops::add);
    }

    template <typename LHS, typename RHS>
    constexpr auto operator-(const LHS& lhs, const RHS& rhs) noexcept {
        DO_BINARY_OP(internal::ops::subtract);
    }

    template <typename LHS, typename RHS>
    constexpr auto operator*(const LHS& lhs, const RHS& rhs) noexcept {
        DO_BINARY_OP(internal::ops::multiply);
    }

    template <typename LHS, typename RHS>
    constexpr auto operator/(const LHS& lhs, const RHS& rhs) noexcept {
        DO_BINARY_OP(internal::ops::divide);
    }

    template <typename LHS, typename RHS>
    constexpr auto operator==(const LHS& lhs, const RHS& rhs) noexcept {
        DO_BINARY_OP(internal::ops::eq);
    }

    template <typename LHS, typename RHS>
    constexpr auto operator<(const LHS& lhs, const RHS& rhs) noexcept {
        DO_BINARY_OP(internal::ops::less);
    }

    template <typename LHS, typename RHS>
    constexpr auto operator<=(const LHS& lhs, const RHS& rhs) noexcept {
        DO_BINARY_OP(internal::ops::lessEq);
    }

    template <typename LHS, typename RHS>
    constexpr auto operator>(const LHS& lhs, const RHS& rhs) noexcept {
        DO_BINARY_OP(internal::ops::greater);
    }

    template <typename LHS, typename RHS>
    constexpr auto operator>=(const LHS& lhs, const RHS& rhs) noexcept {
        DO_BINARY_OP(internal::ops::greaterEq);
    }

    template <typename LHS, typename RHS>
    constexpr auto operator&&(const LHS& lhs, const RHS& rhs) noexcept {
        DO_BINARY_OP(internal::ops::andOp);
    }

    template <typename LHS, typename RHS>
    constexpr auto operator||(const LHS& lhs, const RHS& rhs) noexcept {
        DO_BINARY_OP(internal::ops::orOp);
    }

    template <typename LHS, typename RHS>
    constexpr auto min(const LHS& lhs, const RHS& rhs) noexcept {
        DO_BINARY_OP(internal::ops::min);
    }

    template <typename LHS, typename RHS>
    constexpr auto max(const LHS& lhs, const RHS& rhs) noexcept {
        DO_BINARY_OP(internal::ops::max);
    }
} /* StealthTileMap */

#undef DO_BINARY_OP
#endif
