#include "TileMap/TileMap.hpp"
#include <iostream>

float doDoubleAdd(float a, float b) {
    return a * 2.0f + b;
}

int main() {
    StealthTileMap::TileMapF<5, 5, 2> test{};
    const StealthTileMap::TileMapF<5, 5, 2> test2{};
    StealthTileMap::TileMapF<5, 5, 2> test3 = test + test2;

    test(0, 0) = 1.5f;
    test(0, 1) = 1.5f;
    test(1, 1) = 1.5f;
    test(0, 0, 1) = 3.1f;
    test(0, 1, 1) = 3.1f;
    test(1, 1, 1) = 3.1f;

    // test2(0, 0, 1) = 3.0f;
    test3 = (test + test2) + test;
    std::cout << test3 << '\n';

    auto testLayer = StealthTileMap::layer(test, 0);
    std::cout << "TileMap View showing a layer\n" << testLayer << '\n';
    std::cout << "Layer of Summation with empty TileMap\n" << StealthTileMap::layer(test + test2, 1) << '\n';
    std::cout << "Layer of empty TileMap\n" << StealthTileMap::layer(test2, 1) << '\n';

    // Changing a value in the view actually changes the TileMap
    StealthTileMap::block<2, 2>(test)(0, 0, 1) = 2.4f;
    // Construct a separate TileMap from the view.
    auto testView = StealthTileMap::block<2, 2>(test, 0, 0);
    auto testReshape = StealthTileMap::reshape<2, 4>(testView);

    std::cout << "TestView at 1, 1: " << testView(1, 1) << '\n';
    std::cout << "TestView\n" << testView << '\n';

    std::cout << "Test\n" << test << '\n';
    std::cout << "Test2\n" << test2 << '\n';
    std::cout << "Test3\n" << test3 << '\n';
    std::cout << "TestReshape\n" << testReshape << '\n';
    std::cout << "Test && Test2\n" << (test && test2) << '\n';
    std::cout << "Test || Test2\n" << (test || test2) << '\n';
    std::cout << "!Test\n" << !test << '\n';
    std::cout << "Doubled Int Test\n" << StealthTileMap::apply([](float in) -> int {return in * 2;}, test) << '\n';
    std::cout << "Doubled Add Test\n" << StealthTileMap::apply(doDoubleAdd, test, test3) << '\n';

    // Creating TileMap from array
    std::array<float, 4> init{{0, 1, 2, 3}};
    StealthTileMap::TileMapF<2, 2> testInit{init};
    std::cout << "Array Initialization Test\n" << testInit << '\n';
}
