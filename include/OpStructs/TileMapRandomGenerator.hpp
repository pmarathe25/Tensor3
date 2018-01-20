#ifndef TILEMAP_RANDOM_GENERATOR_H
#define TILEMAP_RANDOM_GENERATOR_H
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"
#include <stealthutil>
#include <type_traits>
#include <random>

namespace StealthTileMap {
    namespace internal {
        template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime,
            typename Distribution, typename Generator, int areaAtCompileTime, int sizeAtCompileTime>
        struct traits<TileMapRandomGenerator<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime,
            Distribution, Generator, areaAtCompileTime, sizeAtCompileTime>> {
            typedef typename Distribution::result_type ScalarType;
            static constexpr int width = widthAtCompileTime, length = lengthAtCompileTime, height = heightAtCompileTime,
                area = areaAtCompileTime, size = sizeAtCompileTime;
            typedef std::false_type containsData;
            typedef std::false_type isWritable;
        };
    } /* internal */
    namespace {
        static inline std::mt19937 DefaultGenerator;
    }

    // TileMapRandomGenerator number generator
    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime,
        typename Distribution, typename Generator = decltype(DefaultGenerator), int areaAtCompileTime, int sizeAtCompileTime>
    class TileMapRandomGenerator : public TileMapBase<TileMapRandomGenerator<widthAtCompileTime,
        lengthAtCompileTime, heightAtCompileTime, Distribution, Generator, areaAtCompileTime, sizeAtCompileTime>>  {
        public:
            constexpr TileMapRandomGenerator(Distribution&& distribution, long seed = stealth::getCurrentTime(),
                Generator&& generator = std::forward<Generator&&>(DefaultGenerator)) noexcept
                : generator{generator}, distribution{distribution} {
                generator.seed(seed);
            }

            constexpr auto operator()(int x, int y, int z) const noexcept {
                return distribution(generator);
            }

            constexpr auto operator()(int x, int y) const noexcept {
                return distribution(generator);
            }

            constexpr auto operator()(int x) const noexcept {
                return distribution(generator);
            }

            constexpr auto operator[](int index) const noexcept {
                return distribution(generator);
            }

        private:
            Generator& generator;
            Distribution& distribution;
    };

} /* StealthTileMap */

#endif /* end of include guard: TILEMAP_RANDOM_GENERATOR_H */
