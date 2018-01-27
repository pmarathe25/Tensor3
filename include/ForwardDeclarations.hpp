#ifndef FORWARD_DECLARATIONS_H
#define FORWARD_DECLARATIONS_H
#include <type_traits>

// Macros
#ifdef __GNUC__
    #define STEALTH_ALWAYS_INLINE __attribute__((always_inline))
#else
    #define STEALTH_ALWAYS_INLINE inline
#endif

namespace StealthTileMap {
    namespace internal {
        template <typename T> struct traits {
            typedef T ScalarType;
            static constexpr int length = 1, width = 1, height = 1, size = 1;
        };
    } /* internal */

    // TileMapBase
    template <typename Derived>
    class TileMapBase;

    // TileMap
    template <typename type, int widthAtCompileTime, int lengthAtCompileTime = 1, int heightAtCompileTime = 1,
        int areaAtCompileTime = widthAtCompileTime * lengthAtCompileTime, int sizeAtCompileTime
        = widthAtCompileTime * lengthAtCompileTime * heightAtCompileTime>
    class TileMap;

    // Binary Op

    template <typename BinaryOperation, typename LHS, typename RHS>
    class BinaryOp;

    // Unary Op
    template <typename UnaryOperation, typename LHS>
    class UnaryOp;

    // View of a section of a TileMap or OpStruct
    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename TileMapType,
        typename containsData = typename internal::traits<TileMapType>::containsData,
        typename isWritable = typename std::conjunction<containsData, typename internal::traits<TileMapType>::isWritable>::type>
    class Block;

    // Generate random numbers on demand
    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime,
        typename Distribution, typename Generator, int areaAtCompileTime
        = widthAtCompileTime * lengthAtCompileTime, int sizeAtCompileTime
        = widthAtCompileTime * lengthAtCompileTime * heightAtCompileTime>
    class TileMapRandomGenerator;
} /* StealthTileMap */

#endif
