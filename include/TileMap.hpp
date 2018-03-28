#pragma once
#include "./ForwardDeclarations.hpp"
#include "./TileMapBase.hpp"
#include "./Operations/BinaryOperations.hpp"
#include "./Operations/UnaryOperations.hpp"
#include "./Operations/UnaryOperations.hpp"
#include "./Operations/BlockOperations.hpp"
#include "./Expressions/TileMapRandomGenerator.hpp"
#include <stealthutil>
#include <vector>
#include <random>

#ifdef DEBUG
    #include <iostream>
#endif
// #include <iostream>

namespace Stealth {
    namespace internal {
        template <typename type, int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime,
            int areaAtCompileTime, int sizeAtCompileTime>
        struct traits<TileMap<type, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime, areaAtCompileTime, sizeAtCompileTime>> {
            using ScalarType = type;
            static constexpr int width = widthAtCompileTime,
                length = lengthAtCompileTime,
                height = heightAtCompileTime,
                area = areaAtCompileTime,
                size = sizeAtCompileTime,
                indexingMode = 1;
            static constexpr bool is_scalar = size == 1;
            static constexpr bool is_vector = !is_scalar and (width == size or length == size or height == size);
            static constexpr bool is_matrix = !is_vector and (width == 1 or length == 1 or height == 1);
        };
    } /* internal */

    template <typename ScalarType, int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime,
        int areaAtCompileTime, int sizeAtCompileTime>
    class TileMap : public TileMapBase<TileMap<ScalarType, widthAtCompileTime, lengthAtCompileTime,
        heightAtCompileTime, areaAtCompileTime, sizeAtCompileTime>> {
        public:
            constexpr STEALTH_ALWAYS_INLINE TileMap() noexcept : tiles(sizeAtCompileTime) { }

            constexpr STEALTH_ALWAYS_INLINE TileMap(const std::initializer_list<ScalarType>& other) noexcept : tiles(sizeAtCompileTime) {
                if (other.size() > sizeAtCompileTime) {
                    throw std::invalid_argument("Cannot initialize TileMap from incompatible initializer list");
                }
                tiles = std::move(other);
            }

            // Copy
            template <typename OtherTileMap>
            constexpr STEALTH_ALWAYS_INLINE TileMap(OtherTileMap&& other) noexcept : tiles(sizeAtCompileTime) {
                copy(std::forward<OtherTileMap&&>(other));
            }

            constexpr STEALTH_ALWAYS_INLINE TileMap(const TileMap& other) noexcept : tiles(sizeAtCompileTime) {


                #ifdef DEBUG
                    std::cout << "\t\t!!!!Doing copy!!!!" << '\n';
                #endif

                copy(other);
            }

            // Move Constructor
            constexpr STEALTH_ALWAYS_INLINE TileMap(TileMap&& other) noexcept = default;

            template <int width, int length, int height>
            constexpr STEALTH_ALWAYS_INLINE TileMap(TileMap<ScalarType, width, length, height>&& other) {
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

                #ifdef DEBUG
                    std::cout << "\t\t!!!!Doing copy!!!!" << '\n';
                #endif

                copy(other);
                return *this;
            }

            template <typename OtherTileMap>
            constexpr STEALTH_ALWAYS_INLINE TileMap& operator=(OtherTileMap&& other) noexcept {
                copy(std::forward<OtherTileMap&&>(other));
                return *this;
            }

            // Accessors
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
        private:
            std::vector<ScalarType> tiles;

            template <typename OtherTileMap>
            constexpr void copy(OtherTileMap&& other) {
                static_assert(other.size() == TileMap::size(), "Cannot copy incompatible TileMaps.");

                constexpr int indexingModeToUse = std::max(internal::traits<TileMap>::indexingMode,
                    internal::traits<raw_type<OtherTileMap>>::indexingMode);

                #ifdef DEBUG
                    std::cout << "\t\t!!!!Doing copy using indexing mode: " << indexingModeToUse << '\n';
                #endif

                if constexpr (indexingModeToUse == 1) {
                    // Treat it as a 1D array
                    #pragma omp parallel for simd
                    for (int i = 0; i < other.size(); ++i) {
                        (*this)(i) = other(i);
                    }
                } else if constexpr (indexingModeToUse == 2) {
                    // Treat it as a long 2D array.
                    #pragma omp parallel for simd
                    for (int j = 0; j < other.length() * other.height(); ++j) {
                        for (int i = 0; i < other.width(); ++i) {
                            (*this)(i + j * other.width()) = other(i, j);
                        }
                    }
                } else {
                    // Copy as 3D array.
                    #pragma omp parallel for simd
                    for (int z = 0; z < other.height(); ++z) {
                        for (int j = 0; j < other.length(); ++j) {
                            for (int i = 0; i < other.width(); ++i) {
                                (*this)(i + j * other.width() + z * other.area()) = other(i, j, z);
                            }
                        }
                    }
                }

            }
    };

    template <int widthAtCompileTime = 1, int lengthAtCompileTime = 1, int heightAtCompileTime = 1>
    using TileMapI = TileMap<int, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime>;

    template <int widthAtCompileTime = 1, int lengthAtCompileTime = 1, int heightAtCompileTime = 1>
    using TileMapF = TileMap<float, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime>;

    template <int widthAtCompileTime = 1, int lengthAtCompileTime = 1, int heightAtCompileTime = 1>
    using TileMapD = TileMap<double, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime>;
} /* Stealth */
