#ifndef RUNTIME_OP_H
#define RUNTIME_OP_H
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"

namespace StealthTileMap {
    namespace internal {
        template <typename UnaryOperation, typename LHS>
        struct traits<UnaryOp<UnaryOperation, LHS>> {
            // Since the incoming LHS is either a const ref or copy,
            // we need to remove qualifiers to get size information.
            using LHSNoCV = strip_qualifiers<LHS>;
            using ScalarType = typename std::invoke_result<UnaryOperation, optimal_scalar_type<LHSNoCV>>::type;
            // Dimensions
            static constexpr int length = internal::traits<LHSNoCV>::length,
                width = internal::traits<LHSNoCV>::width,
                height = internal::traits<LHSNoCV>::height,
                area = internal::traits<LHSNoCV>::area,
                size = internal::traits<LHSNoCV>::size;
            using containsData = std::false_type;
            using isWritable = std::false_type;
            using UnderlyingTileMapType = UnaryOp<UnaryOperation, LHS>;
        };
    } /* internal */

    template <typename UnaryOperation, typename LHS>
    class UnaryOp : public TileMapBase<UnaryOp<UnaryOperation, LHS>> {
        public:
            using ScalarType = typename internal::traits<UnaryOp>::ScalarType;

            constexpr STEALTH_ALWAYS_INLINE UnaryOp(UnaryOperation op, LHS lhs) noexcept
                : op{std::move(op)}, lhs{lhs} { }

            constexpr STEALTH_ALWAYS_INLINE auto hintedIndex(int hintX, int hintY, int x, int y, int z) const {
                return op(lhs.hintedIndex(hintX, hintY, x, y, z));
            }

            constexpr STEALTH_ALWAYS_INLINE auto hintedIndex(int hintX, int hintY, int x, int y, int z) {
                return op(lhs.hintedIndex(hintX, hintY, x, y, z));
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

        private:
            LHS lhs;
            UnaryOperation op;
    };
} /* StealthTileMap */

#endif
