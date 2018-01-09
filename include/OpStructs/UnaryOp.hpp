#ifndef UNARY_OP_H
#define UNARY_OP_H
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"

namespace StealthTileMap {
    namespace internal {
        template <typename ReturnType, typename LHS, UnaryOperation<ReturnType, typename internal::traits<LHS>::ScalarType> op>
        struct traits<UnaryOp<ReturnType, LHS, op>> {
            typedef ReturnType ScalarType;
            // Dimensions
            static constexpr int length = internal::traits<LHS>::length,
                width = internal::traits<LHS>::width,
                height = internal::traits<LHS>::height,
                area = internal::traits<LHS>::area,
                size = internal::traits<LHS>::size;
            static constexpr bool isWritable = false;
        };
    } /* internal */

    template <typename ReturnType, typename LHS, UnaryOperation<ReturnType, typename internal::traits<LHS>::ScalarType> op>
    class UnaryOp : public TileMapBase<UnaryOp<ReturnType, LHS, op>> {
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
