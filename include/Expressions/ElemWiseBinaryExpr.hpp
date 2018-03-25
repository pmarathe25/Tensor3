#pragma once
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"
#include "../utils.hpp"

namespace Stealth {
    namespace internal {
        template <typename LHS, typename BinaryOperation, typename RHS>
        struct traits<ElemWiseBinaryExpr<LHS, BinaryOperation, RHS>> {
            // Since the incoming LHS/RHS is either a reference or copy,
            // we need to remove qualifiers to get size information.
            using ScalarType = typename std::invoke_result<BinaryOperation, scalar_element<raw_type<LHS>>,
                scalar_element<raw_type<RHS>>>::type;
            // Dimensions
            static constexpr int length = internal::traits<raw_type<LHS>>::length,
                width = internal::traits<raw_type<LHS>>::width,
                height = internal::traits<raw_type<LHS>>::height,
                area = internal::traits<raw_type<LHS>>::area,
                size = internal::traits<raw_type<LHS>>::size,
                indexingModeRequired = std::max(internal::traits<raw_type<LHS>>::indexingModeRequired,
                    internal::traits<raw_type<RHS>>::indexingModeRequired);
            using StoredLHS = expression_stored_type<LHS>;
            using StoredRHS = expression_stored_type<RHS>;
        };
    } /* internal */


    template <typename LHS, typename BinaryOperation, typename RHS>
    class ElemWiseBinaryExpr : public TileMapBase<ElemWiseBinaryExpr<LHS, BinaryOperation, RHS>> {
        // Store either a reference or copy depending on what the operands are.
        using StoredLHS = typename internal::traits<ElemWiseBinaryExpr>::StoredLHS;
        using StoredRHS = typename internal::traits<ElemWiseBinaryExpr>::StoredRHS;

        public:
            constexpr STEALTH_ALWAYS_INLINE ElemWiseBinaryExpr(LHS&& lhs, BinaryOperation op, RHS&& rhs) noexcept
                : lhs{std::forward<LHS&&>(lhs)}, op{std::move(op)}, rhs{std::forward<RHS&&>(rhs)} {
                // For the purposes of size information, we need the types without qualifiers
                static_assert(internal::traits<raw_type<LHS>>::size == internal::traits<raw_type<RHS>>::size,
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

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y, int z) const {
                return op(lhs(x, y, z), rhs(x, y, z));
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x, int y) const {
                return op(lhs(x, y), rhs(x, y));
            }

            constexpr STEALTH_ALWAYS_INLINE auto operator()(int x) const {
                return op(lhs(x), rhs(x));
            }

        private:
            StoredLHS lhs;
            BinaryOperation op;
            StoredRHS rhs;
    };
} /* Stealth */
