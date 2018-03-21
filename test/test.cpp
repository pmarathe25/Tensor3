#include "Benchmark/Benchmark.hpp"
#include "TileMap/TileMap.hpp"
#include <iostream>
#include <algorithm>

constexpr int kTEST_WIDTH = 25;
constexpr int kTEST_LENGTH = 25;
constexpr int kTEST_HEIGHT = 25;
constexpr int kTEST_AREA = kTEST_WIDTH * kTEST_LENGTH;
constexpr int kTEST_SIZE = kTEST_AREA * kTEST_HEIGHT;

template <int width = 1, int length = 1, int height = 1>
constexpr auto SequentialTileMapF(int startValue = 0) noexcept {
    Stealth::TileMapF<width, length, height> out{};
    std::iota(out.begin(), out.end(), startValue);
    return out;
}

template <typename Callable>
constexpr bool runTest(const Callable& test) {
    // Returns true if the test passed, false otherwise.
    auto result = test();
    if (result.errorCode) {
        std::cout << "Test " << result.testName << " failed with error code " << result.errorCode << '\n';
        return false;
    }
    return true;
}

struct TestResult {
    TestResult(int errorCode, std::string testName = __builtin_FUNCTION()) : testName{testName}, errorCode{errorCode} { }
    int errorCode;
    std::string testName;
};

// int testBinaryOps() {
//
// }

namespace Block {
    // Choose blocks starting about halfway in the TileMap.
    constexpr int kBLOCK_X = kTEST_WIDTH / 2, kBLOCK_Y = kTEST_LENGTH / 2, kBLOCK_Z = kTEST_HEIGHT / 2;
    constexpr int kBLOCK_WIDTH = kTEST_WIDTH / 4, kBLOCK_LENGTH = kTEST_LENGTH / 4, kBLOCK_HEIGHT = kTEST_HEIGHT / 4;
    // In a sequential TileMap, what should the value be at this location?
    constexpr int kSTART_VALUE1D = kBLOCK_X;
    constexpr int kSTART_VALUE2D = kBLOCK_X + kBLOCK_Y * kTEST_WIDTH;
    constexpr int kSTART_VALUE3D = kBLOCK_X + kBLOCK_Y * kTEST_WIDTH + kBLOCK_Z * kTEST_AREA;

    TestResult testReshape() {
        // First generate a TileMap with sequential values.
        auto reshapeTest0 = SequentialTileMapF<kTEST_WIDTH, kTEST_LENGTH>();
        // Try to reshape.
        auto reshapeTest1 = SequentialTileMapF<kTEST_AREA>();
        // Make sure that it worked.
        auto checkReshape0 = (Stealth::reshape<kTEST_AREA>(reshapeTest0) == reshapeTest1).eval();
        int error = not std::all_of(checkReshape0.cbegin(), checkReshape0.cend(), [](bool in) { return in; });
        return TestResult{error};
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
            numIncorrect += blockTest1(0, y) != (kSTART_VALUE2D + kTEST_WIDTH * y);
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
            numIncorrect += blockTest1(0, y) != (kSTART_VALUE3D + kTEST_WIDTH * y);
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
    allTestsPassed &= runTest(Block::testReshape);

    allTestsPassed &= runTest(Block::test2DBlockFrom2D);
    allTestsPassed &= runTest(Block::test2DBlockFrom3D);
    return allTestsPassed;
}

//
// int testUnaryOps() {
//     // First generate a TileMap with sequential values.
//     auto unaryTest0 = SequentialTileMapF<4, 4>();
//     std::cout << unaryTest0 << '\n';
// }
//
// float doDoubleAdd(float a, float b) {
//     return a * 2.0f + b;
// }
//
// template <typename TileMapType, typename OtherTileMap>
// constexpr auto doSum(TileMapType&& first, OtherTileMap&& second) noexcept {
// // constexpr STEALTH_ALWAYS_INLINE auto doSum(const TileMapType& first, const OtherTileMap& second) noexcept {
//     // first + second will create a temporary ElemWiseBinaryExpr.
//     // Check if it is destroyed when the function returns.
//     // IT IS!!
//     return first + second;
// }
//
// template <typename... TileMapTypes>
// constexpr auto doMultiSum(TileMapTypes&&... tileMaps) {
//     return (tileMaps + ...);
// }
//
// int testTemporaryExpressionPersistence() {
//     Stealth::TileMapF<4, 4> test0;
//     test0(0, 0) = 1.0f;
//     Stealth::TileMapF<4, 4> test1;
//     test1(1, 1) = 2.0f;
//     Stealth::TileMapF<4, 4> test2;
//     test2(2, 2) = 3.0f;
//     Stealth::TileMapF<4, 4> test3;
//     test3(3, 3) = 4.0f;
//     // First check if the multi sum function works.
//     auto testIntSum = doMultiSum(1, 1, 1, 1);
//     std::cout << "Int Sum (should be 4): " << testIntSum << '\n';
//
//     Stealth::TileMapF<4, 4> testResult;
//     testResult = doMultiSum(test0, test1, test2, test3);
//     std::cout << "Result (should NOT segfault): " << testResult << '\n';
//     return 0;
// }
//
// int testExpressionIndexing() {
//     Stealth::TileMapF<4, 4> test0;
//     test0(0, 0) = 1.0f;
//     Stealth::TileMapF<4, 4> test1;
//     test1(1, 1) = 2.0f;
//     auto binExpr0 = test0 + test1;
//     auto binExpr1 = test0 + 3.14f;
//     std::cout << "Expr0 at (0, 0) is: " << binExpr0(0, 0) << '\n';
//     std::cout << "Expr1 at (0, 0) is: " << binExpr1(0, 0) << '\n';
//     return 0;
// }


int main() {
    bool allTestsPassed = true;
    // Block op tests
    allTestsPassed &= testBlockOps();
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
