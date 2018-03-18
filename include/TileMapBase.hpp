#pragma once
#include "./ForwardDeclarations.hpp"
#include <ostream>

namespace Stealth {
    template <typename Derived>
    class TileMapBase {
        public:
            typedef typename internal::traits<Derived>::ScalarType ScalarType;
            // Dimensions
            static constexpr STEALTH_ALWAYS_INLINE auto width() noexcept {
                return internal::traits<Derived>::width;
            }

            static constexpr STEALTH_ALWAYS_INLINE auto length() noexcept {
                return internal::traits<Derived>::length;
            }

            static constexpr STEALTH_ALWAYS_INLINE auto height() noexcept {
                return internal::traits<Derived>::height;
            }

            static constexpr STEALTH_ALWAYS_INLINE auto area() noexcept {
                return internal::traits<Derived>::area;
            }

            static constexpr STEALTH_ALWAYS_INLINE auto size() noexcept {
                return internal::traits<Derived>::size;
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& hintedIndex(int hintX, int hintY, int x, int y, int z) const {
                return static_cast<const Derived*>(this) -> hintedIndex(hintX, hintY, x, y, z);
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

            constexpr STEALTH_ALWAYS_INLINE TileMap<ScalarType, TileMapBase::width(),
                TileMapBase::length(), TileMapBase::height()> eval() const {
                return *(static_cast<const Derived*>(this));
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& underlyingTileMap() const {
                return *(static_cast<const Derived*>(this));
            }

            static constexpr int minX = 0, minY = 0, minZ = 0;
    };

    namespace {
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
} /* Stealth */
