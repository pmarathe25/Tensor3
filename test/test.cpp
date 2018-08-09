#include "interfaces/Tensor"
#include <iostream>

using namespace Stealth::Tensor;

namespace DenseStorageTest {
    int testInitialize() {
        DenseStorage<float> test(2);
        for (const auto& elem : test)
            std::cout << elem << '\t';
        std::cout << std::endl;
    }
} /* DenseStorageTest */

int testTensorDims() {
    TensorF test{3, 4, 5, 2};
    for (const auto& dim : test.dims())
        std::cout << dim << '\t';
    std::cout << std::endl;
}

int testTensorStrides() {
    TensorF test{3, 4, 5, 2};
    for (const auto& stride : test.strides())
        std::cout << stride << '\t';
    std::cout << std::endl;
}

int testTensorData() {
    TensorF test{3, 4, 5, 2};
    std::cout << test.size() << '\n';
    for (const auto& elem : test)
        std::cout << elem << '\t';
    std::cout << std::endl;
}

int main() {
    DenseStorageTest::testInitialize();
    // testTensorDims();
    // testTensorStrides();
    // testTensorData();
    return 0;
}
