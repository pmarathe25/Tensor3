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
            static constexpr STEALTH_ALWAYS_INLINE int width() noexcept {
                return internal::traits<Derived>::width;
            }

            static constexpr STEALTH_ALWAYS_INLINE int length() noexcept {
                return internal::traits<Derived>::length;
            }

            static constexpr STEALTH_ALWAYS_INLINE int height() noexcept {
                return internal::traits<Derived>::height;
            }

            static constexpr STEALTH_ALWAYS_INLINE int area() noexcept {
                return internal::traits<Derived>::area;
            }

            static constexpr STEALTH_ALWAYS_INLINE int size() noexcept {
                return internal::traits<Derived>::size;
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y, int z) const {
                return static_cast<const Derived*>(this) -> operator()(x, y, z);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y) const {
                return static_cast<const Derived*>(this) -> operator()(x, y);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x) const {
                return static_cast<const Derived*>(this) -> operator()(x);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator[](int x) const {
                return static_cast<const Derived*>(this) -> operator[](x);
            }

            constexpr STEALTH_ALWAYS_INLINE TileMap<ScalarType, width(), length(), height()> eval() const {
                return *(static_cast<const Derived*>(this));
            }
    };

    namespace {
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
    }

    template <typename Derived>
    std::ostream& operator<<(std::ostream& os, const TileMapBase<Derived>& tileMap) {
        for (int k = 0; k < tileMap.height(); ++k) {
            os << "Layer " << k << '\n';
            for (int j = 0; j < tileMap.length(); ++j) {
                for (int i = 0; i < tileMap.width(); ++i) {
                    os << internal_to_string(tileMap(i, j, k)) << " ";
                }
                os << '\n';
            }
        }
        return os;
    }
} /* StealthTileMap */

#endif /* end of include guard: TILE_MAP_BASE_H */
