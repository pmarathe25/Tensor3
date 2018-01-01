#include "TileMap/TileMap.hpp"

int main() {
    StealthTileMap::TileMapF<10, 10> test{};

    test(0, 0) = 1.5f;

    display(test);
}
