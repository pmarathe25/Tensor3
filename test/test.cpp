#include "Benchmark/Benchmark.hpp"
#include "TileMap/TileMap.hpp"
#include <iostream>
#include <algorithm>

constexpr static int kTEST_WIDTH = 25;
constexpr static int kTEST_HEIGHT = 25;
constexpr static int kTEST_SIZE = kTEST_WIDTH * kTEST_HEIGHT;


template <int width = 1, int length = 1, int height = 1>
constexpr auto SequentialTileMapF() noexcept {
    Stealth::TileMapF<width, length, height> out{};
    std::iota(out.begin(), out.end(), 0);
    return out;
}

// int testBinaryOps() {
//
// }

namespace Block {
    int testReshape() {
        // First generate a TileMap with sequential values.
        auto reshapeTest0 = SequentialTileMapF<kTEST_WIDTH, kTEST_HEIGHT>();
        // Try to reshape.
        auto reshapeTest1 = SequentialTileMapF<kTEST_SIZE>();
        // Make sure that it worked.
        auto checkReshape0 = (Stealth::reshape<kTEST_SIZE>(reshapeTest0) == reshapeTest1).eval();
        if (std::all_of(checkReshape0.cbegin(), checkReshape0.cend(), [](bool in) { return in; })) {
            // Test passed, exit with no error code.
            return 0;
        } else {
            return 1;
        }
    }

    int testBlockOps() {
        int numFailed = 0;
        numFailed += testReshape();
        return numFailed;
    }
} /* Block */

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
    int numFailed = 0;
    // Block op tests
    numFailed += Block::testBlockOps();
    // numFailed += testUnaryOps();
    // numFailed += testTemporaryExpressionPersistence();
    // numFailed += testExpressionIndexing();
    if (numFailed) {
        std::cout << "One or more tests failed!" << '\n';
    } else {
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
