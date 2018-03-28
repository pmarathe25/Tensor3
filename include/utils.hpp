#pragma once
#include <type_traits>

namespace Stealth {
    namespace {
        // Remove const and reference
        template <typename QualifiedType>
        using raw_type = typename std::remove_const<typename std::remove_reference<QualifiedType>::type>::type;

        // Determine what expressions should store - a reference or a copy.
        // Generally want a reference for lvalues and copy for rvalues.
        // FIXME: If a Tensor3 does not contain data (i.e. is an expression, make a copy)
        template <typename Tensor3Type>
        using expression_stored_type = typename std::conditional<
            std::is_rvalue_reference<Tensor3Type>::value,
            // Make it a copy.
            raw_type<Tensor3Type>,
            // Otherwise, make it a reference.
            typename std::add_lvalue_reference<Tensor3Type>::type
        >::type;

        // If the scalar is large enough, use a const reference, otherwise pass by copy.
        template <typename Tensor3Type>
        using scalar_element = typename std::conditional<
            sizeof(typename internal::traits<raw_type<Tensor3Type>>::ScalarType) <= sizeof(void*),
            typename internal::traits<raw_type<Tensor3Type>>::ScalarType,
            const typename internal::traits<raw_type<Tensor3Type>>::ScalarType&
        >::type;
    }
}
