#include "DenseStorage.hpp"
#include <initializer_list>

namespace Stealth::Tensor {
    // Static Tensor - dimensions are know at compile time.
    template <typename Scalar, int... dims>
    class Tensor {

    };

    // Specialization for Dynamic Tensors.
    template <typename Scalar>
    class Tensor<Scalar> {
        using RankArray = DenseStorage<size_t>;
        public:
            Tensor(const std::initializer_list<size_t>& dims) {
                setDims(dims);
                calculateStrides();
                mData.resize(mStrides.front() * mDims[0]);
            }

            constexpr size_t size() const { return mData.size(); }
            constexpr const auto begin() const { return mData.begin(); }
            constexpr const auto end() const { return mData.end(); }

            constexpr const RankArray& dims() const { return mDims; }
            constexpr const RankArray& rank() const { return mDims.size(); }
            constexpr const RankArray& strides() const { return mStrides; }
        private:
            constexpr RankArray setDims(const std::initializer_list<size_t>& dims) {
                // Set dims.
                mDims.resize(dims.size());
                int index = 0;
                for (auto elem : dims)
                mDims[index++] = elem;
            }

            constexpr RankArray calculateStrides() {
                // Compute strides.
                mStrides.resize(mDims.size());
                mStrides.back() = 1;
                for (int index = mDims.size() - 1; index > 0; --index) {
                    mStrides[index - 1] = mDims[index] * mStrides[index];
                }
            }

            DenseStorage<Scalar> mData;
            RankArray mDims;
            RankArray mStrides;
    };

    template <int... dims>
    using StaticTensorF = Tensor<float, dims...>;
    using TensorF = Tensor<float>;

} /* Stealth::Tensor */
