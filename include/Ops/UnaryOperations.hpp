#ifndef UNARY_OPS_H
#define UNARY_OPS_H
#include "../OpStructs/UnaryOp.hpp"

namespace StealthTileMap {
    template <typename Derived>
    constexpr UnaryOp<Derived, internal::ops::notOp> operator!(const Derived& lhs) noexcept {
        return UnaryOp<Derived, internal::ops::notOp>{lhs};
    }
} /* StealthTileMap */

#endif /* end of include guard: UNARY_OPS_H */
