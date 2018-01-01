#ifndef RUNTIME_OP_H
#define RUNTIME_OP_H

namespace StealthTileMap {
    namespace internal {
        template <typename LHS, typename RuntimeOperation>
        struct traits<RuntimeOp<LHS, RuntimeOperation>> {
            typedef typename std::invoke_result<RuntimeOperation, typename internal::traits<LHS>::ScalarType>::type ScalarType;
            // Dimensions
            static constexpr int rows = internal::traits<LHS>::rows,
                cols = internal::traits<LHS>::cols,
                size = internal::traits<LHS>::size;
        };
    } /* internal */

    template <typename LHS, typename RuntimeOperation>
    class RuntimeOp {
        public:
            typedef typename internal::traits<RuntimeOp>::ScalarType ScalarType;
            // Dimensions
            static constexpr int rows = internal::traits<RuntimeOp>::rows, cols = internal::traits<RuntimeOp>::cols,
                size = internal::traits<RuntimeOp>::size;

            constexpr RuntimeOp(const RuntimeOperation& op, const LHS& lhs) noexcept
                : op(op), lhs(lhs) { }

            constexpr ScalarType at(int row, int col = 0, int layer = 0) const {
                return op(lhs.at(row, col, layer));
            }

            constexpr TileMap<ScalarType, rows, cols> eval() {
                return (*this);
            }
        private:
            const LHS& lhs;
            const RuntimeOperation& op;
    };
} /* StealthTileMap */

#endif
