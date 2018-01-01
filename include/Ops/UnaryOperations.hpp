#ifndef UNARY_OPS_H
#define UNARY_OPS_H
#include "../OpStructs/UnaryOp.hpp"

namespace StealthTileMap {
    template <typename LHS>
    constexpr UnaryOp<LHS, internal::ops::notOp> operator!(const LHS& lhs) noexcept {
        return UnaryOp<LHS, internal::ops::notOp>{lhs};
    }
} /* StealthTileMap */

#endif /* end of include guard: UNARY_OPS_H */
