#ifndef TILE_MAP_VIEW_H
#define TILE_MAP_VIEW_H
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"

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
    class TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, TileMapType, false>
        : public TileMapBase<TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, TileMapType, false>> {
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

            constexpr ScalarType& operator()(int x, int y) {
                int z = y / this -> length();
                y %= this -> length();
                return (*this)(x, y, z);
            }

            constexpr const ScalarType& operator()(int x, int y) const {
                int z = y / this -> length();
                y %= this -> length();
                return (*this)(x, y, z);
            }

            constexpr ScalarType& operator()(int x) {
                int y = x / this -> width();
                x %= this -> width();
                return (*this)(x, y);
            }

            constexpr const ScalarType& operator()(int x) const {
                int y = x / this -> width();
                x %= this -> width();
                return (*this)(x, y);
            }

            constexpr const ScalarType& operator[](int x) const {
                return this -> operator()(x);
            }

            constexpr const ScalarType* data() const noexcept {
                return &(this -> operator()(0));
            }

            constexpr ScalarType* data() noexcept {
                return &(this -> operator()(0));
            }
        private:
            TileMapType& tileMap;
            int minX, minY, minZ;
    };

    // A view of a temporary object. Cannot be modified.
    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename TileMapType>
    class TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, TileMapType, true>
        : public TileMapBase<TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, TileMapType, true>> {
        public:
            typedef typename internal::traits<TileMapView>::ScalarType ScalarType;

            constexpr TileMapView(const TileMapType& tileMap, int minX = 0, int minY = 0, int minZ = 0) noexcept
                : tileMap{tileMap}, minX{minX}, minY{minY}, minZ{minZ} { }

            constexpr ScalarType operator()(int x, int y, int z) const {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr ScalarType operator()(int x, int y) const {
                int z = y / this -> length();
                y %= this -> length();
                return (*this)(x, y, z);
            }

            constexpr ScalarType operator()(int x) const {
                int y = x / this -> width();
                x %= this -> width();
                return (*this)(x, y);
            }

            constexpr ScalarType operator[](int x) const {
                return this -> operator()(x);
            }
        private:
            const TileMapType& tileMap;
            int minX, minY, minZ;
    };

} /* StealthTileMap */

#endif /* end of include guard: TILE_MAP_VIEW_H */
