#ifndef UNARY_OP_H
#define UNARY_OP_H
#include "../ForwardDeclarations.hpp"

namespace StealthTileMap {
    namespace internal {
        template <typename LHS, UnaryOperation<typename internal::traits<LHS>::ScalarType> op>
        struct traits<UnaryOp<LHS, op>> {
            typedef typename internal::traits<LHS>::ScalarType ScalarType;
            // Dimensions
            static constexpr int length = internal::traits<LHS>::length,
                width = internal::traits<LHS>::width,
                height = internal::traits<LHS>::height,
                area = internal::traits<LHS>::area,
                size = internal::traits<LHS>::size;
            static constexpr bool isTemporary = true;
        };
    } /* internal */

    template <typename LHS, UnaryOperation<typename internal::traits<LHS>::ScalarType> op>
    class UnaryOp {
        public:
            typedef typename internal::traits<UnaryOp>::ScalarType ScalarType;

            constexpr UnaryOp(const LHS& lhs) noexcept
                : lhs(lhs) { }

            constexpr ScalarType operator()(int x, int y = 0, int z = 0) const {
                return op(lhs(x, y, z));
            }

            constexpr TileMap<ScalarType, internal::traits<UnaryOp>::length,
                internal::traits<UnaryOp>::width, internal::traits<UnaryOp>::height> eval() {
                return (*this);
            }
        private:
            const LHS& lhs;
    };
} /* StealthTileMap */

#endif
