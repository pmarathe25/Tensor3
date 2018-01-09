#ifndef TILE_MAP_VIEW_OPERATIONS_H
#define TILE_MAP_VIEW_OPERATIONS_H
#include "../OpStructs/TileMapView.hpp"
#include <type_traits>
#include <cassert>

namespace StealthTileMap {
    template <typename LHS>
    constexpr auto layer(LHS&& lhs, int layerNum = 0) {
        typedef typename std::remove_reference<LHS>::type LHSBaseType;
        return TileMapView<internal::traits<LHSBaseType>::width, internal::traits<LHSBaseType>::length, 1, LHSBaseType>
            (std::forward<LHS>(lhs), 0, 0, layerNum);
    }

    template <int width, int length, int height, typename LHS>
    constexpr auto block(LHS&& lhs, int minX = 0, int minY = 0, int minZ = 0) {
        typedef typename std::remove_reference<LHS>::type LHSBaseType;
        return TileMapView<width, length, height, LHSBaseType>{std::forward<LHS>(lhs), minX, minY, minZ};
    }

    template <int width, int length, typename LHS>
    constexpr auto block(LHS&& lhs, int minX = 0, int minY = 0) {
        typedef typename std::remove_reference<LHS>::type LHSBaseType;
        return TileMapView<width, length, internal::traits<LHSBaseType>::height, LHSBaseType>
            {std::forward<LHS>(lhs), minX, minY, 0};
    }

    template <int width, typename LHS>
    constexpr auto block(LHS&& lhs, int minX = 0) {
        typedef typename std::remove_reference<LHS>::type LHSBaseType;
        return TileMapView<width, internal::traits<LHSBaseType>::length, internal::traits<LHSBaseType>::height, LHSBaseType>
            {std::forward<LHS>(lhs), minX, 0, 0};
    }
} /* StealthTileMap */

#endif /* end of include guard: TILE_MAP_VIEW_OPERATIONS_H */
