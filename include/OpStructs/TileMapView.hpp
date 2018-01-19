#ifndef TILE_MAP_VIEW_H
#define TILE_MAP_VIEW_H
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"

namespace StealthTileMap {
    namespace internal {
        template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename InternalTileMap, typename dat, typename writable>
        struct traits<TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, dat, writable>> {
            typedef typename internal::traits<InternalTileMap>::ScalarType ScalarType;
            // Dimensions
            static constexpr int length = lengthAtCompileTime,
                width = widthAtCompileTime,
                height = heightAtCompileTime,
                area = length * width,
                size = area * height;
            typedef dat containsData;
            typedef writable isWritable;
        };
    } /* internal */

    namespace {
        template <typename InternalTileMap, typename TileMapViewType>
        constexpr auto& singleIndexAccess(TileMapViewType& view, int x) {
            if constexpr (view.width() == internal::traits<InternalTileMap>::width
                && view.length() == internal::traits<InternalTileMap>::length) {
                // No need to deduce anything, dimensions are the same
                return view(x);
            } else if constexpr (view.width() == internal::traits<InternalTileMap>::width) {
                if constexpr (view.width() == 1) {
                    // There's only a single column - no need to calculate y
                    return view(0, x);
                } else if constexpr(view.length() == 1) {
                    // Just a single row
                    return view(x);
                } else {
                    // Need to figure out what y is.
                    int y = x / view.width();
                    x %= view.width();
                    return view(x, y);
                }
            } else {
                // Otherwise we need to figure out everything.
                if constexpr (view.length() == 1 && view.width() == 1) {
                    // Z is the only coordinate needed.
                    return view(0, 0, x);
                } if constexpr (view.length() == 1) {
                    // No need to calculate z because z = y.
                    int y = x / view.width();
                    return view(x, 0, y);
                } else if constexpr (view.width() == 1) {
                    // x is always 0
                    int z = x / view.length();
                    int y = x % view.length();
                    return view(0, y, z);
                } else {
                    int y = x / view.width();
                    int z = y / view.length();
                    x %= view.width();
                    y %= view.length();
                    return view(x, y, z);
                }
            }
        }

        template <typename InternalTileMap, typename TileMapViewType>
        constexpr const auto& singleIndexAccess(const TileMapViewType& view, int x) {
            if constexpr (view.width() == internal::traits<InternalTileMap>::width
                && view.length() == internal::traits<InternalTileMap>::length) {
                // No need to deduce anything, dimensions are the same
                return view(x);
            } else if constexpr (view.width() == internal::traits<InternalTileMap>::width) {
                if constexpr (view.width() == 1) {
                    // There's only a single column - no need to calculate y
                    return view(0, x);
                } else if constexpr(view.length() == 1) {
                    // Just a single row
                    return view(x);
                } else {
                    // Need to figure out what y is.
                    int y = x / view.width();
                    x %= view.width();
                    return view(x, y);
                }
            } else {
                // Otherwise we need to figure out everything.
                if constexpr (view.length() == 1 && view.width() == 1) {
                    // Z is the only coordinate needed.
                    return view(0, 0, x);
                } if constexpr (view.length() == 1) {
                    // No need to calculate z because z = y.
                    int y = x / view.width();
                    return view(x, 0, y);
                } else if constexpr (view.width() == 1) {
                    // x is always 0
                    int z = x / view.length();
                    int y = x % view.length();
                    return view(0, y, z);
                } else {
                    int y = x / view.width();
                    int z = y / view.length();
                    x %= view.width();
                    y %= view.length();
                    return view(x, y, z);
                }
            }
        }
    }

    // Writable view
    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename InternalTileMap>
    class TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::true_type, std::true_type>
        : public TileMapBase<TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::true_type, std::true_type>> {
        public:
            typedef typename internal::traits<TileMapView>::ScalarType ScalarType;

            constexpr TileMapView(InternalTileMap& tileMap, int minX = 0, int minY = 0, int minZ = 0) noexcept
                : tileMap{tileMap}, minX{minX}, minY{minY}, minZ{minZ},
                offset{minX + minY * widthAtCompileTime + minZ * widthAtCompileTime * lengthAtCompileTime},
                offsetY{minY * widthAtCompileTime},
                offsetXZ{minX + minZ * widthAtCompileTime * lengthAtCompileTime} { }

            constexpr auto& operator()(int x, int y, int z) {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr const auto& operator()(int x, int y, int z) const {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr auto& operator()(int x, int y) {
                return tileMap(x + offsetXZ, y + minY);
            }

            constexpr const auto& operator()(int x, int y) const {
                return tileMap(x + offsetXZ, y + minY);
            }

            constexpr auto& operator()(int x) {
                return tileMap(x + offset);
            }

            constexpr const auto& operator()(int x) const {
                return tileMap(x + offset);
            }

            constexpr const auto& operator[](int x) const {
                return singleIndexAccess<InternalTileMap>(*this, x);
            }

            constexpr auto& operator[](int x) {
                return singleIndexAccess<InternalTileMap>(*this, x);
            }

            constexpr const auto* data() const noexcept {
                return &(this -> operator[](0));
            }

            constexpr auto* data() noexcept {
                return &(this -> operator[](0));
            }
        private:
            InternalTileMap& tileMap;
            const int minX, minY, minZ;
            const int offset, offsetY, offsetXZ;
    };

    // Const view
    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename InternalTileMap>
    class TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::true_type, std::false_type>
        : public TileMapBase<TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::true_type, std::false_type>> {
        public:
            typedef typename internal::traits<TileMapView>::ScalarType ScalarType;

            constexpr TileMapView(const InternalTileMap& tileMap, int minX = 0, int minY = 0, int minZ = 0) noexcept
                : tileMap{tileMap}, minX{minX}, minY{minY}, minZ{minZ},
                offset{minX + minY * widthAtCompileTime + minZ * widthAtCompileTime * lengthAtCompileTime},
                offsetY{minY * widthAtCompileTime},
                offsetXZ{minX + minZ * widthAtCompileTime * lengthAtCompileTime} { }

            constexpr const auto& operator()(int x, int y, int z) const {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr const auto& operator()(int x, int y) const {
                return tileMap(x + offsetXZ, y + minY);
            }

            constexpr const auto& operator()(int x) const {
                return tileMap(x + offset);
            }

            constexpr const auto& operator[](int x) const {
                return singleIndexAccess<InternalTileMap>(*this, x);
            }

            constexpr const auto* data() const noexcept {
                return &(this -> operator[](0));
            }
        private:
            const InternalTileMap& tileMap;
            const int minX, minY, minZ;
            const int offset, offsetY, offsetXZ;
    };

    // A view of a temporary object. Cannot be modified.
    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename InternalTileMap>
    class TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::false_type>
        : public TileMapBase<TileMapView<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, InternalTileMap, std::false_type>> {
        public:
            typedef typename internal::traits<TileMapView>::ScalarType ScalarType;

            constexpr TileMapView(const InternalTileMap& tileMap, int minX = 0, int minY = 0, int minZ = 0) noexcept
                : tileMap{tileMap}, minX{minX}, minY{minY}, minZ{minZ},
                offset{minX + minY * widthAtCompileTime + minZ * widthAtCompileTime * lengthAtCompileTime},
                offsetY{minY * widthAtCompileTime},
                offsetXZ{minX + minZ * widthAtCompileTime * lengthAtCompileTime} { }

            constexpr auto operator()(int x, int y, int z) const {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr auto operator()(int x, int y) const {
                return tileMap(x + offsetXZ, y + minY);
            }

            constexpr auto operator()(int x) const {
                return tileMap(x + offset);
            }

            constexpr auto operator[](int x) const {
                return singleIndexAccess<InternalTileMap>(*this, x);
            }
        private:
            const InternalTileMap& tileMap;
            const int minX, minY, minZ;
            const int offset, offsetY, offsetXZ;
    };

} /* StealthTileMap */

#endif /* end of include guard: TILE_MAP_VIEW_H */
