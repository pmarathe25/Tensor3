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
        // Utility Functions

        // Remove const and reference
        template <typename QualifiedType>
        using strip_qualifiers = typename std::remove_const<typename std::remove_reference<QualifiedType>::type>::type;

        // Determine what expressions should store - a const ref or a copy
        // (const ref for lvalues and copy for rvalues)
        // If a TileMap does not contain data (i.e. is an expression, make a copy)
        template <typename TileMapType>
        using expression_stored_type = typename std::conditional<std::is_rvalue_reference<TileMapType>::value
            or not internal::traits<strip_qualifiers<TileMapType>>::containsData::value,
            // Otherwise, make it a copy.
            typename std::remove_reference<typename std::remove_const<TileMapType>::type>::type,
            // Make it a const reference.
            typename std::add_lvalue_reference<typename std::add_const<TileMapType>::type>::type
        >::type;


        // If the scalar is large enough, use a reference, otherwise pass by copy.
        template <typename TileMapType>
        using optimal_scalar_type = typename std::conditional<
            sizeof(typename internal::traits<strip_qualifiers<TileMapType>>::ScalarType) <= sizeof(void*),
            typename internal::traits<strip_qualifiers<TileMapType>>::ScalarType,
            const typename internal::traits<strip_qualifiers<TileMapType>>::ScalarType&
        >::type;

        // Function that tries to index a type if it is not a scalar.
        template <typename LHS>
        constexpr auto tryHintedIndex(LHS&& lhs, int hintX, int hintY, int x, int y, int z) {
            using LHSNoCV = strip_qualifiers<LHS>;
            if constexpr (std::is_scalar<LHSNoCV>::value) {
                return lhs;
            } else {
                return lhs.hintedIndex(hintX, hintY, x, y, z);
            }
        }

        // Function that tries to index a type if it is not a scalar.
        template <typename LHS, typename... Index>
        constexpr auto tryIndex(LHS&& lhs, Index&&... indices) {
            using LHSNoCV = strip_qualifiers<LHS>;
            if constexpr (std::is_scalar<LHSNoCV>::value) {
                return lhs;
            } else {
                return lhs(std::forward<Index&&>(indices)...);
            }
        }

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
