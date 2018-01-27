#ifndef RUNTIME_OP_H
#define RUNTIME_OP_H
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"

namespace StealthTileMap {
    namespace internal {
        template <typename UnaryOperation, typename LHS>
        struct traits<UnaryOp<UnaryOperation, LHS>> {
            typedef typename std::invoke_result<UnaryOperation, optimal_scalar_type<LHS>>::type ScalarType;
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

    template <typename UnaryOperation, typename LHS>
    class UnaryOp : public TileMapBase<UnaryOp<UnaryOperation, LHS>> {
        public:
            typedef typename internal::traits<UnaryOp>::ScalarType ScalarType;

            constexpr STEALTH_ALWAYS_INLINE UnaryOp(const UnaryOperation& op, const LHS& lhs) noexcept
                : op{op}, lhs{lhs} { }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int index, int x, int y, int z) const {
                return op(lhs(index, x, y, z));
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y, int z) const {
                return op(lhs(x, y, z));
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y) const {
                return op(lhs(x, y));
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x) const {
                return op(lhs(x));
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator[](int x) const {
                return op(lhs[x]);
            }
        private:
            const LHS& lhs;
            const UnaryOperation& op;
    };
} /* StealthTileMap */

#endif
