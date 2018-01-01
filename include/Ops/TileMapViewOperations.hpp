#ifndef TILE_MAP_VIEW_OPERATIONS_H
#define TILE_MAP_VIEW_OPERATIONS_H
#include "../OpStructs/TileMapView.hpp"
#include <cassert>

namespace StealthTileMap {
    template <typename LHS>
    constexpr auto layer(const LHS& lhs, int layerNum = 0) {
        assert(layerNum < internal::traits<LHS>::height);
        return TileMapView<internal::traits<LHS>::width, internal::traits<LHS>::length, 1, LHS>{lhs, 0, 0, layerNum};
    }

    template <typename LHS>
    constexpr auto layer(LHS& lhs, int layerNum = 0) {
        assert(layerNum < internal::traits<LHS>::height);
        return TileMapView<internal::traits<LHS>::width, internal::traits<LHS>::length, 1, LHS>{lhs, 0, 0, layerNum};
    }

} /* StealthTileMap */

#endif /* end of include guard: TILE_MAP_VIEW_OPERATIONS_H */
