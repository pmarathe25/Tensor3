#ifndef UNARY_OPS_H
#define UNARY_OPS_H
#include "../OpStructs/UnaryOp.hpp"
#include "./InternalOperations.hpp"

namespace StealthTileMap {
    template <typename UnaryOperation, typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto apply(const UnaryOperation& op, const LHS& lhs) noexcept {
        return UnaryOp{op, lhs};
    }

    template <typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator!(const LHS& lhs) noexcept {
        return UnaryOp{internal::ops::notOp<LHS>{}, lhs};
    }
} /* StealthTileMap */

#endif
