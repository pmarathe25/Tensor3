#ifndef STEALTH_TILEMAP_H
#define STEALTH_TILEMAP_H
#include "./ForwardDeclarations.hpp"
#include "./TileMapBase.hpp"
#include "./Ops/BinaryOperations.hpp"
#include "./Ops/UnaryOperations.hpp"
#include "./Ops/UnaryOperations.hpp"
#include "./Ops/TileMapViewOperations.hpp"
#include <stealthutil>
#include <vector>

namespace StealthTileMap {
    constexpr int NUM_THREADS = 8;

    namespace internal {
        template <typename type, int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime,
            int areaAtCompileTime, int sizeAtCompileTime>
        struct traits<TileMap<type, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, areaAtCompileTime, sizeAtCompileTime>> {
            typedef type ScalarType;
            static constexpr int width = widthAtCompileTime, length = lengthAtCompileTime, height = heightAtCompileTime,
                area = areaAtCompileTime, size = sizeAtCompileTime;
            typedef std::true_type containsData;
            typedef std::true_type isWritable;
        };

        template <typename type, int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime,
            int areaAtCompileTime, int sizeAtCompileTime>
        struct traits<const TileMap<type, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, areaAtCompileTime, sizeAtCompileTime>> {
            typedef type ScalarType;
            static constexpr int width = widthAtCompileTime, length = lengthAtCompileTime, height = heightAtCompileTime,
                area = areaAtCompileTime, size = sizeAtCompileTime;
            typedef std::true_type containsData;
            typedef std::false_type isWritable;
        };
    } /* internal */

    template <typename type, int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime,
        int areaAtCompileTime, int sizeAtCompileTime>
    class TileMap : public TileMapBase<TileMap<type, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, areaAtCompileTime, sizeAtCompileTime>> {
        public:
            typedef type ScalarType;
            static constexpr bool containsData = false;

            constexpr TileMap() noexcept : tiles(sizeAtCompileTime) { }

            constexpr TileMap(const std::initializer_list<ScalarType>& other) noexcept : tiles(sizeAtCompileTime) {
                if (other.size() > sizeAtCompileTime) {
                    throw std::invalid_argument("Cannot initialize TileMap from incompatible initializer list");
                }
                tiles = std::move(other);
            }

            // Copy
            template <typename OtherTileMap>
            constexpr TileMap(const OtherTileMap& other) noexcept : tiles(sizeAtCompileTime) {
                static_assert(other.size() == this -> size(), "Cannot copy incompatible TileMaps");
                copy(other);
            }

            constexpr TileMap(const TileMap& other) noexcept {
                tiles = other.tiles;
            }

            // Move Constructor
            constexpr TileMap(TileMap&& other) noexcept = default;

            template <typename OtherType, int width, int length, int height>
            constexpr TileMap(TileMap<OtherType, width, length, height>&& other) {
                static_assert(other.size() == this -> size(), "Cannot copy incompatible TileMaps");
                tiles = std::move(other.elements());
            }

            // Move Assignment
            constexpr TileMap& operator=(TileMap&& other) noexcept = default;

            template <typename OtherType, int width, int length, int height>
            constexpr TileMap& operator=(TileMap<OtherType, width, length, height>&& other) {
                static_assert(other.size() == this -> size(), "Cannot copy incompatible TileMaps");
                tiles = std::move(other.elements());
                return *this;
            }

            // Copy Assignment
            constexpr TileMap& operator=(const TileMap& other) noexcept {
                tiles = other.tiles;
                return *this;
            }

            template <typename OtherTileMap>
            constexpr TileMap& operator=(const OtherTileMap& other) noexcept {
                static_assert(other.size() == this -> size(), "Cannot copy incompatible TileMaps");
                copy(other);
                return *this;
            }

            // Accessors
            constexpr auto& operator()(int x, int y, int z) {
                return tiles[this -> area() * z + this -> width() * y + x];
            }

            constexpr const auto& operator()(int x, int y, int z) const {
                return tiles[this -> area() * z + this -> width() * y + x];
            }

            constexpr auto& operator()(int x, int y) {
                return tiles[this -> width() * y + x];
            }

            constexpr const auto& operator()(int x, int y) const {
                return tiles[this -> width() * y + x];
            }

            constexpr auto& operator()(int x) {
                return tiles[x];
            }

            constexpr const auto& operator()(int x) const {
                return tiles[x];
            }

            constexpr const auto& operator[](int x) const {
                return tiles[x];
            }

            constexpr auto& operator[](int x) {
                return tiles[x];
            }

            constexpr const auto* data() const {
                static_assert(!std::is_same<ScalarType, bool>::value, "Cannot access data() member of boolean TileMap");
                return tiles.data();
            }

            constexpr auto* data() {
                static_assert(!std::is_same<ScalarType, bool>::value, "Cannot access data() member of boolean TileMap");
                return tiles.data();
            }

            constexpr const auto& elements() const noexcept {
                return tiles;
            }

            constexpr auto& elements() noexcept {
                return tiles;
            }

            constexpr auto begin() noexcept {
                return tiles.begin();
            }

            constexpr auto cbegin() const noexcept {
                return tiles.cbegin();
            }

            constexpr auto end() noexcept {
                return tiles.end();
            }

            constexpr auto cend() const noexcept {
                return tiles.cend();
            }

            constexpr TileMap& eval() {
                return (*this);
            }
        private:
            std::vector<ScalarType> tiles;

            template <typename OtherTileMap>
            constexpr void copy(const OtherTileMap& other) {
                for (int i = 0; i < this -> size(); ++i) {
                    tiles[i] = other[i];
                }
            }
    };

    template <int width, int length = 1, int height = 1, typename LHS>
    constexpr auto reshape(LHS&& lhs) {
        typedef typename std::remove_reference<LHS>::type LHSRawType;
        typedef typename internal::traits<LHSRawType>::ScalarType ScalarType;
        static_assert(width * length * height == internal::traits<LHSRawType>::size, "Cannot reshape into incompatible dimensions");
        return TileMap<ScalarType, width, length, height>{std::forward<LHS&&>(lhs)};
    }

    template <int widthAtCompileTime, int lengthAtCompileTime = 1, int heightAtCompileTime = 1>
    using TileMapF = TileMap<float, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime>;

    template <int widthAtCompileTime, int lengthAtCompileTime = 1, int heightAtCompileTime = 1>
    using TileMapD = TileMap<double, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime>;
} /* StealthTileMap */

#endif
