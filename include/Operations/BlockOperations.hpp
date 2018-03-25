#pragma once
#include "../Expressions/BlockExpr.hpp"

namespace Stealth {
    template <typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto layer(LHS&& lhs, int layerNum = 0) {
        return BlockExpr<lhs.width(), lhs.length(), 1, LHS&&>
            {std::forward<LHS&&>(lhs), 0, 0, layerNum};
    }

    template <int width, int length, int height, typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto block(LHS&& lhs, int minX = 0, int minY = 0, int minZ = 0) noexcept {
        return BlockExpr<width, length, height, LHS&&>{std::forward<LHS&&>(lhs), minX, minY, minZ};
    }

    template <int width, int length, typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto block(LHS&& lhs, int minX = 0, int minY = 0, int minZ = 0) {
        return BlockExpr<width, length, 1, LHS&&>{std::forward<LHS&&>(lhs), minX, minY, minZ};
    }

    template <int width, typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto block(LHS&& lhs, int minX = 0, int minY = 0, int minZ = 0) {
        return BlockExpr<width, 1, 1, LHS&&>{std::forward<LHS&&>(lhs), minX, minY, minZ};
    }

} /* Stealth */
