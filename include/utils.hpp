#pragma once
#include <type_traits>

namespace Stealth::Math {
    // Method to create copies of objects.
    template <typename T>
    constexpr STEALTH_ALWAYS_INLINE auto copy(const T& copyable) {
        return T{copyable};
    }

    template <typename T>
    constexpr STEALTH_ALWAYS_INLINE auto move(T&& moveable) {
        return std::move(moveable);
    }

    // Remove const and reference
    template <typename QualifiedType>
    using raw_type = typename std::remove_const<typename std::remove_reference<QualifiedType>::type>::type;

    namespace {
        // Determine what expressions should store - a reference or a copy.
        // Generally want a reference for lvalues and copy for rvalues.
        template <typename Tensor3Type>
        using expr_ref = typename std::conditional<
            std::is_rvalue_reference<Tensor3Type>::value,
            // Make it a copy.
            raw_type<Tensor3Type>,
            // Otherwise, make it a reference.
            typename std::add_lvalue_reference<Tensor3Type>::type
            >::type;

        // If the scalar is large enough, use a const reference, otherwise pass by copy.
        template <typename Tensor3Type>
        using scalar_element = typename std::conditional<
            sizeof(typename internal::traits<Tensor3Type>::ScalarType) <= sizeof(void*),
            typename internal::traits<Tensor3Type>::ScalarType,
            const typename internal::traits<Tensor3Type>::ScalarType&
            >::type;
    }
}
