#pragma once
#include "./ForwardDeclarations.hpp"
#include "./Tensor3Base.hpp"
#include "./Operations/ElemWiseBinaryOps.hpp"
#include "./Operations/ElemWiseUnaryOps.hpp"
#include "./Operations/ElemWiseUnaryOps.hpp"
#include "./Operations/BlockOperations.hpp"
#include <vector>
#include <random>

#ifdef DEBUG
    #include <iostream>
#endif

namespace Stealth::Math {
    namespace internal {
        template <typename type, int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime,
            int areaAtCompileTime, int sizeAtCompileTime>
        struct traits<Tensor3<type, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, areaAtCompileTime, sizeAtCompileTime>> {
            static constexpr ExpressionType exprType = ExpressionType::Tensor3;
            using ScalarType = type;
            static constexpr int width = widthAtCompileTime,
                length = lengthAtCompileTime,
                height = heightAtCompileTime,
                area = areaAtCompileTime,
                size = sizeAtCompileTime,
                indexingMode = 1;
            static constexpr bool is_scalar = size == 1;
            static constexpr bool is_vector = !is_scalar and (width == size or length == size or height == size);
            static constexpr bool is_matrix = !is_vector and (width == 1 or length == 1 or height == 1);
        };
    } /* internal */

    template <typename ScalarType, int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime,
        int areaAtCompileTime, int sizeAtCompileTime>
    class Tensor3 : public Tensor3Base<Tensor3<ScalarType, widthAtCompileTime, lengthAtCompileTime,
        heightAtCompileTime, areaAtCompileTime, sizeAtCompileTime>> {
        public:
            constexpr STEALTH_ALWAYS_INLINE Tensor3() noexcept : mData(sizeAtCompileTime) { }

            constexpr STEALTH_ALWAYS_INLINE Tensor3(const std::initializer_list<ScalarType>& other) noexcept : mData(sizeAtCompileTime) {
                if (other.size() > sizeAtCompileTime) {
                    throw std::invalid_argument("Cannot initialize Tensor3 from incompatible initializer list");
                }
                mData = std::move(other);
            }

            // Copy
            constexpr STEALTH_ALWAYS_INLINE Tensor3(const Tensor3& other) noexcept : mData(sizeAtCompileTime) {
                copy(other);
            }

            template <typename OtherTensor3>
            constexpr STEALTH_ALWAYS_INLINE Tensor3(OtherTensor3&& other) noexcept : mData(sizeAtCompileTime) {
                copy(std::forward<OtherTensor3&&>(other));
            }

            constexpr STEALTH_ALWAYS_INLINE Tensor3& operator=(const Tensor3& other) noexcept {
                copy(other);
                return *this;
            }

            template <typename OtherTensor3>
            constexpr STEALTH_ALWAYS_INLINE Tensor3& operator=(OtherTensor3&& other) noexcept {
                copy(std::forward<OtherTensor3&&>(other));
                return *this;
            }

            // Move Constructor
            constexpr STEALTH_ALWAYS_INLINE Tensor3(Tensor3&& other) noexcept {
                move(other);
            }

            template <int width, int length, int height>
            constexpr STEALTH_ALWAYS_INLINE Tensor3(Tensor3<ScalarType, width, length, height>&& other) {
                move(other);
            }

            // Move Assignment
            constexpr STEALTH_ALWAYS_INLINE Tensor3& operator=(Tensor3&& other) noexcept {
                move(other);
                return *this;
            }

            template <typename OtherType, int width, int length, int height>
            constexpr STEALTH_ALWAYS_INLINE Tensor3& operator=(Tensor3<OtherType, width, length, height>&& other) {
                move(other);
                return *this;
            }

            // Accessors
            constexpr STEALTH_ALWAYS_INLINE auto& operator()(int x, int y, int z) {
                return mData[x + y * Tensor3::width() + z * Tensor3::area()];
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int x, int y, int z) const {
                return mData[x + y * Tensor3::width() + z * Tensor3::area()];
            }

            constexpr STEALTH_ALWAYS_INLINE auto& operator()(int x, int y) {
                return mData[x + y * Tensor3::width()];
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int x, int y) const {
                return mData[x + y * Tensor3::width()];
            }

            constexpr STEALTH_ALWAYS_INLINE auto& operator()(int x) {
                return mData[x];
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int x) const {
                return mData[x];
            }

            constexpr STEALTH_ALWAYS_INLINE const auto* data() const {
                static_assert(!std::is_same<ScalarType, bool>::value, "Cannot access data() member of boolean Tensor3");
                return mData.data();
            }

            constexpr STEALTH_ALWAYS_INLINE auto* data() {
                static_assert(!std::is_same<ScalarType, bool>::value, "Cannot access data() member of boolean Tensor3");
                return mData.data();
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& elements() const noexcept {
                return mData;
            }

            constexpr STEALTH_ALWAYS_INLINE auto& elements() noexcept {
                return mData;
            }

            constexpr STEALTH_ALWAYS_INLINE auto begin() noexcept {
                return mData.begin();
            }

            constexpr STEALTH_ALWAYS_INLINE auto cbegin() const noexcept {
                return mData.cbegin();
            }

            constexpr STEALTH_ALWAYS_INLINE auto end() noexcept {
                return mData.end();
            }

            constexpr STEALTH_ALWAYS_INLINE auto cend() const noexcept {
                return mData.cend();
            }

            template <typename OtherTensor3>
            constexpr STEALTH_ALWAYS_INLINE void operator+=(OtherTensor3&& other) {
                (*this) = (*this) + std::forward<OtherTensor3&&>(other);
            }

            template <typename OtherTensor3>
            constexpr STEALTH_ALWAYS_INLINE void operator*=(OtherTensor3&& other) {
                (*this) = (*this) * std::forward<OtherTensor3&&>(other);
            }

            template <typename OtherTensor3>
            constexpr STEALTH_ALWAYS_INLINE void operator-=(OtherTensor3&& other) {
                (*this) = (*this) - std::forward<OtherTensor3&&>(other);
            }

            template <typename OtherTensor3>
            constexpr STEALTH_ALWAYS_INLINE void operator/=(OtherTensor3&& other) {
                (*this) = (*this) / std::forward<OtherTensor3&&>(other);
            }

            constexpr STEALTH_ALWAYS_INLINE Tensor3& eval() {
                return (*this);
            }

            constexpr STEALTH_ALWAYS_INLINE const Tensor3& eval() const {
                return (*this);
            }
        private:
            std::vector<ScalarType> mData;

            constexpr void assign_scalar_impl(ScalarType scalar) {
                // Assign the scalar value to every element.
                for (int i = 0; i < Tensor3::size(); ++i) {
                    (*this)(i) = scalar;
                }
            }

            template <typename OtherTensor3>
            constexpr void copy_impl(OtherTensor3&& other) {
                static_assert(other.size() == Tensor3::size(), "Cannot copy incompatible Tensor3s.");
                constexpr int indexingModeToUse = std::max(internal::traits<Tensor3>::indexingMode,
                    internal::traits<OtherTensor3>::indexingMode);

                #ifdef DEBUG
                    std::cout << "\t\t!!!!Doing copy using indexing mode: " << indexingModeToUse << '\n';
                #endif

                if constexpr (indexingModeToUse == 1) {
                    // Treat it as a 1D array
                    #pragma omp parallel for simd
                    for (int i = 0; i < other.size(); ++i) {
                        (*this)(i) = other(i);
                    }
                } else if constexpr (indexingModeToUse == 2) {
                    // Treat it as a long 2D array.
                    #pragma omp parallel for simd
                    for (int j = 0; j < other.length() * other.height(); ++j) {
                        for (int i = 0; i < other.width(); ++i) {
                            (*this)(i + j * other.width()) = other(i, j);
                        }
                    }
                } else {
                    // Copy as 3D array.
                    #pragma omp parallel for simd
                    for (int z = 0; z < other.height(); ++z) {
                        for (int j = 0; j < other.length(); ++j) {
                            for (int i = 0; i < other.width(); ++i) {
                                (*this)(i + j * other.width() + z * other.area()) = other(i, j, z);
                            }
                        }
                    }
                }
            }

            template <typename OtherTensor3>
            constexpr void copy(OtherTensor3&& other) {
                // If the other thing is a scalar, use the copy scalar function.
                if constexpr (std::is_scalar<raw_type<OtherTensor3>>::value) {
                    return assign_scalar_impl(other);
                } else {
                    return copy_impl(std::forward<OtherTensor3&&>(other));
                }
            }

            template <typename OtherTensor3>
            constexpr void move_impl(OtherTensor3&& other) {
                if constexpr (std::is_scalar<raw_type<OtherTensor3>>::value) {
                    return assign_scalar_impl(other);
                }

                static_assert(other.size() == Tensor3::size(), "Cannot move incompatible Tensor3s");
                mData = std::move(other.elements());
            }

            template <typename OtherTensor3>
            constexpr void move(OtherTensor3&& other) {
                // If the other thing is a scalar, use the copy scalar function.
                if constexpr (std::is_scalar<raw_type<OtherTensor3>>::value) {
                    return assign_scalar_impl(other);
                } else {
                    return move_impl(std::forward<OtherTensor3&&>(other));
                }
            }

    };

} /* Stealth::Math */
