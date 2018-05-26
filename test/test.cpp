#include <Stealth/Tensor3>
// #include <Stealth/Benchmark>
#include <Stealth/util>
#include <iostream>
#include <algorithm>

constexpr int kTEST_WIDTH = 30;
constexpr int kTEST_LENGTH = 30;
constexpr int kTEST_HEIGHT = 30;
constexpr int kTEST_AREA = kTEST_WIDTH * kTEST_LENGTH;
constexpr int kTEST_SIZE = kTEST_AREA * kTEST_HEIGHT;
constexpr int kPERF_ITERS = 10000;

template <int width = 1, int length = 1, int height = 1>
constexpr auto SequentialTensor3F(int startValue = 0) noexcept {
    Stealth::Tensor::Tensor3F<width, length, height> out{};
    // std::iota(out.begin(), out.end(), startValue);
    for (int i = 0; i < out.size(); ++i) {
        out(i) = i;
    }
    return out;
}

struct TestResult {
    TestResult(bool passed = true, std::string errorMessage = {}, std::string testName = __builtin_FUNCTION())
        : passed{passed}, testName{testName}, errorMessage{errorMessage} { }
    bool passed;
    std::string errorMessage;
    std::string testName;
};

template <typename Callable>
constexpr bool runTest(const Callable& test) {
    // Returns true if the test passed, false otherwise.
    // auto executionInfo = Stealth::Benchmark::measureExecutionTime(test);
    // if (!executionInfo.returnValue.passed) {
    //     std::cout << "FAILED Test " << executionInfo.returnValue.testName << " with: " << executionInfo.returnValue.errorMessage << '\n';
    //     return false;
    // } else {
    //     std::cout << "PASSED Test " << executionInfo.returnValue.testName << " in " << executionInfo.microseconds() << " μs.\n";
    //     return true;
    // }
    // FIXME:
    TestResult result = test();
    if (result.passed) {
        std::cout << "PASSED Test " << result.testName << '\n';
    } else {
        std::cout << "FAILED Test " << result.testName << " with: "
            << result.errorMessage << '\n';
    }
    return result.passed;
}

namespace Block {
    // Choose blocks starting about halfway in the Tensor3.
    constexpr int kBLOCK_X = kTEST_WIDTH / 2, kBLOCK_Y = kTEST_LENGTH / 2, kBLOCK_Z = kTEST_HEIGHT / 2;
    constexpr int kBLOCK_WIDTH = kTEST_WIDTH / 2, kBLOCK_LENGTH = kTEST_LENGTH / 2, kBLOCK_HEIGHT = kTEST_HEIGHT / 2;
    // Choose subblocks starting about halfway in the other blocks.
    constexpr int kSUBBLOCK_X = kBLOCK_WIDTH / 2, kSUBBLOCK_Y = kBLOCK_LENGTH / 2, kSUBBLOCK_Z = kBLOCK_HEIGHT / 2;
    constexpr int kSUBBLOCK_WIDTH = kBLOCK_WIDTH / 2, kSUBBLOCK_LENGTH = kBLOCK_LENGTH / 2, kSUBBLOCK_HEIGHT = kBLOCK_HEIGHT / 2;
    // In a sequential Tensor3, what should the value be at this location?
    constexpr int kBLOCK_START1D = kBLOCK_X;
    constexpr int kBLOCK_START2D = kBLOCK_X + kBLOCK_Y * kTEST_WIDTH;
    constexpr int kBLOCK_START3D = kBLOCK_X + kBLOCK_Y * kTEST_WIDTH + kBLOCK_Z * kTEST_AREA;
    constexpr int kSUBBLOCK_START1D = (kBLOCK_X + kSUBBLOCK_X);
    constexpr int kSUBBLOCK_START2D = (kBLOCK_X + kSUBBLOCK_X) + (kBLOCK_Y + kSUBBLOCK_Y) * kTEST_WIDTH;
    constexpr int kSUBBLOCK_START3D = (kBLOCK_X + kSUBBLOCK_X) + (kBLOCK_Y + kSUBBLOCK_Y) * kTEST_WIDTH + (kBLOCK_Z + kSUBBLOCK_Z) * kTEST_AREA;

    TestResult testBlockception() {
        // Test whether a block of a block works as expected.
        auto blockTest0 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        // Retrieve a block.
        auto blockTest1 = Stealth::Tensor::block<kBLOCK_WIDTH, kBLOCK_LENGTH>(blockTest0, kBLOCK_X, kBLOCK_Y);
        // Retrieve a block from that block
        auto blockTest2 = Stealth::Tensor::block<kSUBBLOCK_WIDTH, kSUBBLOCK_LENGTH>(blockTest1, kSUBBLOCK_X, kSUBBLOCK_Y);
        // Check whether every digit is consecutive row-by-row and that all the rows start on the correct value.
        int numIncorrect = 0;
        for (int y = 0; y < blockTest2.length(); ++y) {
            numIncorrect += blockTest2(0, y) != (kSUBBLOCK_START2D + kTEST_WIDTH * y);
            for (int x = 1; x < blockTest2.width(); ++x) {
                numIncorrect += blockTest2(x, y) != (blockTest2(x - 1, y) + 1);
            }
        }
        return TestResult{!numIncorrect, std::to_string(numIncorrect) + " values incorrect."};
    }

    TestResult testConstBlock() {
        // Test whether we can grab a block from a const Tensor3.
        const auto blockTest0 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH>();
        // Retrieve a block.
        auto blockTest1 = Stealth::Tensor::block<kBLOCK_WIDTH, kBLOCK_LENGTH>(blockTest0, kBLOCK_X, kBLOCK_Y);
        // Check whether every digit is consecutive row-by-row and that all the rows start on the correct value.
        int numIncorrect = 0;
        for (int y = 0; y < blockTest1.length(); ++y) {
            numIncorrect += blockTest1(0, y) != (kBLOCK_START2D + kTEST_WIDTH * y);
            for (int x = 1; x < blockTest1.width(); ++x) {
                numIncorrect += blockTest1(x, y) != blockTest1(x - 1, y) + 1;
            }
        }
        return TestResult{!numIncorrect, std::to_string(numIncorrect) + " values incorrect."};
    }

    TestResult test1DBlockFrom1D() {

    }

    TestResult test1DBlockFrom2D() {

    }

    TestResult test1DBlockFrom3D() {

    }

    TestResult test2DBlockFrom2D() {
        auto blockTest0 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH>();
        // Retrieve a block.
        auto blockTest1 = Stealth::Tensor::block<kBLOCK_WIDTH, kBLOCK_LENGTH>(blockTest0, kBLOCK_X, kBLOCK_Y);
        // Check whether every digit is consecutive row-by-row and that all the rows start on the correct value.
        int numIncorrect = 0;
        for (int y = 0; y < blockTest1.length(); ++y) {
            numIncorrect += blockTest1(0, y) != (kBLOCK_START2D + kTEST_WIDTH * y);
            for (int x = 1; x < blockTest1.width(); ++x) {
                numIncorrect += blockTest1(x, y) != blockTest1(x - 1, y) + 1;
            }
        }
        return TestResult{!numIncorrect, std::to_string(numIncorrect) + " values incorrect."};
    }

    TestResult test2DBlockFrom3D() {
        auto blockTest0 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        // Retrieve a block.
        auto blockTest1 = Stealth::Tensor::block<kBLOCK_WIDTH, kBLOCK_LENGTH>(blockTest0, kBLOCK_X, kBLOCK_Y, kBLOCK_Z);
        // Check whether every digit is consecutive row-by-row and that all the rows start on the correct value.
        int numIncorrect = 0;
        for (int y = 0; y < blockTest1.length(); ++y) {
            numIncorrect += blockTest1(0, y) != (kBLOCK_START3D + kTEST_WIDTH * y);
            for (int x = 1; x < blockTest1.width(); ++x) {
                numIncorrect += blockTest1(x, y) != blockTest1(x - 1, y) + 1;
            }
        }
        return TestResult{!numIncorrect, std::to_string(numIncorrect) + " values incorrect."};
    }

    int test3DBlockFrom3D() {

    }
} /* Block */

bool testBlockOps() {
    bool allTestsPassed = true;
    allTestsPassed &= runTest(Block::testBlockception);
    allTestsPassed &= runTest(Block::testConstBlock);
    allTestsPassed &= runTest(Block::test2DBlockFrom2D);
    allTestsPassed &= runTest(Block::test2DBlockFrom3D);
    return allTestsPassed;
}

namespace Perf {
    TestResult testCopy() {
        auto perfTest0 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        Stealth::Tensor::VectorF<kTEST_SIZE> result;
        for (int i = 0; i < kPERF_ITERS; ++i) {
            result = perfTest0;
        }
        return TestResult{};
    }

    TestResult testLargeSum() {
        auto perfTest0 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto perfTest1 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto perfTest2 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto perfTest3 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto perfTest4 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        Stealth::Tensor::Tensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT> result;
        for (int i = 0; i < kPERF_ITERS; ++i) {
            result = perfTest0 + perfTest1 + perfTest2 + perfTest3 + perfTest4;
        }
        return TestResult{};
    }

    // Choose blocks starting about halfway in the Tensor3.
    constexpr int kBLOCK_X = kTEST_WIDTH / 2, kBLOCK_Y = kTEST_LENGTH / 2, kBLOCK_Z = kTEST_HEIGHT / 2;
    constexpr int kBLOCK_WIDTH = kTEST_WIDTH / 2, kBLOCK_LENGTH = kTEST_LENGTH / 2, kBLOCK_HEIGHT = kTEST_HEIGHT / 2;

    TestResult testBlockSum() {

        auto perfTest0 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto perfTest1 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto perfTest2 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto perfTest3 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto perfTest4 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();

        auto perfTest5 = SequentialTensor3F<kBLOCK_WIDTH, kBLOCK_LENGTH, kBLOCK_HEIGHT>();
        auto perfTest6 = SequentialTensor3F<kBLOCK_WIDTH, kBLOCK_LENGTH, kBLOCK_HEIGHT>();
        auto perfTest7 = SequentialTensor3F<kBLOCK_WIDTH, kBLOCK_LENGTH, kBLOCK_HEIGHT>();
        auto perfTest8 = SequentialTensor3F<kBLOCK_WIDTH, kBLOCK_LENGTH, kBLOCK_HEIGHT>();
        auto perfTest9 = SequentialTensor3F<kBLOCK_WIDTH, kBLOCK_LENGTH, kBLOCK_HEIGHT>();
        // This should force 3D accesses, since the block is in the center of a 3D Tensor3.
        auto block0 = Stealth::Tensor::block<kBLOCK_WIDTH, kBLOCK_LENGTH, kBLOCK_HEIGHT>(perfTest0 + perfTest1
            + perfTest2 + perfTest3 + perfTest4, kBLOCK_X, kBLOCK_Y, kBLOCK_Z);
        // This should use 1D accesses.
        auto block1 = Stealth::Tensor::block<kBLOCK_WIDTH, kBLOCK_LENGTH, kBLOCK_HEIGHT>(perfTest5 + perfTest6
            + perfTest7 + perfTest8 + perfTest9);

        Stealth::Tensor::Tensor3F<kBLOCK_WIDTH, kBLOCK_LENGTH, kBLOCK_HEIGHT> result;
        for (int i = 0; i < kPERF_ITERS; ++i) {
            result = block0 + block1;
        }
        return TestResult{};
    }

} /* Perf */

bool testPerf() {
    bool allTestsPassed = true;
    allTestsPassed &= runTest(Perf::testCopy);
    allTestsPassed &= runTest(Perf::testLargeSum);
    allTestsPassed &= runTest(Perf::testBlockSum);
    return allTestsPassed;
}

namespace Binary {
    TestResult testSum() {
        auto binaryTest0 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto binaryTest1 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        Stealth::Tensor::Tensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT> result = binaryTest0 + binaryTest1;
        int numIncorrect = 0;
        for (int i = 0; i < result.size(); ++i) {
            numIncorrect += result(i) != i * 2;
        }
        return TestResult{!numIncorrect, std::to_string(numIncorrect) + " values incorrect."};
    }

    TestResult test1DBroadcastOver2DSum() {
        auto binaryTest0 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH>();
        auto binaryTest1 = SequentialTensor3F<kTEST_WIDTH, 1>();
        Stealth::Tensor::MatrixF<kTEST_WIDTH, kTEST_LENGTH> result = binaryTest0 + binaryTest1;
        int numIncorrect = 0;
        for (int k = 0; k < result.height(); ++k) {
            for (int j = 0; j < result.length(); ++j) {
                for (int i = 0; i < result.width(); ++i) {
                    numIncorrect += result(i, j, k) != (binaryTest0(i, j, k) + i);
                }
            }
        }
        return TestResult{!numIncorrect, std::to_string(numIncorrect) + " values incorrect."};
    }

    TestResult testScalarMultiply() {
        auto binaryTest0 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH>();
        constexpr float kSCALAR_CONST = 2.0f;
        Stealth::Tensor::MatrixF<kTEST_WIDTH, kTEST_LENGTH> result = binaryTest0 * kSCALAR_CONST;
        int numIncorrect = 0;
        for (int k = 0; k < result.height(); ++k) {
            for (int j = 0; j < result.length(); ++j) {
                for (int i = 0; i < result.width(); ++i) {
                    numIncorrect += result(i, j, k) != (binaryTest0(i, j, k) * kSCALAR_CONST);
                }
            }
        }
        return TestResult{!numIncorrect, std::to_string(numIncorrect) + " values incorrect."};
    }
} /* Binary */

bool testBinary() {
    bool allTestsPassed = true;
    allTestsPassed &= runTest(Binary::testSum);
    allTestsPassed &= runTest(Binary::test1DBroadcastOver2DSum);
    allTestsPassed &= runTest(Binary::testScalarMultiply);
    return allTestsPassed;
}

namespace Storage {
    TestResult testDenseStorageSmall() {
        auto storageTest0 = Stealth::Tensor::internal::DenseStorage<float, 16>{};
        return TestResult{storageTest0.smallOptimizationsEnabled(), "Small storage optimizations were not enabled."};
    }

    TestResult testDenseStorageLarge() {
        auto storageTest0 = Stealth::Tensor::internal::DenseStorage<float, kTEST_SIZE / 2>{};
        return TestResult{!storageTest0.smallOptimizationsEnabled(), "Small storage optimizations were incorrectly enabled."};
    }


    TestResult testInitializerListAssignment() {
        auto storageTest0 = Stealth::Tensor::VectorI<5>{};
        storageTest0 = {0.f, 1.f, 2.f, 3.f, 4.f};
        int numIncorrect = 0;
        float expected = 0.f;
        for (auto& elem : storageTest0) {
            numIncorrect += (elem != expected++);
        }
        return TestResult{!numIncorrect, std::to_string(numIncorrect) + " values incorrect."};
    }
}

bool testStorage() {
    bool allTestsPassed = true;
    allTestsPassed &= runTest(Storage::testDenseStorageSmall);
    allTestsPassed &= runTest(Storage::testDenseStorageLarge);
    allTestsPassed &= runTest(Storage::testInitializerListAssignment);
    return allTestsPassed;
}

int main() {
    bool allTestsPassed = true;
    allTestsPassed &= testBlockOps();
    allTestsPassed &= testPerf();
    allTestsPassed &= testBinary();
    allTestsPassed &= testStorage();
    if (allTestsPassed) {
        std::cout << "All tests passed!" << '\n';
        return 0;
    } else {
        std::cout << "One or more tests failed!" << '\n';
        return 1;
    }
}
