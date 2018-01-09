#ifndef BINARY_OPS_H
#define BINARY_OPS_H
#define doBinaryOp(op) \
    checkTileMapCompatibility<LHS, RHS>(); \
    typedef typename internal::traits<LHS>::ScalarType ScalarTypeLHS; \
    typedef typename internal::traits<RHS>::ScalarType ScalarTypeRHS; \
    typedef decltype(&op<ScalarTypeLHS, ScalarTypeRHS>) FuncType; \
    typedef typename std::invoke_result<FuncType, ScalarTypeLHS, ScalarTypeRHS>::type ReturnType; \
    /* If the type is smaller than a pointer, return a copy rather than a reference */ \
    if constexpr (sizeof(ScalarTypeLHS) <= sizeof(void*) && sizeof(ScalarTypeRHS) <= sizeof(void*)) { \
        return BinaryOp<ReturnType, ScalarTypeLHS, ScalarTypeRHS, op, LHS, RHS>{lhs, rhs}; \
    } else if constexpr (sizeof(ScalarTypeLHS) <= sizeof(void*)) { \
        return BinaryOp<ReturnType, ScalarTypeLHS, const ScalarTypeRHS&, op, LHS, RHS>{lhs, rhs}; \
    } else if constexpr (sizeof(ScalarTypeRHS) <= sizeof(void*)) { \
        return BinaryOp<ReturnType, const ScalarTypeLHS&, ScalarTypeRHS, op, LHS, RHS>{lhs, rhs}; \
    } else { \
        return BinaryOp<ReturnType, const ScalarTypeLHS&, const ScalarTypeRHS&, op, LHS, RHS>{lhs, rhs}; \
    }
#include "../OpStructs/BinaryOp.hpp"
#include "./InternalOperations.hpp"

namespace StealthTileMap {
    template <typename LHS, typename RHS>
    constexpr void checkTileMapCompatibility() noexcept {
        static_assert(internal::traits<LHS>::length == internal::traits<RHS>::length
            && internal::traits<LHS>::width == internal::traits<RHS>::width
            && internal::traits<LHS>::height == internal::traits<RHS>::height
            || (internal::traits<LHS>::size == 1 || internal::traits<RHS>::size == 1),
            "Cannot operate on incompatible arguments");
    }

    template <typename LHS, typename RHS>
    constexpr auto operator+(const LHS& lhs, const RHS& rhs) noexcept {
        doBinaryOp(internal::ops::add);
    }

    template <typename LHS, typename RHS>
    constexpr auto operator-(const LHS& lhs, const RHS& rhs) noexcept {
        doBinaryOp(internal::ops::subtract);
    }

    template <typename LHS, typename RHS>
    constexpr auto operator*(const LHS& lhs, const RHS& rhs) noexcept {
        doBinaryOp(internal::ops::multiply);
    }

    template <typename LHS, typename RHS>
    constexpr auto operator/(const LHS& lhs, const RHS& rhs) noexcept {
        doBinaryOp(internal::ops::divide);
    }

    template <typename LHS, typename RHS>
    constexpr auto operator==(const LHS& lhs, const RHS& rhs) noexcept {
        doBinaryOp(internal::ops::eq);
    }

    template <typename LHS, typename RHS>
    constexpr auto operator<(const LHS& lhs, const RHS& rhs) noexcept {
        doBinaryOp(internal::ops::less);
    }

    template <typename LHS, typename RHS>
    constexpr auto operator<=(const LHS& lhs, const RHS& rhs) noexcept {
        doBinaryOp(internal::ops::lessEq);
    }

    template <typename LHS, typename RHS>
    constexpr auto operator>(const LHS& lhs, const RHS& rhs) noexcept {
        doBinaryOp(internal::ops::greater);
    }

    template <typename LHS, typename RHS>
    constexpr auto operator>=(const LHS& lhs, const RHS& rhs) noexcept {
        doBinaryOp(internal::ops::greaterEq);
    }

    template <typename LHS, typename RHS>
    constexpr auto operator&&(const LHS& lhs, const RHS& rhs) noexcept {
        doBinaryOp(internal::ops::andOp);
    }

    template <typename LHS, typename RHS>
    constexpr auto operator||(const LHS& lhs, const RHS& rhs) noexcept {
        doBinaryOp(internal::ops::orOp);
    }

    template <typename LHS, typename RHS>
    constexpr auto min(const LHS& lhs, const RHS& rhs) noexcept {
        doBinaryOp(internal::ops::min);
    }

    template <typename LHS, typename RHS>
    constexpr auto max(const LHS& lhs, const RHS& rhs) noexcept {
        doBinaryOp(internal::ops::max);
    }
} /* StealthTileMap */

#endif
