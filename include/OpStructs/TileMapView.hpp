#ifndef TILE_MAP_VIEW_H
#define TILE_MAP_VIEW_H
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"

namespace StealthTileMap {
    namespace internal {
        template <unsigned widthAtCompileTime, unsigned lengthAtCompileTime, unsigned heightAtCompileTime, typename InternalTileMap, typename dat, typename writable>
        struct traits<TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, dat, writable>> {
            typedef typename internal::traits<InternalTileMap>::ScalarType ScalarType;
            // Dimensions
            static constexpr unsigned length = lengthAtCompileTime,
                width = widthAtCompileTime,
                height = heightAtCompileTime,
                area = length * width,
                size = area * height;
            typedef dat containsData;
            typedef writable isWritable;
        };
    } /* internal */

    // Writable view
    template <unsigned widthAtCompileTime, unsigned lengthAtCompileTime, unsigned heightAtCompileTime, typename InternalTileMap>
    class TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::true_type, std::true_type>
        : public TileMapBase<TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::true_type, std::true_type>> {
        public:
            typedef typename internal::traits<TileMapView>::ScalarType ScalarType;

            constexpr TileMapView(InternalTileMap& tileMap, unsigned minX = 0, unsigned minY = 0, unsigned minZ = 0) noexcept
                : tileMap{tileMap}, minX{minX}, minY{minY}, minZ{minZ},
                offset1D{minX + minY * widthAtCompileTime + minZ * widthAtCompileTime * lengthAtCompileTime},
                offset2D{minX + minZ * widthAtCompileTime * lengthAtCompileTime} { }

            constexpr auto& operator()(unsigned x, unsigned y, unsigned z) {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr const auto& operator()(unsigned x, unsigned y, unsigned z) const {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr auto& operator()(unsigned x, unsigned y) {
                return tileMap(x + offset2D, y + minY);
            }

            constexpr const auto& operator()(unsigned x, unsigned y) const {
                return tileMap(x + offset2D, y + minY);
            }

            constexpr auto& operator()(unsigned x) {
                return tileMap(x + offset1D);
            }

            constexpr const auto& operator()(unsigned x) const {
                return tileMap(x + offset1D);
            }

            constexpr const auto& operator[](unsigned x) const {
                unsigned y = x / this -> width();
                unsigned z = y / this -> length();
                x %= this -> width();
                y %= this -> length();
                return this -> operator()(x, y, z);
            }

            constexpr auto& operator[](unsigned x) {
                unsigned y = x / this -> width();
                unsigned z = y / this -> length();
                x %= this -> width();
                y %= this -> length();
                return this -> operator()(x, y, z);
            }

            constexpr const auto* data() const noexcept {
                return &(this -> operator[](0));
            }

            constexpr auto* data() noexcept {
                return &(this -> operator[](0));
            }
        private:
            InternalTileMap& tileMap;
            const unsigned minX, minY, minZ;
            const unsigned offset1D, offset2D;
    };

    // Const view
    template <unsigned widthAtCompileTime, unsigned lengthAtCompileTime, unsigned heightAtCompileTime, typename InternalTileMap>
    class TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::true_type, std::false_type>
        : public TileMapBase<TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::true_type, std::false_type>> {
        public:
            typedef typename internal::traits<TileMapView>::ScalarType ScalarType;

            constexpr TileMapView(const InternalTileMap& tileMap, unsigned minX = 0, unsigned minY = 0, unsigned minZ = 0) noexcept
                : tileMap{tileMap}, minX{minX}, minY{minY}, minZ{minZ},
                offset1D{minX + minY * widthAtCompileTime + minZ * widthAtCompileTime * lengthAtCompileTime},
                offset2D{minX + minZ * widthAtCompileTime * lengthAtCompileTime} { }

            constexpr const auto& operator()(unsigned x, unsigned y, unsigned z) const {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr const auto& operator()(unsigned x, unsigned y) const {
                return tileMap(x + offset2D, y + minY);
            }

            constexpr const auto& operator()(unsigned x) const {
                return tileMap(x + offset1D);
            }

            constexpr const auto& operator[](unsigned x) const {
                unsigned y = x / this -> width();
                unsigned z = y / this -> length();
                x %= this -> width();
                y %= this -> length();
                return this -> operator()(x, y, z);
            }

            constexpr const auto* data() const noexcept {
                return &(this -> operator[](0));
            }
        private:
            const InternalTileMap& tileMap;
            const unsigned minX, minY, minZ;
            const unsigned offset1D, offset2D;
    };

    // A view of a temporary object. Cannot be modified.
    template <unsigned widthAtCompileTime, unsigned lengthAtCompileTime, unsigned heightAtCompileTime, typename InternalTileMap>
    class TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::false_type>
        : public TileMapBase<TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::false_type>> {
        public:
            typedef typename internal::traits<TileMapView>::ScalarType ScalarType;

            constexpr TileMapView(const InternalTileMap& tileMap, unsigned minX = 0, unsigned minY = 0, unsigned minZ = 0) noexcept
                : tileMap{tileMap}, minX{minX}, minY{minY}, minZ{minZ},
                offset1D{minX + minY * widthAtCompileTime + minZ * widthAtCompileTime * lengthAtCompileTime},
                offset2D{minX + minZ * widthAtCompileTime * lengthAtCompileTime} { }

            constexpr auto operator()(unsigned x, unsigned y, unsigned z) const {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr auto operator()(unsigned x, unsigned y) const {
                return tileMap(x + offset2D, y + minY);
            }

            constexpr auto operator()(unsigned x) const {
                return tileMap(x + offset1D);
            }

            constexpr auto operator[](unsigned x) const {
                unsigned y = x / this -> width();
                unsigned z = y / this -> length();
                x %= this -> width();
                y %= this -> length();
                return this -> operator()(x, y, z);
            }
        private:
            const InternalTileMap& tileMap;
            const unsigned minX, minY, minZ;
            const unsigned offset1D, offset2D;
    };

} /* StealthTileMap */

#endif /* end of include guard: TILE_MAP_VIEW_H */
