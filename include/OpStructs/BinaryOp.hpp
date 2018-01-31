#ifndef BINARY_OP_H
#define BINARY_OP_H
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"
#include <type_traits>

namespace StealthTileMap {
    namespace internal {
        template <typename BinaryOperation, typename LHS, typename RHS>
        struct traits<BinaryOp<BinaryOperation, LHS, RHS>> {
            typedef typename std::invoke_result<BinaryOperation, optimal_scalar_type<LHS>,
                optimal_scalar_type<RHS>>::type ScalarType;
            // Dimensions
            static constexpr int length = (std::is_scalar<LHS>::value ? internal::traits<RHS>::length : internal::traits<LHS>::length),
                width = (std::is_scalar<LHS>::value ? internal::traits<RHS>::width : internal::traits<LHS>::width),
                height = (std::is_scalar<LHS>::value ? internal::traits<RHS>::height : internal::traits<LHS>::height),
                area = (std::is_scalar<LHS>::value ? internal::traits<RHS>::area : internal::traits<LHS>::area),
                size = (std::is_scalar<LHS>::value ? internal::traits<RHS>::size : internal::traits<LHS>::size);
            typedef std::false_type containsData;
            typedef std::false_type isWritable;
            typedef BinaryOp<BinaryOperation, LHS, RHS> UnderlyingTileMapType;
        };
    } /* internal */

    template <typename BinaryOperation, typename LHS, typename RHS>
    class BinaryOp : public TileMapBase<BinaryOp<BinaryOperation, LHS, RHS>> {
        public:
            typedef typename internal::traits<BinaryOp>::ScalarType ScalarType;

            constexpr STEALTH_ALWAYS_INLINE BinaryOp(const BinaryOperation& op, const LHS& lhs, const RHS& rhs) noexcept
                : op{op}, lhs{lhs}, rhs{rhs} { }

            constexpr STEALTH_ALWAYS_INLINE auto hintedIndex(int hintX, int hintY, int x, int y, int z) const {
                if constexpr (std::is_scalar<RHS>::value) {
                    return op(lhs.hintedIndex(hintX, hintY, x, y, z), rhs);
                } else if constexpr (std::is_scalar<LHS>::value) {
                    return op(lhs, rhs.hintedIndex(hintX, hintY, x, y, z));
                } else {
                    return op(lhs.hintedIndex(hintX, hintY, x, y, z), rhs.hintedIndex(hintX, hintY, x, y, z));
                }
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y, int z) const {
                if constexpr (std::is_scalar<RHS>::value) {
                    return op(lhs(x, y, z), rhs);
                } else if constexpr (std::is_scalar<LHS>::value) {
                    return op(lhs, rhs(x, y, z));
                } else {
                    return op(lhs(x, y, z), rhs(x, y, z));
                }
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y) const {
                if constexpr (std::is_scalar<RHS>::value) {
                    return op(lhs(x, y), rhs);
                } else if constexpr (std::is_scalar<LHS>::value) {
                    return op(lhs, rhs(x, y));
                } else {
                    return op(lhs(x, y), rhs(x, y));
                }
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x) const {
                if constexpr (std::is_scalar<RHS>::value) {
                    return op(lhs(x), rhs);
                } else if constexpr (std::is_scalar<LHS>::value) {
                    return op(lhs, rhs(x));
                } else {
                    return op(lhs(x), rhs(x));
                }
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator[](int x) const {
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
