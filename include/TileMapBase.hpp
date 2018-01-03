#ifndef TILE_MAP_BASE_H
#define TILE_MAP_BASE_H
#include "./ForwardDeclarations.hpp"

namespace StealthTileMap {
    template <typename Derived>
    class TileMapBase {
        public:
            typedef typename internal::traits<Derived>::ScalarType ScalarType;
            // Dimensions
            static constexpr int width() noexcept {
                return internal::traits<Derived>::width;
            }

            static constexpr int length() noexcept {
                return internal::traits<Derived>::length;
            }

            static constexpr int height() noexcept {
                return internal::traits<Derived>::height;
            }

            static constexpr int area() noexcept {
                return internal::traits<Derived>::area;
            }

            static constexpr int size() noexcept {
                return internal::traits<Derived>::size;
            }

            constexpr auto operator()(int x, int y, int z) const {
                return static_cast<const Derived*>(this) -> operator()(x, y, z);
            }

            constexpr auto operator()(int x, int y) const {
                return static_cast<const Derived*>(this) -> operator()(x, y);
            }

            constexpr auto operator()(int x) const {
                return static_cast<const Derived*>(this) -> operator()(x);
            }

            constexpr TileMap<ScalarType, width(), length(), height()> eval() const {
                return *(static_cast<const Derived*>(this));
            }
    };
} /* StealthTileMap */

#endif /* end of include guard: TILE_MAP_BASE_H */
