#ifndef FORWARD_DECLARATIONS_H
#define FORWARD_DECLARATIONS_H
#include <type_traits>
#include <cstddef>

namespace StealthTileMap {
    namespace internal {
        template <typename T> struct traits {
            typedef T ScalarType;
            static constexpr std::size_t length = 1, width = 1, height = 1, size = 1;
        };
    } /* internal */

    // TileMapBase
    template <typename Derived>
    class TileMapBase;

    // TileMap
    template <typename type, std::size_t widthAtCompileTime, std::size_t lengthAtCompileTime = 1, std::size_t heightAtCompileTime = 1,
        std::size_t areaAtCompileTime = widthAtCompileTime * lengthAtCompileTime, std::size_t sizeAtCompileTime
        = widthAtCompileTime * lengthAtCompileTime * heightAtCompileTime>
    class TileMap;

    // Binary Op

    template <typename BinaryOperation, typename LHS, typename RHS>
    class BinaryOp;

    // Unary Op
    template <typename UnaryOperation, typename LHS>
    class UnaryOp;

    // View of a section of a TileMap or OpStruct
    template <std::size_t widthAtCompileTime, std::size_t lengthAtCompileTime, std::size_t heightAtCompileTime, typename TileMapType,
        typename containsData = typename internal::traits<TileMapType>::containsData,
        typename isWritable = typename std::conjunction<containsData, typename internal::traits<TileMapType>::isWritable>::type>
    class TileMapView;
} /* StealthTileMap */

#endif
