#ifndef RUNTIME_OPS_H
#define RUNTIME_OPS_H
#include "../OpStructs/UnaryOp.hpp"
#include "./InternalOperations.hpp"

namespace StealthTileMap {

    template <typename LHS, typename UnaryOperation>
    constexpr auto apply(const UnaryOperation& op, const LHS& lhs) noexcept {
        return UnaryOp{op, lhs};
    }

    template <typename LHS>
    constexpr auto operator!(const LHS& lhs) noexcept {
        return UnaryOp{internal::ops::notOp<LHS>{}, lhs};
    }
} /* StealthTileMap */

#endif
