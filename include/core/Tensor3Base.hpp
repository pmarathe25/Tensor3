#pragma once
#include "ForwardDeclarations.hpp"
#include <ostream>

namespace Stealth::Tensor {
    template <typename Derived>
    class Tensor3Base {
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

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y, int z) const {
                return static_cast<const Derived*>(this) -> operator()(x, y, z);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y) const {
                return static_cast<const Derived*>(this) -> operator()(x, y);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x) const {
                return static_cast<const Derived*>(this) -> operator()(x);
            }

            constexpr STEALTH_ALWAYS_INLINE Tensor3<ScalarType, Tensor3Base::width(),
                Tensor3Base::length(), Tensor3Base::height()> eval() const {
                return *(static_cast<const Derived*>(this));
            }
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
    std::ostream& operator<<(std::ostream& os, const Tensor3Base<Derived>& tensor3) {
        for (int k = 0; k < tensor3.height(); ++k) {
            os << "Layer " << k << '\n';
            for (int j = 0; j < tensor3.length(); ++j) {
                for (int i = 0; i < tensor3.width(); ++i) {
                    os << internal_to_string(tensor3(i, j, k)) << " ";
                }
                os << '\n';
            }
        }
        return os;
    }
} /* Stealth::Tensor */
