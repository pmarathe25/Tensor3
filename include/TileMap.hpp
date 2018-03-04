#ifndef STEALTH_TILEMAP_H
#define STEALTH_TILEMAP_H
#include "./ForwardDeclarations.hpp"
#include "./TileMapBase.hpp"
#include "./Ops/BinaryOperations.hpp"
#include "./Ops/UnaryOperations.hpp"
#include "./Ops/UnaryOperations.hpp"
#include "./Ops/BlockOperations.hpp"
#include "./OpStructs/TileMapRandomGenerator.hpp"
#include <stealthutil>
#include <vector>
#include <random>



// DEBUG:
#include <iostream>




namespace StealthTileMap {
    namespace internal {
        template <typename type, int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime,
            int areaAtCompileTime, int sizeAtCompileTime>
        struct traits<TileMap<type, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, areaAtCompileTime, sizeAtCompileTime>> {
            typedef type ScalarType;
            static constexpr int width = widthAtCompileTime, length = lengthAtCompileTime, height = heightAtCompileTime,
                area = areaAtCompileTime, size = sizeAtCompileTime;
            typedef std::true_type containsData;
            typedef std::true_type isWritable;
            typedef TileMap<type, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, areaAtCompileTime, sizeAtCompileTime> UnderlyingTileMapType;
        };

        template <typename type, int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime,
            int areaAtCompileTime, int sizeAtCompileTime>
        struct traits<const TileMap<type, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, areaAtCompileTime, sizeAtCompileTime>> {
            typedef type ScalarType;
            static constexpr int width = widthAtCompileTime, length = lengthAtCompileTime, height = heightAtCompileTime,
                area = areaAtCompileTime, size = sizeAtCompileTime;
            typedef std::true_type containsData;
            typedef std::false_type isWritable;
            typedef const TileMap<type, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, areaAtCompileTime, sizeAtCompileTime> UnderlyingTileMapType;
        };
    } /* internal */

    template <typename type, int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime,
        int areaAtCompileTime, int sizeAtCompileTime>
    class TileMap : public TileMapBase<TileMap<type, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, areaAtCompileTime, sizeAtCompileTime>> {
        public:
            typedef type ScalarType;
            static constexpr bool containsData = false;

            constexpr STEALTH_ALWAYS_INLINE TileMap() noexcept : tiles(sizeAtCompileTime) { }

            constexpr STEALTH_ALWAYS_INLINE TileMap(const std::initializer_list<ScalarType>& other) noexcept : tiles(sizeAtCompileTime) {
                if (other.size() > sizeAtCompileTime) {
                    throw std::invalid_argument("Cannot initialize TileMap from incompatible initializer list");
                }
                tiles = std::move(other);
            }

            // Copy
            template <typename OtherTileMap>
            constexpr STEALTH_ALWAYS_INLINE TileMap(const OtherTileMap& other) noexcept : tiles(sizeAtCompileTime) {
                const_copy(other);
            }

            constexpr STEALTH_ALWAYS_INLINE TileMap(const TileMap& other) noexcept {


                // DEBUG:
                std::cout << "\t\t!!!!Doing copy!!!!" << '\n';

                tiles = other.tiles;
            }

            // Move Constructor
            constexpr STEALTH_ALWAYS_INLINE TileMap(TileMap&& other) noexcept = default;

            template <typename OtherType, int width, int length, int height>
            constexpr STEALTH_ALWAYS_INLINE TileMap(TileMap<OtherType, width, length, height>&& other) {
                static_assert(other.size() == TileMap::size(), "Cannot move incompatible TileMaps");
                tiles = std::move(other.elements());
            }

            // Move Assignment
            constexpr STEALTH_ALWAYS_INLINE TileMap& operator=(TileMap&& other) noexcept = default;

            template <typename OtherType, int width, int length, int height>
            constexpr STEALTH_ALWAYS_INLINE TileMap& operator=(TileMap<OtherType, width, length, height>&& other) {
                static_assert(other.size() == TileMap::size(), "Cannot move incompatible TileMaps");
                tiles = std::move(other.elements());
                return *this;
            }

            // Copy Assignment
            constexpr STEALTH_ALWAYS_INLINE TileMap& operator=(const TileMap& other) noexcept {

                // DEBUG:
                std::cout << "\t\t!!!!Doing copy!!!!" << '\n';

                tiles = other.tiles;
                return *this;
            }

            template <typename OtherTileMap>
            constexpr STEALTH_ALWAYS_INLINE TileMap& operator=(const OtherTileMap& other) noexcept {
                const_copy(other);
                return *this;
            }

            // Accessors
            constexpr STEALTH_ALWAYS_INLINE auto& hintedIndex(int hintX, int hintY, int x, int y, int z) {
                return tiles[hintX];
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& hintedIndex(int hintX, int hintY, int x, int y, int z) const {
                return tiles[hintX];
            }

            constexpr STEALTH_ALWAYS_INLINE auto& operator()(int x, int y, int z) {
                return tiles[x + y * TileMap::width() + z * TileMap::area()];
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int x, int y, int z) const {
                return tiles[x + y * TileMap::width() + z * TileMap::area()];
            }

            constexpr STEALTH_ALWAYS_INLINE auto& operator()(int x, int y) {
                return tiles[x + y * TileMap::width()];
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int x, int y) const {
                return tiles[x + y * TileMap::width()];
            }

            constexpr STEALTH_ALWAYS_INLINE auto& operator()(int x) {
                return tiles[x];
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& operator()(int x) const {
                return tiles[x];
            }

            constexpr STEALTH_ALWAYS_INLINE const auto* data() const {
                static_assert(!std::is_same<ScalarType, bool>::value, "Cannot access data() member of boolean TileMap");
                return tiles.data();
            }

            constexpr STEALTH_ALWAYS_INLINE auto* data() {
                static_assert(!std::is_same<ScalarType, bool>::value, "Cannot access data() member of boolean TileMap");
                return tiles.data();
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& elements() const noexcept {
                return tiles;
            }

            constexpr STEALTH_ALWAYS_INLINE auto& elements() noexcept {
                return tiles;
            }

            constexpr STEALTH_ALWAYS_INLINE auto begin() noexcept {
                return tiles.begin();
            }

            constexpr STEALTH_ALWAYS_INLINE auto cbegin() const noexcept {
                return tiles.cbegin();
            }

            constexpr STEALTH_ALWAYS_INLINE auto end() noexcept {
                return tiles.end();
            }

            constexpr STEALTH_ALWAYS_INLINE auto cend() const noexcept {
                return tiles.cend();
            }

            template <typename OtherTileMap>
            constexpr STEALTH_ALWAYS_INLINE void operator+=(OtherTileMap&& other) {
                (*this) = (*this) + std::forward<OtherTileMap&&>(other);
            }

            template <typename OtherTileMap>
            constexpr STEALTH_ALWAYS_INLINE void operator*=(OtherTileMap&& other) {
                (*this) = (*this) * std::forward<OtherTileMap&&>(other);
            }

            template <typename OtherTileMap>
            constexpr STEALTH_ALWAYS_INLINE void operator-=(OtherTileMap&& other) {
                (*this) = (*this) - std::forward<OtherTileMap&&>(other);
            }

            template <typename OtherTileMap>
            constexpr STEALTH_ALWAYS_INLINE void operator/=(OtherTileMap&& other) {
                (*this) = (*this) / std::forward<OtherTileMap&&>(other);
            }

            template <typename Distribution = decltype(DefaultDistribution), typename Generator = decltype(DefaultGenerator)>
            constexpr TileMap& randomize(Distribution&& distribution = std::forward<Distribution&&>(DefaultDistribution),
                long seed = stealth::getCurrentTime(), Generator&& generator = std::forward<Generator&&>(DefaultGenerator)) noexcept {
                generator.seed(seed);
                for (int i = 0; i < TileMap::size(); ++i) {
                    tiles[i] = distribution(generator);
                }
                return (*this);
            }

            constexpr STEALTH_ALWAYS_INLINE TileMap& eval() {
                return (*this);
            }

            constexpr STEALTH_ALWAYS_INLINE const TileMap& eval() const {
                return (*this);
            }

            template <typename Distribution = decltype(DefaultDistribution), typename Generator = decltype(DefaultGenerator)>
            static constexpr STEALTH_ALWAYS_INLINE auto Random(Distribution&& distribution = std::forward<Distribution&&>(DefaultDistribution),
                long seed = stealth::getCurrentTime(), Generator&& generator = std::forward<Generator&&>(DefaultGenerator)) noexcept {
                return TileMapRandomGenerator<TileMap::width(), TileMap::length(), TileMap::height(), Distribution, Generator>
                    {std::forward<Distribution&&>(distribution), seed, std::forward<Generator&&>(generator)};
            }

            constexpr STEALTH_ALWAYS_INLINE auto& underlyingTileMap() {
                return (*this);
            }

            constexpr STEALTH_ALWAYS_INLINE const auto& underlyingTileMap() const {
                return (*this);
            }
        private:
            std::vector<ScalarType> tiles;

            template <typename OtherTileMap>
            constexpr void const_copy(const OtherTileMap& other) {



                // DEBUG:
                std::cout << "\t\t!!!!Doing copy!!!!" << '\n';



                if constexpr (!std::is_scalar<OtherTileMap>::value) static_assert(other.size()
                    == TileMap::size(), "Cannot const_copy incompatible TileMaps");
                int hintX = 0, hintY = 0;
                for (int k = 0; k < TileMap::height(); ++k) {
                    for (int j = 0; j < TileMap::length(); ++j) {
                        for (int i = 0; i < TileMap::width(); ++i) {
                            if constexpr (std::is_scalar<OtherTileMap>::value) tiles[hintX] = other;
                            else tiles[hintX] = other.hintedIndex(hintX, hintY, i, j, k);
                            ++hintX;
                        }
                        ++hintY;
                    }
                }
            }
    };

    template <int width, int length = 1, int height = 1, typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto reshape(LHS&& lhs) {
        typedef typename std::remove_reference<LHS>::type LHSRawType;
        typedef typename internal::traits<LHSRawType>::ScalarType ScalarType;
        static_assert(width * length * height == internal::traits<LHSRawType>::size, "Cannot reshape into incompatible dimensions");
        return TileMap<ScalarType, width, length, height>{std::forward<LHS&&>(lhs)};
    }

    template <int widthAtCompileTime, int lengthAtCompileTime = 1, int heightAtCompileTime = 1>
    using TileMapI = TileMap<int, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime>;

    template <int widthAtCompileTime, int lengthAtCompileTime = 1, int heightAtCompileTime = 1>
    using TileMapF = TileMap<float, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime>;

    template <int widthAtCompileTime, int lengthAtCompileTime = 1, int heightAtCompileTime = 1>
    using TileMapD = TileMap<double, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime>;
} /* StealthTileMap */

#endif
