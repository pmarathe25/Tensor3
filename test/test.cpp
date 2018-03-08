#include <iostream>
#include "TileMap/TileMap.hpp"

float doDoubleAdd(float a, float b) {
    return a * 2.0f + b;
}

template <typename TileMapType, typename OtherTileMap>
constexpr auto doSum(TileMapType&& first, OtherTileMap&& second) noexcept {
// constexpr STEALTH_ALWAYS_INLINE auto doSum(const TileMapType& first, const OtherTileMap& second) noexcept {
    // first + second will create a temporary BinaryOp.
    // Check if it is destroyed when the function returns.
    // IT IS!!
    return first + second;
}

template <typename... TileMapTypes>
constexpr auto doMultiSum(TileMapTypes&&... tileMaps) {
    return (tileMaps + ...);
}

int testTemporaryExpressionPersistence() {
    StealthTileMap::TileMapF<4, 4> test0;
    test0(0, 0) = 1.0f;
    StealthTileMap::TileMapF<4, 4> test1;
    test1(1, 1) = 2.0f;
    StealthTileMap::TileMapF<4, 4> test2;
    test2(2, 2) = 3.0f;
    StealthTileMap::TileMapF<4, 4> test3;
    test3(3, 3) = 4.0f;
    // First check if the multi sum function works.
    auto testIntSum = doMultiSum(1, 1, 1, 1);
    std::cout << "Int Sum (should be 4): " << testIntSum << '\n';

    StealthTileMap::TileMapF<4, 4> testResult;
    testResult = doMultiSum(test0, test1, test2, test3);
    std::cout << "Result (should NOT segfault): " << testResult << '\n';
    return 0;
}

int testExpressionIndexing() {
    StealthTileMap::TileMapF<4, 4> test0;
    test0(0, 0) = 1.0f;
    StealthTileMap::TileMapF<4, 4> test1;
    test1(1, 1) = 2.0f;
    auto binExpr0 = test0 + test1;
    auto binExpr1 = test0 + 3.14f;
    std::cout << "Expr0 at (0, 0) is: " << binExpr0(0, 0) << '\n';
    std::cout << "Expr1 at (0, 0) is: " << binExpr1(0, 0) << '\n';
    return 0;
}


int main() noexcept {
    bool testFailed = false;
    testFailed += testTemporaryExpressionPersistence();
    testFailed += testExpressionIndexing();
    if (testFailed) {
        std::cout << "One or more tests failed!" << '\n';
    }


    StealthTileMap::TileMapF<5, 5, 2> test{};
    const StealthTileMap::TileMapF<5, 5, 2> test2 = 1.0f;
    StealthTileMap::TileMapF<5, 5, 2> test3 = test + test2;
    StealthTileMap::TileMapF<5, 5, 2> test4 = 1.2f;
    StealthTileMap::TileMapF<2, 2, 2> test5 = 1.7856f;

    test4 += 2.0f;
    std::cout << "Test4\n" << test4 << '\n';

    // By the magic of inlining, this...
    auto testSum = doSum(test2, doSum(test2, doSum(test2, doSum(test2, doSum(test2, doSum(test2, test2))))));
    StealthTileMap::TileMapF<5, 5, 2> sumTileMap2 = testSum;
    // ...is the same as:
    // StealthTileMap::TileMapF<5, 5, 2> sumTileMap2 = test2 +  test2 +  test2 +  test2 +  test2 +  test2 +  test2;
    std::cout << sumTileMap2 << '\n';

    test(0, 0) = 1.5f;
    test(0, 1) = 1.5f;
    test(1, 1) = 1.5f;
    test(1, 2) = 1.5f;
    test(2, 2) = 1.5f;
    test(3, 2) = 1.5f;
    test(0, 0) = 1.5f;
    test(0, 1) = 1.5f;
    test(1, 1) = 1.5f;
    test(0, 0, 1) = 3.1f;
    test(0, 1, 1) = 3.1f;
    test(1, 1, 1) = 3.1f;
    test(3, 3, 1) = 3.1f;
    test.randomize(std::uniform_real_distribution(0.0f, 1.0f));
    auto testRand = StealthTileMap::TileMapF<5, 5, 2>::Random(std::uniform_real_distribution(0.0f, 1.0f));
    test4 = 1.314f + testRand;

    // test2(0, 0, 1) = 3.0f;
    test3 = (test + test2) + test;
    std::cout << test3 << '\n';

    auto testLayer = StealthTileMap::layer(test, 0);
    std::cout << "TileMap View showing a layer of Test\n" << testLayer << '\n';
    std::cout << "Layer of Summation with 1.0 TileMap\n" << StealthTileMap::layer(test + test2, 1) << '\n';
    std::cout << "Layer of empty TileMap\n" << StealthTileMap::layer(test2, 1) << '\n';

    // Changing a value in the view actually changes the TileMap
    StealthTileMap::block<2, 2>(test)(0, 0, 1) = 2.4f;
    // Construct a separate TileMap from the view.
    auto testView = StealthTileMap::block<3, 5, 2>(test, 1, 0);
    auto testView2 = StealthTileMap::block<2, 2, 2>(testView, 1, 1);
    // auto testReshape = StealthTileMap::reshape<2, 9>(testView);

    std::cout << "Test\n" << test << '\n';
    std::cout << "TestView\n" << testView.eval() << '\n';
    std::cout << "TestView2\n" << testView2.eval() << '\n';
    std::cout << "Test5\n" << test5 << '\n';
    std::cout << "TestView2 + Test5\n" << (testView2 + test5) << '\n';


    std::cout << "Test\n" << test << '\n';
    std::cout << "Test2\n" << test2 << '\n';
    std::cout << "Test3\n" << test3 << '\n';
    std::cout << "Test4\n" << test4 << '\n';
    // std::cout << "TestReshape\n" << testReshape << '\n';
    std::cout << "Test and Test2\n" << (test and test2) << '\n';
    std::cout << "Test or Test2\n" << (test or test2) << '\n';
    std::cout << "!Test\n" << !test << '\n';
    std::cout << "Doubled Int Test\n" << StealthTileMap::apply([](float in) -> int {return in * 2;}, test) << '\n';
    std::cout << "Doubled Add Test\n" << StealthTileMap::apply(doDoubleAdd, test, test3) << '\n';


    StealthTileMap::TileMapF<2, 2> moveSame{StealthTileMap::TileMapF<2, 2>{{0, 1, 2, 3}}};
    StealthTileMap::TileMapF<2, 2> moveDiff{StealthTileMap::TileMapF<4>{{4, 5, 6, 7}}};
    std::cout << "Move TileMaps of same size: " << moveSame << '\n';
    std::cout << "Move TileMaps of different size: " << moveDiff << '\n';
    }
