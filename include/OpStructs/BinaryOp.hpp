#ifndef BINARY_OP_H
#define BINARY_OP_H
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"
#include <type_traits>

namespace StealthTileMap {
    namespace internal {
        template <typename BinaryOperation, typename LHS, typename RHS>
        struct traits<BinaryOp<BinaryOperation, LHS, RHS>> {
            typedef typename std::invoke_result<BinaryOperation, typename optimal_scalar_type<LHS>::type,
                typename optimal_scalar_type<RHS>::type>::type ScalarType;
            // Dimensions
            static constexpr std::size_t length = (std::is_scalar<LHS>::value ? internal::traits<RHS>::length : internal::traits<LHS>::length),
                width = (std::is_scalar<LHS>::value ? internal::traits<RHS>::width : internal::traits<LHS>::width),
                height = (std::is_scalar<LHS>::value ? internal::traits<RHS>::height : internal::traits<LHS>::height),
                area = (std::is_scalar<LHS>::value ? internal::traits<RHS>::area : internal::traits<LHS>::area),
                size = (std::is_scalar<LHS>::value ? internal::traits<RHS>::size : internal::traits<LHS>::size);
            typedef std::false_type containsData;
            typedef std::false_type isWritable;
        };
    } /* internal */

    template <typename BinaryOperation, typename LHS, typename RHS>
    class BinaryOp : public TileMapBase<BinaryOp<BinaryOperation, LHS, RHS>> {
        public:
            typedef typename internal::traits<BinaryOp>::ScalarType ScalarType;

            constexpr BinaryOp(const BinaryOperation& op, const LHS& lhs, const RHS& rhs) noexcept
                : op{op}, lhs{lhs}, rhs{rhs} { }

            constexpr auto operator()(std::size_t x, std::size_t y, std::size_t z) const {
                if constexpr (std::is_scalar<RHS>::value) {
                    return op(lhs(x, y, z), rhs);
                } else if constexpr (std::is_scalar<LHS>::value) {
                    return op(lhs, rhs(x, y, z));
                } else {
                    return op(lhs(x, y, z), rhs(x, y, z));
                }
            }

            constexpr auto operator()(std::size_t x, std::size_t y) const {
                if constexpr (std::is_scalar<RHS>::value) {
                    return op(lhs(x, y), rhs);
                } else if constexpr (std::is_scalar<LHS>::value) {
                    return op(lhs, rhs(x, y));
                } else {
                    return op(lhs(x, y), rhs(x, y));
                }
            }

            constexpr auto operator()(std::size_t x) const {
                if constexpr (std::is_scalar<RHS>::value) {
                    return op(lhs(x), rhs);
                } else if constexpr (std::is_scalar<LHS>::value) {
                    return op(lhs, rhs(x));
                } else {
                    return op(lhs(x), rhs(x));
                }
            }

            constexpr auto operator[](std::size_t x) const {
                if constexpr (std::is_scalar<RHS>::value) {
                    return op(lhs[x], rhs);
                } else if constexpr (std::is_scalar<LHS>::value) {
                    return op(lhs, rhs[x]);
                } else {
                    return op(lhs[x], rhs[x]);
                }
            }
        private:
            const LHS& lhs;
            const RHS& rhs;
            const BinaryOperation& op;
    };
} /* StealthTileMap */

#endif
