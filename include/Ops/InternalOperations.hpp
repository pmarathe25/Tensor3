#ifndef INTERNAL_OPS_H
#define INTERNAL_OPS_H
#include <algorithm>
#include <type_traits>

namespace StealthTileMap::internal::ops {
    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto add(ScalarTypeLHS lhs, ScalarTypeRHS rhs) {
        return lhs + rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto subtract(ScalarTypeLHS lhs, ScalarTypeRHS rhs) {
        return lhs - rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto multiply(ScalarTypeLHS lhs, ScalarTypeRHS rhs) {
        return lhs * rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto divide(ScalarTypeLHS lhs, ScalarTypeRHS rhs) {
        return lhs / rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto eq(ScalarTypeLHS lhs, ScalarTypeRHS rhs) {
        return lhs == rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto less(ScalarTypeLHS lhs, ScalarTypeRHS rhs) {
        return lhs < rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto lessEq(ScalarTypeLHS lhs, ScalarTypeRHS rhs) {
        return lhs <= rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto greater(ScalarTypeLHS lhs, ScalarTypeRHS rhs) {
        return lhs > rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto greaterEq(ScalarTypeLHS lhs, ScalarTypeRHS rhs) {
        return lhs >= rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto andOp(ScalarTypeLHS lhs, ScalarTypeRHS rhs) {
        return lhs && rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto orOp(ScalarTypeLHS lhs, ScalarTypeRHS rhs) {
        return lhs || rhs;
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto min(ScalarTypeLHS lhs, ScalarTypeRHS rhs) {
        return std::min(lhs, rhs);
    }

    template <typename ScalarTypeLHS, typename ScalarTypeRHS>
    constexpr auto max(ScalarTypeLHS lhs, ScalarTypeRHS rhs) {
        return std::max(lhs, rhs);
    }

    template <typename ScalarTypeLHS>
    constexpr auto notOp(ScalarTypeLHS lhs) {
        return !lhs;
    }

} /* StealthTileMap::internal::ops */

#endif
