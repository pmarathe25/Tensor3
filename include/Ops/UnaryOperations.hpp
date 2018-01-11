#ifndef UNARY_OPS_H
#define UNARY_OPS_H
#define DO_UNARY_OP(op) \
    /* If the type is smaller than a pointer, return a copy rather than a reference */ \
    typedef typename std::conditional<sizeof(typename internal::traits<LHS>::ScalarType) <= sizeof(void*), \
        typename internal::traits<LHS>::ScalarType, const typename internal::traits<LHS>::ScalarType&>::type ScalarTypeLHS; \
    typedef decltype(&op<ScalarTypeLHS>) FuncType; \
    typedef typename std::invoke_result<FuncType, ScalarTypeLHS>::type ReturnType; \
    return UnaryOp<ReturnType, ScalarTypeLHS, op, LHS>{lhs};
#include "../OpStructs/UnaryOp.hpp"

namespace StealthTileMap {
    template <typename LHS>
    constexpr auto operator!(const LHS& lhs) noexcept {
        DO_UNARY_OP(internal::ops::notOp);
    }

    template <typename ScalarTypeLHS, typename ReturnType, UnaryOperation<ReturnType, ScalarTypeLHS> op, typename LHS>
    constexpr auto transform(const LHS& lhs) noexcept {
        return UnaryOp<ReturnType, ScalarTypeLHS, op, LHS>{lhs};
    }
} /* StealthTileMap */

#undef DO_UNARY_OP
#endif /* end of include guard: UNARY_OPS_H */
