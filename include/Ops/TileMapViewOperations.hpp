#ifndef TILE_MAP_VIEW_OPERATIONS_H
#define TILE_MAP_VIEW_OPERATIONS_H
#include "../OpStructs/TileMapView.hpp"
#include <type_traits>
#include <cassert>

namespace StealthTileMap {
    template <typename LHS>
    constexpr auto layer(LHS&& lhs, int layerNum = 0) {
        assert(layerNum < internal::traits<typename std::remove_reference<LHS>::type>::height);
        return TileMapView<internal::traits<typename std::remove_reference<LHS>::type>::width,
            internal::traits<typename std::remove_reference<LHS>::type>::length, 1,
            typename std::remove_reference<LHS>::type>{std::forward<LHS>(lhs), 0, 0, layerNum};
    }

} /* StealthTileMap */

#endif /* end of include guard: TILE_MAP_VIEW_OPERATIONS_H */
