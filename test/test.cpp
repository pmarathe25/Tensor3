#include "TileMap/TileMap.hpp"

int main() {
    StealthTileMap::TileMapF<5, 5, 2> test{};
    StealthTileMap::TileMapF<5, 5, 2> test2{};

    test(0, 0, 0) = 1.5f;
    test(0, 0, 1) = 3.0f;

    auto test3 = (test + test2).eval();

    display(test3);
    // std::cout << test.height() << '\n';
    // display(test);
}
