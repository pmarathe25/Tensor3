#pragma once
#include "BlockExpr.hpp"

namespace Stealth::Tensor {
    template <int width = 1, int length = 1, int height = 1, typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto block(LHS&& lhs, int minX = 0, int minY = 0, int minZ = 0) noexcept {
        return BlockExpr<width, length, height, LHS&&>{std::forward<LHS&&>(lhs), minX, minY, minZ};
    }

    template <typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto layer(LHS&& lhs, int layerNum = 0) {
        return block<std::remove_reference_t<LHS>::width(), std::remove_reference_t<LHS>::length()>(std::forward<LHS&&>(lhs), 0, 0, layerNum);
    }
} /* Stealth::Tensor */
