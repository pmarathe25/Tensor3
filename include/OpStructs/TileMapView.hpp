#ifndef TILE_MAP_VIEW_H
#define TILE_MAP_VIEW_H
#include "../ForwardDeclarations.hpp"

namespace StealthTileMap {
    namespace internal {
        template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename TileMapType, bool temp>
        struct traits<TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, TileMapType, temp>> {
            typedef typename internal::traits<TileMapType>::ScalarType ScalarType;
            // Dimensions
            static constexpr int length = lengthAtCompileTime,
                width = widthAtCompileTime,
                height = heightAtCompileTime,
                area = length * width,
                size = area * height;
            static constexpr bool isTemporary = temp;
        };
    } /* internal */

    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename TileMapType>
    class TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, TileMapType, false> {
        public:
            typedef typename internal::traits<TileMapView>::ScalarType ScalarType;

            constexpr TileMapView(TileMapType& tileMap, int minX = 0, int minY = 0, int minZ = 0) noexcept
                : tileMap{tileMap}, minX{minX}, minY{minY}, minZ{minZ} { }

            constexpr ScalarType& operator()(int x, int y, int z) {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr const ScalarType& operator()(int x, int y, int z) const {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            // TODO: Implement accessors that accept fewer indices.
            constexpr ScalarType& operator()(int x, int y) {
                int z = y / length();
                y %= length();
                return (*this)(x, y, z);
            }

            constexpr const ScalarType& operator()(int x, int y) const {
                int z = y / length();
                y %= length();
                return (*this)(x, y, z);
            }

            constexpr ScalarType& operator()(int x) {
                int y = x / width();
                x %= width();
                return (*this)(x, y);
            }

            constexpr const ScalarType& operator()(int x) const {
                int y = x / width();
                x %= width();
                return (*this)(x, y);
            }

            // Dimensions
            static constexpr int width() noexcept {
                return internal::traits<TileMapView>::width;
            }

            static constexpr int length() noexcept {
                return internal::traits<TileMapView>::length;
            }

            static constexpr int height() noexcept {
                return internal::traits<TileMapView>::height;
            }

            static constexpr int area() noexcept {
                return internal::traits<TileMapView>::area;
            }

            static constexpr int size() noexcept {
                return internal::traits<TileMapView>::size;
            }

            constexpr TileMap<ScalarType, internal::traits<TileMapView>::width,
                internal::traits<TileMapView>::length, internal::traits<TileMapView>::height> eval() {
                return (*this);
            }
        private:
            TileMapType& tileMap;
            int minX, minY, minZ;
    };

    // A view of a temporary object. Cannot be modified.
    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename TileMapType>
    class TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, TileMapType, true> {
        public:
            typedef typename internal::traits<TileMapView>::ScalarType ScalarType;

            constexpr TileMapView(const TileMapType& tileMap, int minX = 0, int minY = 0, int minZ = 0) noexcept
                : tileMap{tileMap}, minX{minX}, minY{minY}, minZ{minZ} { }

            constexpr ScalarType operator()(int x, int y, int z) const {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            // TODO: Implement accessors that accept fewer indices.
            constexpr ScalarType operator()(int x, int y) const {
                int z = y / length();
                y %= length();
                return (*this)(x, y, z);
            }

            constexpr ScalarType operator()(int x) const {
                int y = x / width();
                x %= width();
                return (*this)(x, y);
            }

            // Dimensions
            static constexpr int width() noexcept {
                return internal::traits<TileMapView>::width;
            }

            static constexpr int length() noexcept {
                return internal::traits<TileMapView>::length;
            }

            static constexpr int height() noexcept {
                return internal::traits<TileMapView>::height;
            }

            static constexpr int area() noexcept {
                return internal::traits<TileMapView>::area;
            }

            static constexpr int size() noexcept {
                return internal::traits<TileMapView>::size;
            }

            constexpr TileMap<ScalarType, internal::traits<TileMapView>::width,
                internal::traits<TileMapView>::length, internal::traits<TileMapView>::height> eval() {
                return (*this);
            }
        private:
            const TileMapType& tileMap;
            int minX, minY, minZ;
    };

} /* StealthTileMap */

#endif /* end of include guard: TILE_MAP_VIEW_H */
