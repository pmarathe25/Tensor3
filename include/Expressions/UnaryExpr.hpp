#ifndef RUNTIME_OP_H
#define RUNTIME_OP_H
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"
#include "../utils.hpp"

namespace Stealth {
    namespace internal {
        template <typename UnaryOperation, typename LHS>
        struct traits<UnaryExpr<UnaryOperation, LHS>> {
            // Since the incoming LHS is either a reference or copy,
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
            using UnderlyingTileMapType = UnaryExpr<UnaryOperation, LHS>;
        };
    } /* internal */

    template <typename UnaryOperation, typename LHS>
    class UnaryExpr : public TileMapBase<UnaryExpr<UnaryOperation, LHS>> {
        public:
            using ScalarType = typename internal::traits<UnaryExpr>::ScalarType;
            // Store either a reference or copy depending on what the operand is.
            using StoredLHS = expression_stored_type<LHS>;

            constexpr STEALTH_ALWAYS_INLINE UnaryExpr(UnaryOperation op, LHS&& lhs) noexcept
                : op{std::move(op)}, lhs{std::forward<LHS&&>(lhs)} { }

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
            StoredLHS lhs;
            UnaryOperation op;
    };
} /* Stealth */

#endif
