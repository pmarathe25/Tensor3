#ifndef RUNTIME_OP_H
#define RUNTIME_OP_H
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"

namespace StealthTileMap {
    namespace internal {
        template <typename UnaryOperation, typename LHS>
        struct traits<UnaryOp<UnaryOperation, LHS>> {
            typedef typename std::invoke_result<UnaryOperation, typename optimal_scalar_type<LHS>::type>::type ScalarType;
            // Dimensions
            static constexpr size_t length = internal::traits<LHS>::length,
                width = internal::traits<LHS>::width,
                height = internal::traits<LHS>::height,
                area = internal::traits<LHS>::area,
                size = internal::traits<LHS>::size;
            typedef std::false_type containsData;
            typedef std::false_type isWritable;
        };
    } /* internal */

    template <typename UnaryOperation, typename LHS>
    class UnaryOp : public TileMapBase<UnaryOp<UnaryOperation, LHS>> {
        public:
            typedef typename internal::traits<UnaryOp>::ScalarType ScalarType;

            constexpr UnaryOp(const UnaryOperation& op, const LHS& lhs) noexcept
                : op{op}, lhs{lhs} { }

            constexpr auto operator()(size_t x, size_t y, size_t z) const {
                return op(lhs(x, y, z));
            }

            constexpr auto operator()(size_t x, size_t y) const {
                return op(lhs(x, y));
            }

            constexpr auto operator()(size_t x) const {
                return op(lhs(x));
            }

            constexpr auto operator[](size_t x) const {
                return op(lhs[x]);
            }
        private:
            const LHS& lhs;
            const UnaryOperation& op;
    };
} /* StealthTileMap */

#endif
