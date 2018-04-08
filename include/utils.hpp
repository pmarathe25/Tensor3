#pragma once
#include <type_traits>
#include <Stealth/util>

namespace Stealth::Tensor {
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

        // Determines what this type would be in Tensor3 terms (i.e. convert to a Tensor::Scalar if possible).
        template <typename LHS>
        using tensor3_type = typename std::conditional<std::is_scalar<raw_type<LHS>>::value, Scalar<raw_type<LHS>>, LHS>::type;

        // If the scalar is large enough, use a const reference, otherwise pass by copy.
        template <typename Tensor3Type>
        using scalar_element = typename std::conditional<
            sizeof(typename internal::traits<Tensor3Type>::ScalarType) <= sizeof(void*),
            typename internal::traits<Tensor3Type>::ScalarType,
            const typename internal::traits<Tensor3Type>::ScalarType&
            >::type;
    }
}
