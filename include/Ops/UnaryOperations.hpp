#ifndef UNARY_OPS_H
#define UNARY_OPS_H
#define doUnaryOp(op) typedef typename internal::traits<LHS>::ScalarType ScalarTypeLHS; \
    typedef decltype(&op<ScalarTypeLHS>) FuncType; \
    typedef typename std::invoke_result<FuncType, ScalarTypeLHS>::type ReturnType; \
    return UnaryOp<ReturnType, const ScalarTypeLHS&, op, LHS>{lhs};
#include "../OpStructs/UnaryOp.hpp"

namespace StealthTileMap {
    template <typename LHS>
    constexpr auto operator!(const LHS& lhs) noexcept {
        doUnaryOp(internal::ops::notOp);
    }

    template <typename ReturnType, typename ScalarTypeLHS, UnaryOperation<ReturnType, ScalarTypeLHS> op, typename LHS>
    constexpr auto transform(const LHS& lhs) noexcept {
        return UnaryOp<ReturnType, ScalarTypeLHS, op, LHS>{lhs};
    }
} /* StealthTileMap */

#endif /* end of include guard: UNARY_OPS_H */
