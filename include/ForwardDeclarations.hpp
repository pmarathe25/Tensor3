#pragma once

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


namespace Stealth::Math {
    namespace internal {
        enum class ExpressionType : int {
            Unknown = 0,
            Tensor3,
            ElemWiseBinaryExpr,
            ElemWiseUnaryExpr,
            BlockExpr
        };

        template <typename T> struct traits { };
        template <typename T> struct traits<const T> : traits<T> { };
        template <typename T> struct traits<T&> : traits<T> { };
        template <typename T> struct traits<const T&> : traits<T> { };
        template <typename T> struct traits<T&&> : traits<T> { };
    } /* internal */

    // Tensor3Base
    template <typename Derived>
    class Tensor3Base;

    // Tensor3
    template <typename type, int widthAtCompileTime = 1, int lengthAtCompileTime = 1, int heightAtCompileTime = 1,
        int areaAtCompileTime = widthAtCompileTime * lengthAtCompileTime, int sizeAtCompileTime
        = widthAtCompileTime * lengthAtCompileTime * heightAtCompileTime>
    class Tensor3;

    // Binary Op

    template <typename LHS, typename BinaryOperation, typename RHS>
    class ElemWiseBinaryExpr;

    // Unary Op
    template <typename UnaryOperation, typename LHS>
    class ElemWiseUnaryExpr;

    // View of a section of a Tensor3 or OpStruct
    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime, typename Tensor3Type>
    class BlockExpr;
} /* Stealth::Math */
