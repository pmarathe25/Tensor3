#ifndef FORWARD_DECLARATIONS_H
#define FORWARD_DECLARATIONS_H
#include <type_traits>

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
    template <typename ReturnType, typename ScalarTypeLHS, typename ScalarTypeRHS>
    using BinaryOperation = ReturnType (*)(const ScalarTypeLHS&, const ScalarTypeRHS&);
    template <typename ReturnType, typename LHS, typename RHS, BinaryOperation<ReturnType, typename internal::traits<LHS>::ScalarType, typename internal::traits<RHS>::ScalarType> op>
    class BinaryOp;

    // Unary Op
    template <typename ReturnType, typename ScalarTypeLHS>
    using UnaryOperation = ReturnType (*)(ScalarTypeLHS);
    template <typename ReturnType, typename ScalarTypeLHS, UnaryOperation<ReturnType, ScalarTypeLHS> op, typename LHS>
    class UnaryOp;

    // Runtime Op
    template <typename LHS, typename RuntimeOperation>
    class RuntimeOp;

    // View of a section of a TileMap or OpStruct
    template <int widthAtCompileTime, int lengthAtCompileTime, int heightAtCompileTime,
        typename TileMapType, bool isWritable = internal::traits<TileMapType>::isWritable>
    class TileMapView;
} /* StealthTileMap */

#endif
