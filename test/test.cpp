#include "TileMap/TileMap.hpp"

int main() {
    StealthTileMap::TileMapF<5, 5, 2> test{};
    StealthTileMap::TileMapF<5, 5, 2> test2{};

    test(0, 0) = 1.5f;
    test(0, 1) = 1.5f;
    test(1, 1) = 1.5f;
    test(0, 0, 1) = 3.1f;
    test(0, 1, 1) = 3.1f;
    test(1, 1, 1) = 3.1f;
    test2(0, 0, 1) = 3.0f;

    auto test3 = (test + test2).eval();

    // StealthTileMap::display(test3);
    // std::cout << test.height() << '\n';
    // StealthTileMap::display(test);

    // auto testLayer = StealthTileMap::layer(test, 0);
    // StealthTileMap::display(testLayer);
    // StealthTileMap::display(StealthTileMap::layer(test + test2, 1));


    // testView(0, 0, 1) = 2.4f;
    auto testView = StealthTileMap::block<2, 2>(test).eval();

    std::cout << "Test at 0, 0, 1: " << test(0, 0, 1) << '\n';
    std::cout << "View at 0, 0, 1: " << StealthTileMap::block<2, 2>(test)(0, 0, 1) << '\n';
    std::cout << "TestView at 0, 0, 1: " << testView(0, 0, 1) << '\n';
    // std::cout << "TestView at 0, 0, 1: " << testView(0, 0, 1) << '\n';

    StealthTileMap::display(test, "Original");
    StealthTileMap::display(StealthTileMap::block<2, 2>(test), "2x2 View");
    StealthTileMap::display(testView, "TestView");
    // StealthTileMap::display(StealthTileMap::block<2, 2, 1>(test, 1, 1, 0));
}
