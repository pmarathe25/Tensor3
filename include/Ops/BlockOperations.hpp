#ifndef TILE_MAP_VIEW_OPERATIONS_H
#define TILE_MAP_VIEW_OPERATIONS_H
#include "../OpStructs/Block.hpp"
#include <type_traits>
#include <cassert>

namespace StealthTileMap {
    template <typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto layer(LHS&& lhs, int layerNum = 0) {
        typedef typename std::remove_reference<LHS>::type LHSRawType;
        return Block<internal::traits<LHSRawType>::width, internal::traits<LHSRawType>::length, 1, LHSRawType>
            {std::forward<LHS&&>(lhs), 0, 0, layerNum};
    }

    template <int width, int length, int height, typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto block(LHS&& lhs, int minX = 0, int minY = 0, int minZ = 0) {
        typedef typename std::remove_reference<LHS>::type LHSRawType;
        return Block<width, length, height, LHSRawType>{std::forward<LHS&&>(lhs), minX, minY, minZ};
    }

    template <int width, int length, typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto block(LHS&& lhs, int minX = 0, int minY = 0) {
        typedef typename std::remove_reference<LHS>::type LHSRawType;
        return Block<width, length, internal::traits<LHSRawType>::height, LHSRawType>
            {std::forward<LHS&&>(lhs), minX, minY, 0};
    }

    template <int width, typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto block(LHS&& lhs, int minX = 0) {
        typedef typename std::remove_reference<LHS>::type LHSRawType;
        return Block<width, internal::traits<LHSRawType>::length, internal::traits<LHSRawType>::height, LHSRawType>
            {std::forward<LHS&&>(lhs), minX, 0, 0};
    }
} /* StealthTileMap */

#endif /* end of include guard: TILE_MAP_VIEW_OPERATIONS_H */
