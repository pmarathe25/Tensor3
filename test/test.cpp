#include "Benchmark/Benchmark.hpp"
#include "TileMap/TileMap.hpp"
#include <iostream>
#include <algorithm>

// If you set this too small it will cause segfaults for some reason.
constexpr int kTEST_WIDTH = 30;
constexpr int kTEST_LENGTH = 30;
constexpr int kTEST_HEIGHT = 30;
constexpr int kTEST_AREA = kTEST_WIDTH * kTEST_LENGTH;
constexpr int kTEST_SIZE = kTEST_AREA * kTEST_HEIGHT;
constexpr int kPERF_ITERS = 10000;

template <int width = 1, int length = 1, int height = 1>
constexpr auto SequentialTileMapF(int startValue = 0) noexcept {
    Stealth::TileMapF<width, length, height> out{};
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
    std::cout << "Running test" << "..." << '\n';
    auto result = test();
    if (result.errorCode) {
        std::cout << "Test " << result.testName << " failed with error code " << result.errorCode << '\n';
        return false;
    } else {
        std::cout << "Test " << result.testName << " passed." << '\n';
        return true;
    }
}

// TestResult testReshape() {
//     // First generate a TileMap with sequential values.
//     auto reshapeTest0 = SequentialTileMapF<kTEST_WIDTH, kTEST_LENGTH>();
//     // Try to reshape.
//     auto reshapeTest1 = SequentialTileMapF<kTEST_AREA>();
//     // Make sure that it worked.
//     auto checkReshape0 = (Stealth::reshape<kTEST_AREA>(reshapeTest0) == reshapeTest1).eval();
//     int error = not std::all_of(checkReshape0.cbegin(), checkReshape0.cend(), [](bool in) { return in; });
//     return TestResult{error};
// }

namespace Block {
    // Choose blocks starting about halfway in the TileMap.
    constexpr int kBLOCK_X = kTEST_WIDTH / 2, kBLOCK_Y = kTEST_LENGTH / 2, kBLOCK_Z = kTEST_HEIGHT / 2;
    constexpr int kBLOCK_WIDTH = kTEST_WIDTH / 4, kBLOCK_LENGTH = kTEST_LENGTH / 4, kBLOCK_HEIGHT = kTEST_HEIGHT / 4;
    // Choose subblocks starting about halfway in the other blocks.
    constexpr int kSUBBLOCK_X = kBLOCK_WIDTH / 2, kSUBBLOCK_Y = kBLOCK_LENGTH / 2, kSUBBLOCK_Z = kBLOCK_HEIGHT / 2;
    constexpr int kSUBBLOCK_WIDTH = kBLOCK_WIDTH / 2, kSUBBLOCK_LENGTH = kBLOCK_LENGTH / 2, kSUBBLOCK_HEIGHT = kBLOCK_HEIGHT / 2;
    // In a sequential TileMap, what should the value be at this location?
    constexpr int kBLOCK_START1D = kBLOCK_X;
    constexpr int kBLOCK_START2D = kBLOCK_X + kBLOCK_Y * kTEST_WIDTH;
    constexpr int kBLOCK_START3D = kBLOCK_X + kBLOCK_Y * kTEST_WIDTH + kBLOCK_Z * kTEST_AREA;
    constexpr int kSUBBLOCK_START1D = (kBLOCK_X + kSUBBLOCK_X);
    constexpr int kSUBBLOCK_START2D = (kBLOCK_X + kSUBBLOCK_X) + (kBLOCK_Y + kSUBBLOCK_Y) * kTEST_WIDTH;
    constexpr int kSUBBLOCK_START3D = (kBLOCK_X + kSUBBLOCK_X) + (kBLOCK_Y + kSUBBLOCK_Y) * kTEST_WIDTH + (kBLOCK_Z + kSUBBLOCK_Z) * kTEST_AREA;

    TestResult testBlockception() {
        // Test whether a block of a block works as expected.
        auto blockTest0 = SequentialTileMapF<kTEST_WIDTH, kTEST_LENGTH>();
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
        // Test whether we can grab a block from a const TileMap.
        const auto blockTest0 = SequentialTileMapF<kTEST_WIDTH, kTEST_LENGTH>();
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
        auto blockTest0 = SequentialTileMapF<kTEST_WIDTH, kTEST_LENGTH>();
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
        auto blockTest0 = SequentialTileMapF<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
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
        auto perfTest0 = SequentialTileMapF<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        Stealth::TileMapF<kTEST_SIZE> result;
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
        auto perfTest0 = SequentialTileMapF<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto perfTest1 = SequentialTileMapF<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto perfTest2 = SequentialTileMapF<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto perfTest3 = SequentialTileMapF<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto perfTest4 = SequentialTileMapF<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        Stealth::TileMapF<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT> result;
        bench.start();
        for (int i = 0; i < kPERF_ITERS; ++i) {
            result = perfTest0 + perfTest1 + perfTest2 + perfTest3 + perfTest4;
        }
        bench.finish();
        std::cout << bench.info() << '\n';
        return TestResult{};
    }

    // Choose blocks starting about halfway in the TileMap.
    constexpr int kBLOCK_X = kTEST_WIDTH / 2, kBLOCK_Y = kTEST_LENGTH / 2, kBLOCK_Z = kTEST_HEIGHT / 2;
    constexpr int kBLOCK_WIDTH = kTEST_WIDTH / 4, kBLOCK_LENGTH = kTEST_LENGTH / 4, kBLOCK_HEIGHT = kTEST_HEIGHT / 4;

    TestResult testBlockSum() {
        Stealth::Benchmark bench;

        auto perfTest0 = SequentialTileMapF<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto perfTest1 = SequentialTileMapF<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto perfTest2 = SequentialTileMapF<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto perfTest3 = SequentialTileMapF<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto perfTest4 = SequentialTileMapF<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();

        auto perfTest5 = SequentialTileMapF<kBLOCK_WIDTH, kBLOCK_LENGTH, kBLOCK_HEIGHT>();
        auto perfTest6 = SequentialTileMapF<kBLOCK_WIDTH, kBLOCK_LENGTH, kBLOCK_HEIGHT>();
        auto perfTest7 = SequentialTileMapF<kBLOCK_WIDTH, kBLOCK_LENGTH, kBLOCK_HEIGHT>();
        auto perfTest8 = SequentialTileMapF<kBLOCK_WIDTH, kBLOCK_LENGTH, kBLOCK_HEIGHT>();
        auto perfTest9 = SequentialTileMapF<kBLOCK_WIDTH, kBLOCK_LENGTH, kBLOCK_HEIGHT>();
        // This should force 3D accesses, since the block is in the center of a 3D TileMap.
        auto block0 = Stealth::block<kBLOCK_WIDTH, kBLOCK_LENGTH, kBLOCK_HEIGHT>(perfTest0 + perfTest1
            + perfTest2 + perfTest3 + perfTest4, kBLOCK_X, kBLOCK_Y, kBLOCK_Z);
        // This should use 1D accesses.
        auto block1 = Stealth::block<kBLOCK_WIDTH, kBLOCK_LENGTH, kBLOCK_HEIGHT>(perfTest5 + perfTest6
            + perfTest7 + perfTest8 + perfTest9);

        Stealth::TileMapF<kBLOCK_WIDTH, kBLOCK_LENGTH, kBLOCK_HEIGHT> result;
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
        auto binaryTest0 = SequentialTileMapF<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        auto binaryTest1 = SequentialTileMapF<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT>();
        Stealth::TileMapF<kTEST_WIDTH, kTEST_LENGTH, kTEST_HEIGHT> result = binaryTest0 + binaryTest1;
        int numIncorrect = 0;
        for (int i = 0; i < result.size(); ++i) {
            numIncorrect += result(i) != i * 2;
        }
        return TestResult{numIncorrect};
    }

    TestResult test1DBroadcastOver2DSum() {
        auto binaryTest0 = SequentialTileMapF<kTEST_WIDTH, kTEST_LENGTH>();
        auto binaryTest1 = SequentialTileMapF<kTEST_WIDTH, 1>();
        Stealth::TileMapF<kTEST_WIDTH, kTEST_LENGTH> result = binaryTest0 + binaryTest1;
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
    // Block op tests
    allTestsPassed &= testBlockOps();
    allTestsPassed &= testPerf();
    allTestsPassed &= testBinary();
    // numFailed += testUnaryOps();
    // numFailed += testTemporaryExpressionPersistence();
    // numFailed += testExpressionIndexing();
    if (allTestsPassed) {
        std::cout << "All tests passed!" << '\n';
    }
    //
    //
    // Stealth::TileMapF<5, 5, 2> test{};
    // const Stealth::TileMapF<5, 5, 2> test2 = 1.0f;
    // Stealth::TileMapF<5, 5, 2> test3 = test + test2;
    // Stealth::TileMapF<5, 5, 2> test4 = 1.2f;
    // Stealth::TileMapF<2, 2, 2> test5 = 1.7856f;
    //
    // test4 += 2.0f;
    // std::cout << "Test4\n" << test4 << '\n';
    //
    // // By the magic of inlining, this...
    // auto testSum = doSum(test2, doSum(test2, doSum(test2, doSum(test2, doSum(test2, doSum(test2, test2))))));
    // Stealth::TileMapF<5, 5, 2> sumTileMap2 = testSum;
    // // ...is the same as:
    // // Stealth::TileMapF<5, 5, 2> sumTileMap2 = test2 +  test2 +  test2 +  test2 +  test2 +  test2 +  test2;
    // std::cout << sumTileMap2 << '\n';
    //
    // test(0, 0) = 1.5f;
    // test(0, 1) = 1.5f;
    // test(1, 1) = 1.5f;
    // test(1, 2) = 1.5f;
    // test(2, 2) = 1.5f;
    // test(3, 2) = 1.5f;
    // test(0, 0) = 1.5f;
    // test(0, 1) = 1.5f;
    // test(1, 1) = 1.5f;
    // test(0, 0, 1) = 3.1f;
    // test(0, 1, 1) = 3.1f;
    // test(1, 1, 1) = 3.1f;
    // test(3, 3, 1) = 3.1f;
    // test.randomize(std::uniform_real_distribution(0.0f, 1.0f));
    // auto testRand = Stealth::TileMapF<5, 5, 2>::Random(std::uniform_real_distribution(0.0f, 1.0f));
    // test4 = 1.314f + testRand;
    //
    // // test2(0, 0, 1) = 3.0f;
    // test3 = (test + test2) + test;
    // std::cout << test3 << '\n';
    //
    // auto testLayer = Stealth::layer(test, 0);
    // std::cout << "TileMap View showing a layer of Test\n" << testLayer << '\n';
    // std::cout << "Layer of Summation with 1.0 TileMap\n" << Stealth::layer(test + test2, 1) << '\n';
    // std::cout << "Layer of empty TileMap\n" << Stealth::layer(test2, 1) << '\n';
    //
    // // Changing a value in the view actually changes the TileMap
    // Stealth::block<2, 2>(test)(0, 0, 1) = 2.4f;
    // // Construct a separate TileMap from the view.
    // auto testView = Stealth::block<3, 5, 2>(test, 1, 0);
    // auto testView2 = Stealth::block<2, 2, 2>(testView, 1, 1);
    // // auto testReshape = Stealth::reshape<2, 9>(testView);
    //
    // std::cout << "Test\n" << test << '\n';
    // std::cout << "TestView\n" << testView.eval() << '\n';
    // std::cout << "TestView2\n" << testView2.eval() << '\n';
    // std::cout << "Test5\n" << test5 << '\n';
    // std::cout << "TestView2 + Test5\n" << (testView2 + test5) << '\n';
    //
    //
    // std::cout << "Test\n" << test << '\n';
    // std::cout << "Test2\n" << test2 << '\n';
    // std::cout << "Test3\n" << test3 << '\n';
    // std::cout << "Test4\n" << test4 << '\n';
    // // std::cout << "TestReshape\n" << testReshape << '\n';
    // std::cout << "Test and Test2\n" << (test and test2) << '\n';
    // std::cout << "Test or Test2\n" << (test or test2) << '\n';
    // std::cout << "!Test\n" << !test << '\n';
    // std::cout << "Doubled Int Test\n" << Stealth::apply([](float in) -> int {return in * 2;}, test) << '\n';
    // std::cout << "Doubled Add Test\n" << Stealth::apply(doDoubleAdd, test, test3) << '\n';
    //
    //
    // Stealth::TileMapF<2, 2> moveSame{Stealth::TileMapF<2, 2>{{0, 1, 2, 3}}};
    // Stealth::TileMapF<2, 2> moveDiff{Stealth::TileMapF<4>{{4, 5, 6, 7}}};
    // std::cout << "Move TileMaps of same size: " << moveSame << '\n';
    // std::cout << "Move TileMaps of different size: " << moveDiff << '\n';
}
