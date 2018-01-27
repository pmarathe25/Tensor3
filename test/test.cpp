#include "TileMap/TileMap.hpp"
#include <iostream>

float doDoubleAdd(float a, float b) {
    return a * 2.0f + b;
}

template <typename TileMapType, typename OtherTileMap>
constexpr auto doSum(const TileMapType& first, const OtherTileMap& second) noexcept {
// constexpr STEALTH_ALWAYS_INLINE auto doSum(const TileMapType& first, const OtherTileMap& second) noexcept {
    // first + second will create a temporary BinaryOp.
    // Check if it is destroyed when the function returns.
    // IT IS!!
    return first + second;
}

int main() noexcept {
    StealthTileMap::TileMapF<5, 5, 2> test{};
    const StealthTileMap::TileMapF<5, 5, 2> test2 = 1.0f;
    StealthTileMap::TileMapF<5, 5, 2> test3 = test + test2;
    StealthTileMap::TileMapF<5, 5, 2> test4 = 1.2f;
    StealthTileMap::TileMapF<2, 2, 2> test5 = 1.7856f;

    test4 += 2.0f;
    std::cout << "Test4\n" << test4 << '\n';

    // By the magic of inlining, this...
    StealthTileMap::TileMapF<5, 5, 2> sumTileMap2 = doSum(test2, doSum(test2, doSum(test2, doSum(test2, doSum(test2, doSum(test2, test2))))));
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
    test4 = StealthTileMap::TileMapF<5, 5, 2>::Random(std::uniform_real_distribution(0.0f, 1.0f));

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
    auto testView = StealthTileMap::block<3, 3>(test, 1, 1);
    auto testView2 = StealthTileMap::block<2, 2>(testView, 1, 1);
    auto testReshape = StealthTileMap::reshape<2, 9>(testView);

    std::cout << "Test\n" << test << '\n';
    std::cout << "TestView\n" << testView.eval() << '\n';
    std::cout << "TestView2\n" << testView2.eval() << '\n';
    std::cout << "Test5\n" << test5 << '\n';
    std::cout << "TestView2 + Test5\n" << (testView2 + test5) << '\n';


    // std::cout << "Test\n" << test << '\n';
    // std::cout << "Test2\n" << test2 << '\n';
    // std::cout << "Test3\n" << test3 << '\n';
    // std::cout << "Test4\n" << test4 << '\n';
    // std::cout << "TestReshape\n" << testReshape << '\n';
    // std::cout << "Test && Test2\n" << (test && test2) << '\n';
    // std::cout << "Test || Test2\n" << (test || test2) << '\n';
    // std::cout << "!Test\n" << !test << '\n';
    // std::cout << "Doubled Int Test\n" << StealthTileMap::apply([](float in) -> int {return in * 2;}, test) << '\n';
    // std::cout << "Doubled Add Test\n" << StealthTileMap::apply(doDoubleAdd, test, test3) << '\n';


    // StealthTileMap::TileMapF<2, 2> moveSame{StealthTileMap::TileMapF<2, 2>{{0, 1, 2, 3}}};
    // StealthTileMap::TileMapF<2, 2> moveDiff{StealthTileMap::TileMapF<4>{{4, 5, 6, 7}}};
    // std::cout << "Move TileMaps of same size: " << moveSame << '\n';
    // std::cout << "Move TileMaps of different size: " << moveDiff << '\n';
    //
    // // Creating TileMap from array
    // std::array<float, 4> init{{0, 1, 2, 3}};
    // StealthTileMap::TileMapF<2, 2> testInit{init};
    // std::cout << "Array Initialization Test\n" << testInit << '\n';
}
