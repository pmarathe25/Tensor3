#ifndef INTERNAL_OPS_H
#define INTERNAL_OPS_H
#include <algorithm>
#include <type_traits>

namespace StealthTileMap::internal::ops {
    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto add(const ScalarTypeLHS& lhs, const ScalarTypeRHS& rhs) {
        return lhs + rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto subtract(const ScalarTypeLHS& lhs, const ScalarTypeRHS& rhs) {
        return lhs - rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto multiply(const ScalarTypeLHS& lhs, const ScalarTypeRHS& rhs) {
        return lhs * rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto divide(const ScalarTypeLHS& lhs, const ScalarTypeRHS& rhs) {
        return lhs / rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto eq(const ScalarTypeLHS& lhs, const ScalarTypeRHS& rhs) {
        return lhs == rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto less(const ScalarTypeLHS& lhs, const ScalarTypeRHS& rhs) {
        return lhs < rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto lessEq(const ScalarTypeLHS& lhs, const ScalarTypeRHS& rhs) {
        return lhs <= rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto greater(const ScalarTypeLHS& lhs, const ScalarTypeRHS& rhs) {
        return lhs > rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto greaterEq(const ScalarTypeLHS& lhs, const ScalarTypeRHS& rhs) {
        return lhs >= rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto andOp(const ScalarTypeLHS& lhs, const ScalarTypeRHS& rhs) {
        return lhs && rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto orOp(const ScalarTypeLHS& lhs, const ScalarTypeRHS& rhs) {
        return lhs || rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto min(const ScalarTypeLHS& lhs, const ScalarTypeRHS& rhs) {
        return std::min(lhs, rhs);
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto max(const ScalarTypeLHS& lhs, const ScalarTypeRHS& rhs) {
        return std::max(lhs, rhs);
    }

    template <typename ScalarTypeLHS>
    constexpr auto notOp(const ScalarTypeLHS& lhs) {
        return !lhs;
    }

} /* StealthTileMap::internal::ops */

#endif
