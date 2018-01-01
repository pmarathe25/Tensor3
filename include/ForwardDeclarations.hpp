#ifndef FORWARD_DECLARATIONS_H
#define FORWARD_DECLARATIONS_H
#include <type_traits>

namespace StealthTileMap {
    namespace internal {
        template <typename T> struct traits {
            typedef T ScalarType;
            static constexpr int rows = 1, cols = 1, layers = 1, size = 1;
        };
    } /* internal */

    // TileMap
    template <typename type, int rowsAtCompileTime, int colsAtCompileTime = 1, int layersAtCompileTime = 1,
        int areaAtCompileTime = rowsAtCompileTime * colsAtCompileTime, int sizeAtCompileTime = rowsAtCompileTime * colsAtCompileTime * layersAtCompileTime>
    class TileMap;

    // Binary Op
    template <typename LHS, typename RHS>
    using BinaryOperation = typename std::common_type<LHS, RHS>::type (*)(const LHS&, const RHS&);
    template <typename LHS, typename RHS, BinaryOperation<typename internal::traits<LHS>::ScalarType, typename internal::traits<RHS>::ScalarType> op>
    class BinaryOp;

    // Unary Op
    template <typename LHS>
    using UnaryOperation = LHS (*)(const LHS&);
    template <typename LHS, UnaryOperation<typename internal::traits<LHS>::ScalarType> op>
    class UnaryOp;

    // Runtime Op
    template <typename LHS, typename RuntimeOperation>
    class RuntimeOp;
} /* StealthTileMap */

#endif
