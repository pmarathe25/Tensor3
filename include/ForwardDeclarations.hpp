#pragma once
#include <type_traits>

// Macros
#ifdef __GNUC__
    #define STEALTH_ALWAYS_INLINE __attribute__((always_inline))
#else
    #define STEALTH_ALWAYS_INLINE inline
#endif


// DEBUG:
// #define DEBUG
// DEBUG:
#ifdef DEBUG
    #include <iostream>
    // DEBUG:
    template <typename ExprStoredType>
    constexpr void debugType() {
        std::cout << __PRETTY_FUNCTION__ << '\n';
    }
#endif


namespace Stealth {
    namespace internal {
        template <typename T>
        struct traits {
            typedef T ScalarType;
            static constexpr int length = 1, width = 1, height = 1, size = 1;
            static constexpr bool is_scalar = size == 1;
            static constexpr bool is_vector = !is_scalar and (width == size or length == size or height == size);
            static constexpr bool is_matrix = !is_vector and (width == 1 or length == 1 or height == 1);
        };
    } /* internal */

    // TileMapBase
    template <typename Derived>
    class TileMapBase;

    // TileMap
    template <typename type, int widthAtCompileTime = 1, int lengthAtCompileTime = 1, int heightAtCompileTime = 1,
        int areaAtCompileTime = widthAtCompileTime * lengthAtCompileTime, int sizeAtCompileTime
        = widthAtCompileTime * lengthAtCompileTime * heightAtCompileTime>
    class TileMap;

    // Binary Op

    template <typename LHS, typename BinaryOperation, typename RHS>
    class ElemWiseBinaryExpr;

    // Unary Op
    template <typename UnaryOperation, typename LHS>
    class ElemWiseUnaryExpr;

    // View of a section of a TileMap or OpStruct
    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename TileMapType>
    class BlockExpr;

    // Generate random numbers on demand
    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime,
        typename Distribution, typename Generator, int areaAtCompileTime
        = widthAtCompileTime * lengthAtCompileTime, int sizeAtCompileTime
        = widthAtCompileTime * lengthAtCompileTime * heightAtCompileTime>
    class TileMapRandomGenerator;
} /* Stealth */
