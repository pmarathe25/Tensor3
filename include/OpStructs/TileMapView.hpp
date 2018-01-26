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
        constexpr STEALTH_ALWAYS_INLINE auto singleIndexAccess(TileMapViewType view, int x) -> typename std::invoke_result<TileMapViewType, int>::type {
            if constexpr (view.width() == internal::traits<InternalTileMap>::width
                && view.length() == internal::traits<InternalTileMap>::length) {
                // No need to deduce anything, dimensions are the same
                return view(x);
            } else if constexpr (view.length() == internal::traits<InternalTileMap>::length) {
                if constexpr (view.width() == 1) {
                    return view(0, x);
                } else {
                    int y = x / view.width();
                    x %= view.width();
                    return view(x, y);
                }
            } else if constexpr (view.width() == internal::traits<InternalTileMap>::width) {
                if constexpr (view.height() == 1) {
                    // Only 1 layer - all elements are continuous.
                    return view(x);
                } else if constexpr (view.width() == 1 && view.length() == 1) {
                    // We're just going up
                    return view(0, 0, x);
                } else {
                    // Need to figure out what height is.
                    int z = x / view.area();
                    x %= view.area();
                    return view(x, 0, z);
                }
            } else {
                // Otherwise we need to figure out everything.
                if constexpr (view.length() == 1 && view.width() == 1) {
                    // Z is the only coordinate needed.
                    return view(0, 0, x);
                } if constexpr (view.length() == 1) {
                    // No need to calculate y.
                    int z = x / view.width();
                    x %= view.width();
                    return view(x, 0, z);
                } else if constexpr (view.width() == 1) {
                    // x is always 0
                    int z = x / view.length();
                    int y = x % view.length();
                    return view(0, y, z);
                } else {
                    int y = x / view.width();
                    x %= view.width();
                    int z = y / view.length();
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

            constexpr STEALTH_ALWAYS_INLINE TileMapView(InternalTileMap& tileMap, int minX = 0, int minY = 0, int minZ = 0) noexcept
                : tileMap{tileMap}, minX{minX}, minY{minY}, minZ{minZ},
                offset{minX + minY * widthAtCompileTime + minZ * widthAtCompileTime * lengthAtCompileTime},
                offsetY{minY * widthAtCompileTime},
                offsetXZ{minX + minZ * widthAtCompileTime * lengthAtCompileTime} { }

            constexpr STEALTH_ALWAYS_INLINE auto& operator()(int x, int y, int z) {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int x, int y, int z) const {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr STEALTH_ALWAYS_INLINE auto& operator()(int x, int y) {
                return tileMap(x + offsetXZ, y + minY);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int x, int y) const {
                return tileMap(x + offsetXZ, y + minY);
            }

            constexpr STEALTH_ALWAYS_INLINE auto& operator()(int x) {
                return tileMap(x + offset);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int x) const {
                return tileMap(x + offset);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator[](int x) const {
                return singleIndexAccess<InternalTileMap, const TileMapView&>(*this, x);
            }

            constexpr STEALTH_ALWAYS_INLINE auto& operator[](int x) {
                return singleIndexAccess<InternalTileMap, TileMapView&>(*this, x);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto* data() const noexcept {
                return &(this -> operator[](0));
            }

            constexpr STEALTH_ALWAYS_INLINE auto* data() noexcept {
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

            constexpr STEALTH_ALWAYS_INLINE TileMapView(const InternalTileMap& tileMap, int minX = 0, int minY = 0, int minZ = 0) noexcept
                : tileMap{tileMap}, minX{minX}, minY{minY}, minZ{minZ},
                offset{minX + minY * widthAtCompileTime + minZ * widthAtCompileTime * lengthAtCompileTime},
                offsetY{minY * widthAtCompileTime},
                offsetXZ{minX + minZ * widthAtCompileTime * lengthAtCompileTime} { }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int x, int y, int z) const {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int x, int y) const {
                return tileMap(x + offsetXZ, y + minY);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int x) const {
                return tileMap(x + offset);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator[](int x) const {
                return singleIndexAccess<InternalTileMap, const TileMapView&>(*this, x);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto* data() const noexcept {
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

            constexpr STEALTH_ALWAYS_INLINE TileMapView(const InternalTileMap& tileMap, int minX = 0, int minY = 0, int minZ = 0) noexcept
                : tileMap{tileMap}, minX{minX}, minY{minY}, minZ{minZ},
                offset{minX + minY * widthAtCompileTime + minZ * widthAtCompileTime * lengthAtCompileTime},
                offsetY{minY * widthAtCompileTime},
                offsetXZ{minX + minZ * widthAtCompileTime * lengthAtCompileTime} { }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y, int z) const {
                return tileMap(x + minX, y + minY, z + minZ);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y) const {
                return tileMap(x + offsetXZ, y + minY);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x) const {
                return tileMap(x + offset);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator[](int x) const {
                return singleIndexAccess<InternalTileMap, const TileMapView&>(*this, x);
            }
        private:
            const InternalTileMap& tileMap;
            const int minX, minY, minZ;
            const int offset, offsetY, offsetXZ;
    };

} /* StealthTileMap */

#endif /* end of include guard: TILE_MAP_VIEW_H */
