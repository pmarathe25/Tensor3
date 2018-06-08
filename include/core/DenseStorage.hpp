#include <algorithm>
#include <initializer_list>

namespace Stealth::Tensor {
    template <typename Scalar>
    class DenseStorage {
        public:
            class const_iterator : public std::iterator<std::random_access_iterator_tag, Scalar, long, const Scalar*, Scalar> {
                public:
                    const_iterator(const Scalar* data, size_t index = 0) : mData{data}, mIndex{index} {}
                    const_iterator& operator++() { ++mIndex; return *this; }
                    // const_iterator operator++(int) { const_iterator prev = *this; ++(*this); return prev; }
                    bool operator==(const const_iterator& other) const { return mIndex == other.mIndex; }
                    bool operator!=(const const_iterator& other) const {return !(*this == other);}
                    const Scalar& operator*() const { return mData[mIndex]; }
                    size_t mIndex;
                private:
                    const Scalar* mData;
            };

            constexpr DenseStorage(size_t size = 0) {
                allocate(size);
            }

            constexpr DenseStorage(DenseStorage&& other) {
                move(std::forward<DenseStorage&&>(other));
            }

            constexpr DenseStorage& operator=(DenseStorage&& other) {
                if (mData) delete[] mData;
                move(std::forward<DenseStorage&&>(other));
                return *this;
            }

            constexpr const_iterator begin() const {
                return const_iterator{mData, 0};
            }

            constexpr const_iterator end() const {
                return const_iterator{mData, mSize};
            }

            constexpr Scalar& front() {
                return mData[0];
            }

            constexpr Scalar& back() {
                return mData[size() - 1];
            }

            constexpr const Scalar& operator[](size_t index) const {
                return mData[index];
            }

            constexpr Scalar& operator[](size_t index) {
                return mData[index];
            }

            constexpr size_t size() const noexcept { return mSize; }

            ~DenseStorage() {
                if (mData) delete[] mData;
            }

        private:
            void allocate(size_t size) {
                mSize = size;
                mData = new Scalar[size];
            }

            void move(DenseStorage&& other) {
                mData = other.mData;
                mSize = other.mSize;
                other.mData = nullptr;
            }

            Scalar* mData{nullptr};
            size_t mSize{0};
    };
} /* Stealth::Tensor */
