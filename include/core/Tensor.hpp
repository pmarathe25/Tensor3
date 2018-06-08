#include "DenseStorage.hpp"
#include <initializer_list>

namespace Stealth::Tensor {
    template <typename Scalar, size_t rank>
    class Tensor {
        using RankArray = DenseStorage<size_t>;
        public:
            Tensor(const std::initializer_list<size_t>& dims) {
                mDims = RankArray{dims.size()};
                mStrides = RankArray{dims.size()};
                int index = 0;
                for (auto elem : dims)
                    mDims[index++] = elem;
                mStrides = computeStrides(mDims);
                mData = std::move(DenseStorage<Scalar>{mStrides.front() * mDims[0]});
            }

            constexpr size_t size() const { return mData.size(); }
            constexpr const auto begin() const { return mData.begin(); }
            constexpr const auto end() const { return mData.end(); }

            constexpr const RankArray& dims() const { return mDims; }
            constexpr const RankArray& strides() const { return mStrides; }
        private:
            constexpr RankArray computeStrides(const RankArray& dims) {
                RankArray strides{dims.size()};
                strides.back() = 1;
                for (int index = dims.size() - 1; index > 0; --index) {
                    strides[index - 1] = dims[index] * strides[index];
                }
                return strides;
            }

            DenseStorage<Scalar> mData;
            RankArray mDims{rank};
            RankArray mStrides{rank};
    };

    template <int rank>
    using TensorF = Tensor<float, rank>;
} /* Stealth::Tensor */
