#include <algorithm>
#include <initializer_list>
#include <array>
#include <vector>

namespace Stealth::Tensor {
    static constexpr int kDYNAMIC = -1;

    // Used when size is known at compile time.
    template <typename Scalar, int size = kDYNAMIC>
    class DenseStorage : public std::array<Scalar, size> {};

    template <typename Scalar>
    class allocator : public std::allocator<Scalar> {
        using super = std::allocator<Scalar>;
        public:
            typename super::pointer allocate(typename super::size_type size, typename super::const_pointer hint = 0) {
                return static_cast<typename super::pointer>(new Scalar[size]);
            }

            void deallocate(typename super::pointer p, typename super::size_type size) {
                delete[] p;
            }

            // void construct(typename super::pointer p, const T& val) {
            //     new(static_cast<void*>(p)) T(val);
            // }
            //
            // void construct(typename super::pointer p) {
            //     new(static_cast<void*>(p)) T();
            // }
            //
            // void destroy(typename super::pointer p) {
            //     p->~T();
            // }

    };

    // template <typename Scalar>
    // class DenseStorage<Scalar, kDYNAMIC> : public std::vector<Scalar, allocator<Scalar>> {};
    template <typename Scalar>
    class DenseStorage<Scalar, kDYNAMIC> : public std::vector<Scalar> {
        // Inherit constructors.
        using super = std::vector<Scalar>;
        using super::super;
    };

    // Specialization for dynamically allocated storage.
    // template <typename Scalar>
    // class DenseStorage<Scalar, kDYNAMIC> {
    //     public:
    //         class const_iterator : public std::iterator<std::random_access_iterator_tag, Scalar, long, const Scalar*, Scalar> {
    //             public:
    //                 const_iterator(const Scalar* data, size_t index = 0) : mData{data}, mIndex{index} {}
    //                 const_iterator& operator++() { ++mIndex; return *this; }
    //                 // const_iterator operator++(int) { const_iterator prev = *this; ++(*this); return prev; }
    //                 bool operator==(const const_iterator& other) const { return mIndex == other.mIndex; }
    //                 bool operator!=(const const_iterator& other) const {return !(*this == other);}
    //                 const Scalar& operator*() const { return mData[mIndex]; }
    //                 size_t mIndex;
    //             private:
    //                 const Scalar* mData;
    //         };
    //
    //         constexpr DenseStorage(size_t size = 0) { allocate(size); }
    //
    //         constexpr DenseStorage(DenseStorage&& other) { move(std::forward<DenseStorage&&>(other)); }
    //
    //         constexpr DenseStorage& operator=(DenseStorage&& other) {
    //             if (mData) delete[] mData;
    //             move(std::forward<DenseStorage&&>(other));
    //             return *this;
    //         }
    //
    //         // Managed access.
    //         constexpr const_iterator begin() const { return const_iterator{mData, 0}; }
    //         constexpr const_iterator end() const { return const_iterator{mData, mSize}; }
    //         constexpr Scalar& front() { return mData[0]; }
    //         constexpr Scalar& back() { return mData[size() - 1]; }
    //         // Raw access.
    //         constexpr const Scalar& operator[](size_t index) const { return mData[index]; }
    //         constexpr Scalar& operator[](size_t index) { return mData[index]; }
    //         constexpr const Scalar* data() const noexcept { return mData; }
    //         constexpr Scalar* data() noexcept { return mData; }
    //         // Metadata
    //         constexpr size_t size() const noexcept { return mSize; }
    //         constexpr void resize(size_t size) {
    //             if (size != mSize) {
    //                 if (mData) delete[] mData;
    //                 allocate(size);
    //             }
    //         }
    //
    //         ~DenseStorage() { if (mData) delete[] mData; }
    //     private:
    //         void allocate(size_t size) {
    //             mSize = size;
    //             mData = new Scalar[size];
    //         }
    //
    //         void move(DenseStorage&& other) {
    //             mData = other.mData;
    //             mSize = other.mSize;
    //             other.mData = nullptr;
    //         }
    //
    //         Scalar* mData{nullptr};
    //         size_t mSize{0};
    // };


} /* Stealth::Tensor */
