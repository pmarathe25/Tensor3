#ifndef RUNTIME_OPS_H
#define RUNTIME_OPS_H
#include "../OpStructs/RuntimeOp.hpp"

namespace StealthTileMap {
    template <typename LHS, typename RuntimeOperation>
    constexpr auto apply(const RuntimeOperation& op, const LHS& lhs) noexcept {
        return RuntimeOp<LHS, RuntimeOperation>{op, lhs};
    }
} /* StealthTileMap */

#endif
