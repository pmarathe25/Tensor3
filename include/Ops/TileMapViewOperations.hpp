#ifndef TILE_MAP_VIEW_OPERATIONS_H
#define TILE_MAP_VIEW_OPERATIONS_H
#include "../OpStructs/TileMapView.hpp"
#include <type_traits>
#include <cassert>

namespace StealthTileMap {
    template <typename LHS>
    constexpr auto layer(LHS& lhs, int layerNum = 0) {
        return TileMapView<internal::traits<LHS>::width, internal::traits<LHS>::length, 1, LHS>
            {lhs, 0, 0, layerNum};
    }

    template <int width, int length, int height, typename LHS>
    constexpr auto block(LHS& lhs, int minX = 0, int minY = 0, int minZ = 0) {
        return TileMapView<width, length, height, LHS>{lhs, minX, minY, minZ};
    }

    template <int width, int length, typename LHS>
    constexpr auto block(LHS& lhs, int minX = 0, int minY = 0) {
        return TileMapView<width, length, internal::traits<LHS>::height, LHS>
            {lhs, minX, minY, 0};
    }

    template <int width, typename LHS>
    constexpr auto block(LHS& lhs, int minX = 0) {
        return TileMapView<width, internal::traits<LHS>::length, internal::traits<LHS>::height, LHS>
            {lhs, minX, 0, 0};
    }

    template <typename LHS>
    constexpr auto layer(const LHS& lhs, int layerNum = 0) {
        return TileMapView<internal::traits<LHS>::width, internal::traits<LHS>::length, 1, LHS, false>
            {lhs, 0, 0, layerNum};
    }

    template <int width, int length, int height, typename LHS>
    constexpr auto block(const LHS& lhs, int minX = 0, int minY = 0, int minZ = 0) {
        return TileMapView<width, length, height, LHS, false>{lhs, minX, minY, minZ};
    }

    template <int width, int length, typename LHS>
    constexpr auto block(const LHS& lhs, int minX = 0, int minY = 0) {
        return TileMapView<width, length, internal::traits<LHS>::height, LHS, false>
            {lhs, minX, minY, 0};
    }

    template <int width, typename LHS>
    constexpr auto block(const LHS& lhs, int minX = 0) {
        return TileMapView<width, internal::traits<LHS>::length, internal::traits<LHS>::height, LHS, false>
            {lhs, minX, 0, 0};
    }
} /* StealthTileMap */

#endif /* end of include guard: TILE_MAP_VIEW_OPERATIONS_H */
