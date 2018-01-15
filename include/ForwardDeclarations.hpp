#ifndef FORWARD_DECLARATIONS_H
#define FORWARD_DECLARATIONS_H
#include <type_traits>

namespace StealthTileMap {
    namespace internal {
        template <typename T> struct traits {
            typedef T ScalarType;
            static constexpr unsigned length = 1, width = 1, height = 1, size = 1;
        };
    } /* internal */

    // TileMapBase
    template <typename Derived>
    class TileMapBase;

    // TileMap
    template <typename type, unsigned widthAtCompileTime, unsigned lengthAtCompileTime = 1, unsigned heightAtCompileTime = 1,
        unsigned areaAtCompileTime = widthAtCompileTime * lengthAtCompileTime, unsigned sizeAtCompileTime
        = widthAtCompileTime * lengthAtCompileTime * heightAtCompileTime>
    class TileMap;

    // Binary Op

    template <typename BinaryOperation, typename LHS, typename RHS>
    class BinaryOp;

    // Unary Op
    template <typename UnaryOperation, typename LHS>
    class UnaryOp;

    // View of a section of a TileMap or OpStruct
    template <unsigned widthAtCompileTime, unsigned lengthAtCompileTime, unsigned heightAtCompileTime, typename TileMapType,
        typename containsData = typename internal::traits<TileMapType>::containsData,
        typename isWritable = typename std::conjunction<containsData, typename internal::traits<TileMapType>::isWritable>::type>
    class TileMapView;
} /* StealthTileMap */

#endif
