#ifndef TILE_MAP_BASE_H
#define TILE_MAP_BASE_H
#include "./ForwardDeclarations.hpp"
#include <ostream>

namespace StealthTileMap {
    template <typename Derived>
    class TileMapBase {
        public:
            typedef typename internal::traits<Derived>::ScalarType ScalarType;
            // Dimensions
            static constexpr std::size_t width() noexcept {
                return internal::traits<Derived>::width;
            }

            static constexpr std::size_t length() noexcept {
                return internal::traits<Derived>::length;
            }

            static constexpr std::size_t height() noexcept {
                return internal::traits<Derived>::height;
            }

            static constexpr std::size_t area() noexcept {
                return internal::traits<Derived>::area;
            }

            static constexpr std::size_t size() noexcept {
                return internal::traits<Derived>::size;
            }

            constexpr auto operator()(std::size_t x, std::size_t y, std::size_t z) const {
                return static_cast<const Derived*>(this) -> operator()(x, y, z);
            }

            constexpr auto operator()(std::size_t x, std::size_t y) const {
                return static_cast<const Derived*>(this) -> operator()(x, y);
            }

            constexpr auto operator()(std::size_t x) const {
                return static_cast<const Derived*>(this) -> operator()(x);
            }

            constexpr auto operator[](std::size_t x) const {
                return static_cast<const Derived*>(this) -> operator[](x);
            }

            constexpr TileMap<ScalarType, width(), length(), height()> eval() const {
                return *(static_cast<const Derived*>(this));
            }
    };

    // Utility Functions
    template <typename TileMapType>
    struct optimal_scalar_type {
        typedef typename std::conditional<sizeof(typename internal::traits<TileMapType>::ScalarType) <= sizeof(void*),
            typename internal::traits<TileMapType>::ScalarType, const typename internal::traits<TileMapType>::ScalarType&>::type type;
    };

    // Display Functions
    template <typename T>
    constexpr std::string internal_to_string(const T& i) {
        if constexpr (std::is_scalar<T>::value) {
            return std::to_string(i);
        } else {
            return to_string(i);
        }
    }

    template <>
    inline std::string internal_to_string(const std::string& tile) {
        return tile;
    }

    template <typename Derived>
    std::ostream& operator<<(std::ostream& os, const TileMapBase<Derived>& tileMap) {
        for (std::size_t k = 0; k < tileMap.height(); ++k) {
            os << "Layer " << k << '\n';
            for (std::size_t j = 0; j < tileMap.length(); ++j) {
                for (std::size_t i = 0; i < tileMap.width(); ++i) {
                    os << internal_to_string(tileMap(i, j, k)) << " ";
                }
                os << '\n';
            }
        }
        return os;
    }
} /* StealthTileMap */

#endif /* end of include guard: TILE_MAP_BASE_H */
