#ifndef RUNTIME_OPS_H
#define RUNTIME_OPS_H
#include "../OpStructs/RuntimeOp.hpp"

namespace StealthTileMap {
    template <typename Derived, typename RuntimeOperation>
    constexpr RuntimeOp<Derived, RuntimeOperation> apply(const RuntimeOperation& op, const Derived& lhs) noexcept {
        return RuntimeOp<Derived, RuntimeOperation>{op, lhs};
    }
} /* StealthTileMap */

#endif
