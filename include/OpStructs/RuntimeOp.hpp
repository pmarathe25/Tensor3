#ifndef RUNTIME_OP_H
#define RUNTIME_OP_H
#include "../ForwardDeclarations.hpp"

namespace StealthTileMap {
    namespace internal {
        template <typename LHS, typename RuntimeOperation>
        struct traits<RuntimeOp<LHS, RuntimeOperation>> {
            typedef typename std::invoke_result<RuntimeOperation, typename internal::traits<LHS>::ScalarType>::type ScalarType;
            // Dimensions
            static constexpr int length = internal::traits<LHS>::length,
                width = internal::traits<LHS>::width,
                height = internal::traits<LHS>::height,
                area = internal::traits<LHS>::area,
                size = internal::traits<LHS>::size;
        };
    } /* internal */

    template <typename LHS, typename RuntimeOperation>
    class RuntimeOp {
        public:
            typedef typename internal::traits<RuntimeOp>::ScalarType ScalarType;
            // Dimensions
            static constexpr int length = internal::traits<RuntimeOp>::length, width = internal::traits<RuntimeOp>::width,
                height = internal::traits<RuntimeOp>::height, area = internal::traits<RuntimeOp>::area,
                size = internal::traits<RuntimeOp>::size;

            constexpr RuntimeOp(const RuntimeOperation& op, const LHS& lhs) noexcept
                : op(op), lhs(lhs) { }

            constexpr ScalarType operator()(int x, int y = 0, int z = 0) const {
                return op(lhs(x, y, z));
            }

            constexpr TileMap<ScalarType, length, width, height> eval() {
                return (*this);
            }
        private:
            const LHS& lhs;
            const RuntimeOperation& op;
    };
} /* StealthTileMap */

#endif
