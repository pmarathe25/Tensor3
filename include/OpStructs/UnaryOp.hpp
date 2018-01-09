#ifndef UNARY_OP_H
#define UNARY_OP_H
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"

namespace StealthTileMap {
    namespace internal {
        template <typename ReturnType, typename ScalarTypeLHS, UnaryOperation<ReturnType, ScalarTypeLHS> op, typename LHS>
        struct traits<UnaryOp<ReturnType, ScalarTypeLHS, op, LHS>> {
            typedef ReturnType ScalarType;
            // Dimensions
            static constexpr int length = internal::traits<LHS>::length,
                width = internal::traits<LHS>::width,
                height = internal::traits<LHS>::height,
                area = internal::traits<LHS>::area,
                size = internal::traits<LHS>::size;
            typedef std::false_type containsData;
            typedef std::false_type isWritable;
        };
    } /* internal */

    template <typename ReturnType, typename ScalarTypeLHS, UnaryOperation<ReturnType, ScalarTypeLHS> op, typename LHS>
    class UnaryOp : public TileMapBase<UnaryOp<ReturnType, ScalarTypeLHS, op, LHS>> {
        public:
            typedef typename internal::traits<UnaryOp>::ScalarType ScalarType;

            constexpr UnaryOp(const LHS& lhs) noexcept
                : lhs(lhs) { }

            constexpr ScalarType operator()(int x, int y, int z) const {
                return op(lhs(x, y, z));
            }

            constexpr ScalarType operator()(int x, int y) const {
                return op(lhs(x, y));
            }

            constexpr ScalarType operator()(int x) const {
                return op(lhs(x));
            }

            constexpr ScalarType operator[](int x) const {
                return this -> operator()(x);
            }
        private:
            const LHS& lhs;
    };
} /* StealthTileMap */

#endif
