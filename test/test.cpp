#include "interfaces/Tensor"
#include <iostream>

using namespace Stealth::Tensor;

int testTensorDims() {
    TensorF<4> test{3, 4, 5, 2};
    for (const auto& dim : test.dims())
        std::cout << dim << '\t';
    std::cout << std::endl;
}

int testTensorStrides() {
    TensorF<4> test{3, 4, 5, 2};
    for (const auto& stride : test.strides())
        std::cout << stride << '\t';
    std::cout << std::endl;
}

int testTensorData() {
    TensorF<4> test{3, 4, 5, 2};
    std::cout << test.size() << '\n';
    for (const auto& elem : test)
        std::cout << elem << '\t';
    std::cout << std::endl;
}

int main() {
    testTensorDims();
    testTensorStrides();
    testTensorData();
    return 0;
}
