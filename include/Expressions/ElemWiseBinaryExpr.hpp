#pragma once
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"
#include "../utils.hpp"

namespace Stealth {
    namespace internal {
        template <typename BinaryOperation, typename LHS, typename RHS>
        struct traits<ElemWiseBinaryExpr<BinaryOperation, LHS, RHS>> {
            // Since the incoming LHS/RHS is either a reference or copy,
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
            using UnderlyingTileMapType = ElemWiseBinaryExpr<BinaryOperation, LHS, RHS>;
        };
    } /* internal */


    template <typename BinaryOperation, typename LHS, typename RHS>
    class ElemWiseBinaryExpr : public TileMapBase<ElemWiseBinaryExpr<BinaryOperation, LHS, RHS>> {
        public:
            using ScalarType = typename internal::traits<ElemWiseBinaryExpr>::ScalarType;
            // Store either a reference or copy depending on what the operands are.
            using StoredLHS = expression_stored_type<LHS>;
            using StoredRHS = expression_stored_type<RHS>;

            constexpr STEALTH_ALWAYS_INLINE ElemWiseBinaryExpr(BinaryOperation op, LHS&& lhs, RHS&& rhs) noexcept
                : op{std::move(op)}, lhs{std::forward<LHS&&>(lhs)}, rhs{std::forward<RHS&&>(rhs)} {
                // For the purposes of size information, we need the types without qualifiers
                using LHSNoCV = strip_qualifiers<LHS>;
                using RHSNoCV = strip_qualifiers<RHS>;
                static_assert(internal::traits<LHSNoCV>::size == internal::traits<RHSNoCV>::size
                    || (std::is_scalar<LHSNoCV>::value || std::is_scalar<RHSNoCV>::value),
                    "Cannot operate on incompatible arguments");

                #ifdef DEBUG
                    std::cout << "Creating ElemWiseBinaryExpr" << '\n';
                    // LHS
                    std::cout << "LHS Information\nLHS Type: ";
                    debugType<LHS&&>();
                    std::cout << "LHS Stored Type: ";
                    debugType<StoredLHS>();
                    // RHS
                    std::cout << "RHS Information\nRHS Type: ";
                    debugType<RHS&&>();
                    std::cout << "RHS Stored Type: ";
                    debugType<StoredRHS>();
                #endif
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
            StoredLHS lhs;
            StoredRHS rhs;
            BinaryOperation op;
    };
} /* Stealth */
