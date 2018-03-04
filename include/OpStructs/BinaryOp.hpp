#ifndef BINARY_OP_H
#define BINARY_OP_H
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"
#include <type_traits>

namespace StealthTileMap {
    namespace internal {
        template <typename BinaryOperation, typename LHS, typename RHS>
        struct traits<BinaryOp<BinaryOperation, LHS, RHS>> {
            using LHSNoCV = strip_qualifiers<LHS>;
            using RHSNoCV = strip_qualifiers<RHS>;
            typedef typename std::invoke_result<BinaryOperation, optimal_scalar_type<LHSNoCV>,
                optimal_scalar_type<RHSNoCV>>::type ScalarType;
            // Dimensions
            static constexpr int length = (std::is_scalar<LHSNoCV>::value ? internal::traits<RHSNoCV>::length : internal::traits<LHSNoCV>::length),
                width = (std::is_scalar<LHSNoCV>::value ? internal::traits<RHSNoCV>::width : internal::traits<LHSNoCV>::width),
                height = (std::is_scalar<LHSNoCV>::value ? internal::traits<RHSNoCV>::height : internal::traits<LHSNoCV>::height),
                area = (std::is_scalar<LHSNoCV>::value ? internal::traits<RHSNoCV>::area : internal::traits<LHSNoCV>::area),
                size = (std::is_scalar<LHSNoCV>::value ? internal::traits<RHSNoCV>::size : internal::traits<LHSNoCV>::size);
            typedef std::false_type containsData;
            typedef std::false_type isWritable;
            typedef BinaryOp<BinaryOperation, LHSNoCV, RHSNoCV> UnderlyingTileMapType;
        };
    } /* internal */



    // DEBUG:
    template <typename ExprStoredType>
    constexpr void debugType() {
        std::cout << __PRETTY_FUNCTION__ << '\n';
    }



    template <typename BinaryOperation, typename LHS, typename RHS>
    class BinaryOp : public TileMapBase<BinaryOp<BinaryOperation, LHS, RHS>> {
        public:
            typedef typename internal::traits<BinaryOp>::ScalarType ScalarType;

            constexpr STEALTH_ALWAYS_INLINE BinaryOp(BinaryOperation op, LHS lhs, RHS rhs) noexcept
                : op{std::move(op)}, lhs{(lhs)}, rhs{(rhs)} {

                std::cout << "Calling from BinaryOp Constructor" << '\n';
                std::cout << __PRETTY_FUNCTION__ << '\n';
                debugType<expression_stored_type<LHS>>();
                debugType<expression_stored_type<RHS>>();



                static_assert(internal::traits<LHS>::size == internal::traits<RHS>::size
                    || (std::is_scalar<LHS>::value || std::is_scalar<RHS>::value),
                    "Cannot operate on incompatible arguments");
            }


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

        private:
            LHS lhs;
            RHS rhs;
            BinaryOperation op;
    };
} /* StealthTileMap */

#endif
