#ifndef RUNTIME_OP_H
#define RUNTIME_OP_H
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"

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
            typedef std::false_type containsData;
            typedef std::false_type isWritable;
        };
    } /* internal */

    template <typename LHS, typename RuntimeOperation>
    class RuntimeOp : public TileMapBase<RuntimeOp<LHS, RuntimeOperation>> {
        public:
            typedef typename internal::traits<RuntimeOp>::ScalarType ScalarType;

            constexpr RuntimeOp(const RuntimeOperation& op, const LHS& lhs) noexcept
                : op(op), lhs(lhs) { }

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
            const RuntimeOperation& op;
    };
} /* StealthTileMap */

#endif
