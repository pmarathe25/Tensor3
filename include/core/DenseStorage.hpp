#pragma once
#include <array>
#include <memory>
#include <iostream>

namespace Stealth::Tensor::internal {
    // Below this threshold, small storage optimizations apply.
    constexpr int kSMALL_THRESHOLD = 16;

    template <int sizeAtCompileTime>
    constexpr bool requiresHeapAllocation() {
        return sizeAtCompileTime > kSMALL_THRESHOLD;
    }

    // In most cases, do a heap allocation, but for small sizes, use in-object storage.
    template <typename ScalarType, int sizeAtCompileTime, bool isLarge = requiresHeapAllocation<sizeAtCompileTime>()>
    class InternalContainer { };

    template <typename ScalarType, int sizeAtCompileTime>
    class InternalContainer<ScalarType, sizeAtCompileTime, false> {
        public:
            constexpr STEALTH_ALWAYS_INLINE InternalContainer() : mData{} { }

            constexpr STEALTH_ALWAYS_INLINE auto& operator*() noexcept {
                return mData;
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator*() const noexcept {
                return mData;
            }
        private:
            std::array<ScalarType, sizeAtCompileTime> mData;
    };

    template <typename ScalarType, int sizeAtCompileTime>
    class InternalContainer<ScalarType, sizeAtCompileTime, true> {
        using ContainerType = std::array<ScalarType, sizeAtCompileTime>;

        public:
            constexpr STEALTH_ALWAYS_INLINE InternalContainer() : mData{new std::array<ScalarType, sizeAtCompileTime>} { }

            constexpr STEALTH_ALWAYS_INLINE InternalContainer(const InternalContainer& other) {
                mData = std::make_unique<ContainerType>(*other.mData);
            }

            constexpr STEALTH_ALWAYS_INLINE InternalContainer& operator=(const InternalContainer& other) {
                mData = std::make_unique<ContainerType>(*other.mData);
                return *this;
            }

            constexpr STEALTH_ALWAYS_INLINE auto& operator*() noexcept {
                return (*mData);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator*() const noexcept {
                return (*mData);
            }
        private:
            std::unique_ptr<ContainerType> mData;
    };

    template <typename ScalarType, int sizeAtCompileTime>
    class DenseStorage {
        public:
            constexpr STEALTH_ALWAYS_INLINE DenseStorage() { }

            // Move constructor.
            // constexpr STEALTH_ALWAYS_INLINE DenseStorage(DenseStorage&& other) {
            //     mData = move(other.mData);
            // }
            //
            // constexpr STEALTH_ALWAYS_INLINE DenseStorage& operator=(DenseStorage&& other) {
            //     mData = move(other.mData);
            //     return *this;
            // }

            // Copy.
            // constexpr STEALTH_ALWAYS_INLINE DenseStorage(const DenseStorage& other) {
            //     mData = other.mData;
            // }
            //
            // constexpr STEALTH_ALWAYS_INLINE DenseStorage& operator=(const DenseStorage& other) {
            //     mData = other.mData;
            // }

            constexpr STEALTH_ALWAYS_INLINE auto& operator[](int index) {
                return (*mData)[index];
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator[](int index) const {
                return (*mData)[index];
            }

            constexpr STEALTH_ALWAYS_INLINE auto size() const noexcept {
                return sizeAtCompileTime;
            }

            constexpr STEALTH_ALWAYS_INLINE auto begin() const noexcept {
                return (*mData).begin();
            }

            constexpr STEALTH_ALWAYS_INLINE auto end() const noexcept {
                return (*mData).end();
            }

            constexpr STEALTH_ALWAYS_INLINE auto smallOptimizationsEnabled() const noexcept {
                return not requiresHeapAllocation<sizeAtCompileTime>();
            }

        private:
            InternalContainer<ScalarType, sizeAtCompileTime> mData;
    };
} /* Stealth::Tensor::internal */
