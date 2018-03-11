#ifndef STEALTH_TILEMAP_UTILS_H
#define STEALTH_TILEMAP_UTILS_H
#include <type_traits>

namespace Stealth {
    namespace {
        // Remove const and reference
        template <typename QualifiedType>
        using strip_qualifiers = typename std::remove_const<typename std::remove_reference<QualifiedType>::type>::type;

        // Determine what expressions should store - a reference or a copy
        // (reference for lvalues and copy for rvalues)
        // If a TileMap does not contain data (i.e. is an expression, make a copy)
        template <typename TileMapType>
        using expression_stored_type = typename std::conditional<
            std::is_rvalue_reference<TileMapType>::value or not internal::traits<strip_qualifiers<TileMapType>>::containsData::value,
            // Otherwise, make it a copy.
            strip_qualifiers<TileMapType>,
            // Make it a reference.
            typename std::add_lvalue_reference<TileMapType>::type
        >::type;


        // If the scalar is large enough, use a const reference, otherwise pass by copy.
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
    }
}

#endif /* end of include guard: STEALTH_TILEMAP_UTILS_H */
