#ifndef TILE_MAP_VIEW_OPERATIONS_H
#define TILE_MAP_VIEW_OPERATIONS_H
#include "../Expressions/BlockExpr.hpp"

namespace Stealth {
    template <typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto layer(LHS&& lhs, int layerNum = 0) {
        typedef typename std::remove_reference<LHS>::type LHSRawType;
        return BlockExpr<lhs.width(), lhs.length(), 1, LHSRawType>
            {std::forward<LHS&&>(lhs), 0, 0, layerNum};
    }

    template <int width, int length, int height, typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto block(LHS&& lhs, int minX = 0, int minY = 0, int minZ = 0) {
        typedef typename std::remove_reference<LHS>::type LHSRawType;
        return BlockExpr<width, length, height, LHSRawType>{std::forward<LHS&&>(lhs), minX, minY, minZ};
    }

    template <int width, int length, typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto block(LHS&& lhs, int minX = 0, int minY = 0, int minZ = 0) {
        typedef typename std::remove_reference<LHS>::type LHSRawType;
        return BlockExpr<width, length, 1, LHSRawType>{std::forward<LHS&&>(lhs), minX, minY, minZ};
    }

    template <int width, typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto block(LHS&& lhs, int minX = 0, int minY = 0, int minZ = 0) {
        typedef typename std::remove_reference<LHS>::type LHSRawType;
        return BlockExpr<width, 1, 1, LHSRawType>{std::forward<LHS&&>(lhs), minX, minY, minZ};
    }
} /* Stealth */

#endif /* end of include guard: TILE_MAP_VIEW_OPERATIONS_H */
