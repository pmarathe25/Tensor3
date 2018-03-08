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
            typedef TileMapRandomGenerator<widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime,
                Distribution, Generator, areaAtCompileTime, sizeAtCompileTime> UnderlyingTileMapType;
        };
    } /* internal */

    namespace {
        static std::mt19937 DefaultGenerator;
        static std::uniform_real_distribution DefaultDistribution{0.0f, 1.0f};
    }

    // TileMapRandomGenerator number generator
    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime,
        typename Distribution = decltype(DefaultDistribution), typename Generator = decltype(DefaultGenerator),
        int areaAtCompileTime, int sizeAtCompileTime>
    class TileMapRandomGenerator : public TileMapBase<TileMapRandomGenerator<widthAtCompileTime,
        lengthAtCompileTime, heightAtCompileTime, Distribution, Generator, areaAtCompileTime, sizeAtCompileTime>>  {
        public:
            constexpr STEALTH_ALWAYS_INLINE TileMapRandomGenerator(Distribution distribution = DefaultDistribution,
                long seed = stealth::getCurrentTime(), Generator generator = DefaultGenerator) noexcept
                : distribution{distribution}, generator{generator} {
                generator.seed(seed);
            }

            constexpr STEALTH_ALWAYS_INLINE auto hintedIndex(int hintX = 0, int hintY = 0, int x = 0, int y = 0, int z = 0) noexcept {
                return distribution(generator);
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x = 0, int y = 0, int z = 0) noexcept {
                return distribution(generator);
            }
        private:
            Distribution distribution;
            Generator generator;
    };

} /* StealthTileMap */

#endif /* end of include guard: TILEMAP_RANDOM_GENERATOR_H */
