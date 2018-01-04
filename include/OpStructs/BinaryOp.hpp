#ifndef BINARY_OP_H
#define BINARY_OP_H
#include "../ForwardDeclarations.hpp"
#include "../TileMapBase.hpp"
#include <type_traits>

namespace StealthTileMap {
    namespace internal {
        template <typename LHS, typename RHS, BinaryOperation<typename internal::traits<LHS>::ScalarType,
            typename internal::traits<RHS>::ScalarType> op>
        struct traits<BinaryOp<LHS, RHS, op>> {
            typedef typename internal::traits<LHS>::ScalarType ScalarTypeLHS;
            typedef typename internal::traits<RHS>::ScalarType ScalarTypeRHS;
            typedef typename std::common_type<ScalarTypeLHS, ScalarTypeRHS>::type ScalarType;
            // Dimensions
            static constexpr int length = (std::is_scalar<LHS>::value ? internal::traits<RHS>::length : internal::traits<LHS>::length),
                width = (std::is_scalar<LHS>::value ? internal::traits<RHS>::width : internal::traits<LHS>::width),
                height = (std::is_scalar<LHS>::value ? internal::traits<RHS>::height : internal::traits<LHS>::height),
                area = (std::is_scalar<LHS>::value ? internal::traits<RHS>::area : internal::traits<LHS>::area),
                size = (std::is_scalar<LHS>::value ? internal::traits<RHS>::size : internal::traits<LHS>::size);
            static constexpr bool isWritable = false;
        };
    } /* internal */

    template <typename LHS, typename RHS, BinaryOperation<typename internal::traits<LHS>::ScalarType,
        typename internal::traits<RHS>::ScalarType> op>
    class BinaryOp : public TileMapBase<BinaryOp<LHS, RHS, op>> {
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
