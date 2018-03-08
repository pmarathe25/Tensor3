#ifndef BINARY_OP_H
#define BINARY_OP_H
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"
#include <type_traits>

namespace StealthTileMap {
    namespace internal {
        template <typename BinaryOperation, typename LHS, typename RHS>
        struct traits<BinaryOp<BinaryOperation, LHS, RHS>> {
            // Since the incoming LHS/RHS is either a const ref or copy,
            // we need to remove qualifiers to get size information.
            using LHSNoCV = strip_qualifiers<LHS>;
            using RHSNoCV = strip_qualifiers<RHS>;
            using ScalarType = typename std::invoke_result<BinaryOperation, optimal_scalar_type<LHSNoCV>,
                optimal_scalar_type<RHSNoCV>>::type;
            // Dimensions
            static constexpr int length = (std::is_scalar<LHSNoCV>::value ? internal::traits<RHSNoCV>::length : internal::traits<LHSNoCV>::length),
                width = (std::is_scalar<LHSNoCV>::value ? internal::traits<RHSNoCV>::width : internal::traits<LHSNoCV>::width),
                height = (std::is_scalar<LHSNoCV>::value ? internal::traits<RHSNoCV>::height : internal::traits<LHSNoCV>::height),
                area = (std::is_scalar<LHSNoCV>::value ? internal::traits<RHSNoCV>::area : internal::traits<LHSNoCV>::area),
                size = (std::is_scalar<LHSNoCV>::value ? internal::traits<RHSNoCV>::size : internal::traits<LHSNoCV>::size);
            using containsData = std::false_type;
            using isWritable = std::false_type;
            using UnderlyingTileMapType = BinaryOp<BinaryOperation, LHS, RHS>;
        };
    } /* internal */


    template <typename BinaryOperation, typename LHS, typename RHS>
    class BinaryOp : public TileMapBase<BinaryOp<BinaryOperation, LHS, RHS>> {
        public:
            using ScalarType = typename internal::traits<BinaryOp>::ScalarType;

            constexpr STEALTH_ALWAYS_INLINE BinaryOp(BinaryOperation op, LHS lhs, RHS rhs) noexcept
                : op{std::move(op)}, lhs{lhs}, rhs{rhs} {
                // For the purposes of size information, we need the types without qualifiers
                static_assert(internal::traits<strip_qualifiers<LHS>>::size == internal::traits<strip_qualifiers<RHS>>::size
                    || (std::is_scalar<strip_qualifiers<LHS>>::value || std::is_scalar<strip_qualifiers<RHS>>::value),
                    "Cannot operate on incompatible arguments");
            }

            // Since LHS/RHS could be scalars, tryHintedIndex checks for that possibility.
            constexpr STEALTH_ALWAYS_INLINE auto hintedIndex(int hintX, int hintY, int x, int y, int z) const {
                return op(tryHintedIndex(lhs, hintX, hintY, x, y, z), tryHintedIndex(rhs, hintX, hintY, x, y, z));
            }

            constexpr STEALTH_ALWAYS_INLINE auto hintedIndex(int hintX, int hintY, int x, int y, int z) {
                return op(tryHintedIndex(lhs, hintX, hintY, x, y, z), tryHintedIndex(rhs, hintX, hintY, x, y, z));
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y, int z) const {
                return op(tryIndex(lhs, x, y, z), tryIndex(rhs, x, y, z));
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y) const {
                return op(tryIndex(lhs, x, y), tryIndex(rhs, x, y));
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x) const {
                return op(tryIndex(lhs, x), tryIndex(rhs, x));
            }

        private:
            LHS lhs;
            RHS rhs;
            BinaryOperation op;
    };
} /* StealthTileMap */

#endif
