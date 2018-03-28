#include "Benchmark/Benchmark.hpp"
#include "Tensor3/Tensor3.hpp"
#include <iostream>
#include <algorithm>

constexpr int kTEST_WIDTH = 4;
constexpr int kTEST_LENGTH = 4;
constexpr int kTEST_HEIGHT = 4;
constexpr int kTEST_AREA = kTEST_WIDTH * kTEST_LENGTH;
constexpr int kTEST_SIZE = kTEST_AREA * kTEST_HEIGHT;
constexpr int kPERF_ITERS = 10000;

template <int width = 1, int length = 1, int height = 1>
constexpr auto SequentialTensor3F(int startValue = 0) noexcept {
    Stealth::Tensor3F<width, length, height> out{};
    std::iota(out.begin(), out.end(), startValue);
    return out;
}

struct TestResult {
    TestResult(int errorCode = 0, std::string testName = __builtin_FUNCTION()) : testName{testName}, errorCode{errorCode} { }
    int errorCode;
    std::string testName;
};

template <typename Callable>
constexpr bool runTest(const Callable& test) {
    // Returns true if the test passed, false otherwise.
    auto result = test();
    if (result.errorCode) {
        std::cout << "Test " << result.testName << " failed with error code " << result.errorCode << '\n';
        return false;
    } else {
        std::cout << "Test " << result.testName << " passed." << '\n';
        return true;
    }
}

namespace Block {
    // Choose blocks starting about halfway in the Tensor3.
    constexpr int kBLOCK_X = kTEST_WIDTH / 2, kBLOCK_Y = kTEST_LENGTH / 2, kBLOCK_Z = kTEST_HEIGHT / 2;
    constexpr int kBLOCK_WIDTH = kTEST_WIDTH / 4, kBLOCK_LENGTH = kTEST_LENGTH / 4, kBLOCK_HEIGHT = kTEST_HEIGHT / 4;
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
        auto blockTest0 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH>();
        // Retrieve a block.
        auto blockTest1 = Stealth::block<kBLOCK_WIDTH, kBLOCK_LENGTH>(blockTest0, kBLOCK_X, kBLOCK_Y);
        // Retrieve a block from that block
        auto blockTest2 = Stealth::block<kSUBBLOCK_WIDTH, kSUBBLOCK_LENGTH>(blockTest1, kSUBBLOCK_X, kSUBBLOCK_Y);
        // Check whether every digit is consecutive row-by-row and that all the rows start on the correct value.
        int numIncorrect = 0;
        for (int y = 0; y < blockTest2.length(); ++y) {
            numIncorrect += blockTest2(0, y) != (kSUBBLOCK_START2D + kTEST_WIDTH * y);
            for (int x = 1; x < blockTest2.width(); ++x) {
                numIncorrect += blockTest2(x, y) != blockTest2(x - 1, y) + 1;
            }
        }
        return TestResult{numIncorrect};
    }

    TestResult testConstBlock() {
        // Test whether we can grab a block from a const Tensor3.
        const auto blockTest0 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH>();
        // Retrieve a block.
        auto blockTest1 = Stealth::block<kBLOCK_WIDTH, kBLOCK_LENGTH>(blockTest0, kBLOCK_X, kBLOCK_Y);
        // Check whether every digit is consecutive row-by-row and that all the rows start on the correct value.
        int numIncorrect = 0;
        for (int y = 0; y < blockTest1.length(); ++y) {
            numIncorrect += blockTest1(0, y) != (kBLOCK_START2D + kTEST_WIDTH * y);
            for (int x = 1; x < blockTest1.width(); ++x) {
                numIncorrect += blockTest1(x, y) != blockTest1(x - 1, y) + 1;
            }
        }
        return TestResult{numIncorrect};
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
        auto blockTest1 = Stealth::block<kBLOCK_WIDTH, kBLOCK_LENGTH>(blockTest0, kBLOCK_X, kBLOCK_Y);
        // Check whether every digit is consecutive row-by-row and that all the rows start on the correct value.
        int numIncorrect = 0;
        for (int y = 0; y < blockTest1.length(); ++y) {
            numIncorrect += blockTest1(0, y) != (kBLOCK_START2D + kTEST_WIDTH * y);
            for (int x = 1; x < blockTest1.width(); ++x) {
                numIncorrect += blockTest1(x, y) != blockTest1(x - 1, y) + 1;
            }
        }
        return TestResult{numIncorrect};
    }

    TestResult test2DBlockFrom3D() {
        auto blockTest0 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        // Retrieve a block.
        auto blockTest1 = Stealth::block<kBLOCK_WIDTH, kBLOCK_LENGTH>(blockTest0, kBLOCK_X, kBLOCK_Y, kBLOCK_Z);
        // Check whether every digit is consecutive row-by-row and that all the rows start on the correct value.
        int numIncorrect = 0;
        for (int y = 0; y < blockTest1.length(); ++y) {
            numIncorrect += blockTest1(0, y) != (kBLOCK_START3D + kTEST_WIDTH * y);
            for (int x = 1; x < blockTest1.width(); ++x) {
                numIncorrect += blockTest1(x, y) != blockTest1(x - 1, y) + 1;
            }
        }
        return TestResult{numIncorrect};
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
        Stealth::Benchmark bench;
        auto perfTest0 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        Stealth::VectorF<kTEST_SIZE> result;
        bench.start();
        for (int i = 0; i < kPERF_ITERS; ++i) {
            result = perfTest0;
        }
        bench.finish();
        std::cout << bench.info() << '\n';
        return TestResult{};
    }

    TestResult testLargeSum() {
        Stealth::Benchmark bench;
        auto perfTest0 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto perfTest1 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto perfTest2 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto perfTest3 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto perfTest4 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        Stealth::Tensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT> result;
        bench.start();
        for (int i = 0; i < kPERF_ITERS; ++i) {
            result = perfTest0 + perfTest1 + perfTest2 + perfTest3 + perfTest4;
        }
        bench.finish();
        std::cout << bench.info() << '\n';
        return TestResult{};
    }

    // Choose blocks starting about halfway in the Tensor3.
    constexpr int kBLOCK_X = kTEST_WIDTH / 2, kBLOCK_Y = kTEST_LENGTH / 2, kBLOCK_Z = kTEST_HEIGHT / 2;
    constexpr int kBLOCK_WIDTH = kTEST_WIDTH / 4, kBLOCK_LENGTH = kTEST_LENGTH / 4, kBLOCK_HEIGHT = kTEST_HEIGHT / 4;

    TestResult testBlockSum() {
        Stealth::Benchmark bench;

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
        auto block0 = Stealth::block<kBLOCK_WIDTH, kBLOCK_LENGTH, kBLOCK_HEIGHT>(perfTest0 + perfTest1
            + perfTest2 + perfTest3 + perfTest4, kBLOCK_X, kBLOCK_Y, kBLOCK_Z);
        // This should use 1D accesses.
        auto block1 = Stealth::block<kBLOCK_WIDTH, kBLOCK_LENGTH, kBLOCK_HEIGHT>(perfTest5 + perfTest6
            + perfTest7 + perfTest8 + perfTest9);

        Stealth::Tensor3F<kBLOCK_WIDTH, kBLOCK_LENGTH, kBLOCK_HEIGHT> result;
        bench.start();
        for (int i = 0; i < kPERF_ITERS; ++i) {
            result = block0 + block1;
        }
        bench.finish();
        std::cout << bench.info() << '\n';
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
        Stealth::Tensor3F<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT> result = binaryTest0 + binaryTest1;
        int numIncorrect = 0;
        for (int i = 0; i < result.size(); ++i) {
            numIncorrect += result(i) != i * 2;
        }
        return TestResult{numIncorrect};
    }

    TestResult test1DBroadcastOver2DSum() {
        auto binaryTest0 = SequentialTensor3F<kTEST_WIDTH, kTEST_LENGTH>();
        auto binaryTest1 = SequentialTensor3F<kTEST_WIDTH, 1>();
        Stealth::MatrixF<kTEST_WIDTH, kTEST_LENGTH> result = binaryTest0 + binaryTest1;
        int numIncorrect = 0;
        for (int k = 0; k < result.height(); ++k) {
            for (int j = 0; j < result.length(); ++j) {
                for (int i = 0; i < result.width(); ++i) {
                    numIncorrect += result(i, j, k) != (binaryTest0(i, j, k) + i);
                }
            }
        }
        return TestResult{numIncorrect};
    }
} /* Binary */

bool testBinary() {
    bool allTestsPassed = true;
    allTestsPassed &= runTest(Binary::testSum);
    allTestsPassed &= runTest(Binary::test1DBroadcastOver2DSum);
    return allTestsPassed;
}

int main() {
    bool allTestsPassed = true;
    allTestsPassed &= testBlockOps();
    allTestsPassed &= testPerf();
    allTestsPassed &= testBinary();
    // numFailed += testUnaryOps();
    // numFailed += testTemporaryExpressionPersistence();
    // numFailed += testExpressionIndexing();
    if (allTestsPassed) {
        std::cout << "All tests passed!" << '\n';
        return 0;
    } else {
        return 1;
    }
}
