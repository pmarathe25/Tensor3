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

        template <typename T> struct traits {
            using ScalarType = T;
            static constexpr int width = 1, length = 1, height = 1, area = 1, size = 1,
                indexingMode = 1;
        };

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

    // Convenience typedefs
    template <int widthAtCompileTime = 1, int lengthAtCompileTime = 1, int heightAtCompileTime = 1>
    using Tensor3I = Tensor3<int, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime>;

    template <int widthAtCompileTime = 1, int lengthAtCompileTime = 1, int heightAtCompileTime = 1>
    using Tensor3F = Tensor3<float, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime>;

    template <int widthAtCompileTime = 1, int lengthAtCompileTime = 1, int heightAtCompileTime = 1>
    using Tensor3D = Tensor3<double, widthAtCompileTime, lengthAtCompileTime, heightAtCompileTime>;

    template <typename ScalarType>
    using Scalar = Tensor3<ScalarType, 1, 1, 1>;

    template <typename ScalarType, int size>
    using Vector = Tensor3<ScalarType, size, 1, 1>;

    template <typename ScalarType, int width, int length>
    using Matrix = Tensor3<ScalarType, width, length, 1>;

    using ScalarI = Scalar<int>;
    using ScalarF = Scalar<float>;
    using ScalarD = Scalar<double>;

    template <int size>
    using VectorI = Vector<int, size>;

    template <int size>
    using VectorF = Vector<float, size>;

    template <int size>
    using VectorD = Vector<double, size>;

    template <int width, int length>
    using MatrixI = Matrix<int, width, length>;

    template <int width, int length>
    using MatrixF = Matrix<float, width, length>;

    template <int width, int length>
    using MatrixD = Matrix<double, width, length>;
} /* Stealth::Math */
