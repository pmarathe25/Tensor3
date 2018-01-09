#ifndef BINARY_OP_H
#define BINARY_OP_H
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"
#include <type_traits>

namespace StealthTileMap {
    namespace internal {
        template <typename ReturnType, typename ScalarTypeLHS, typename ScalarTypeRHS, BinaryOperation<ReturnType,
            ScalarTypeLHS, ScalarTypeRHS> op, typename LHS, typename RHS>
        struct traits<BinaryOp<ReturnType, ScalarTypeLHS, ScalarTypeRHS, op, LHS, RHS>> {
            typedef ReturnType ScalarType;
            // Dimensions
            static constexpr int length = (std::is_scalar<LHS>::value ? internal::traits<RHS>::length : internal::traits<LHS>::length),
                width = (std::is_scalar<LHS>::value ? internal::traits<RHS>::width : internal::traits<LHS>::width),
                height = (std::is_scalar<LHS>::value ? internal::traits<RHS>::height : internal::traits<LHS>::height),
                area = (std::is_scalar<LHS>::value ? internal::traits<RHS>::area : internal::traits<LHS>::area),
                size = (std::is_scalar<LHS>::value ? internal::traits<RHS>::size : internal::traits<LHS>::size);
            typedef std::false_type containsData;
            typedef std::false_type isWritable;
        };
    } /* internal */

    template <typename ReturnType, typename ScalarTypeLHS, typename ScalarTypeRHS, BinaryOperation<ReturnType,
        ScalarTypeLHS, ScalarTypeRHS> op, typename LHS, typename RHS>
    class BinaryOp : public TileMapBase<BinaryOp<ReturnType, ScalarTypeLHS, ScalarTypeRHS, op, LHS, RHS>> {
        public:
            typedef typename internal::traits<BinaryOp>::ScalarType ScalarType;

            constexpr BinaryOp(const LHS& lhs, const RHS& rhs) noexcept
                : lhs(lhs), rhs(rhs) { }

            constexpr ScalarType operator()(int x, int y, int z) const {
                if constexpr (std::is_scalar<RHS>::value) {
                    return op(lhs(x, y, z), rhs);
                } else if constexpr (std::is_scalar<LHS>::value) {
                    return op(lhs, rhs(x, y, z));
                } else {
                    return op(lhs(x, y, z), rhs(x, y, z));
                }
            }

            constexpr ScalarType operator()(int x, int y) const {
                if constexpr (std::is_scalar<RHS>::value) {
                    return op(lhs(x, y), rhs);
                } else if constexpr (std::is_scalar<LHS>::value) {
                    return op(lhs, rhs(x, y));
                } else {
                    return op(lhs(x, y), rhs(x, y));
                }
            }

            constexpr ScalarType operator()(int x) const {
                if constexpr (std::is_scalar<RHS>::value) {
                    return op(lhs(x), rhs);
                } else if constexpr (std::is_scalar<LHS>::value) {
                    return op(lhs, rhs(x));
                } else {
                    return op(lhs(x), rhs(x));
                }
            }

            constexpr ScalarType operator[](int x) const {
                return this -> operator()(x);
            }
        private:
            const LHS& lhs;
            const RHS& rhs;
    };
} /* StealthTileMap */

#endif
