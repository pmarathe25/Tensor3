#ifndef UNARY_OPS_H
#define UNARY_OPS_H
#include "../OpStructs/UnaryOp.hpp"

namespace StealthTileMap {
    namespace internal::ops {
        // Internal Unary Operations
        template <typename LHS>
        struct notOp {
            constexpr STEALTH_ALWAYS_INLINE auto operator()(optimal_scalar_type<LHS> lhs) const noexcept {
                return !lhs;
            }
        };
    } /* StealthTileMap::internal::ops */

    template <typename UnaryOperation, typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto apply(const UnaryOperation& op, LHS&& lhs) noexcept {
        return UnaryOp{op, std::forward<LHS&&>(lhs)};
    }

    template <typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto operator!(LHS&& lhs) noexcept {
        return UnaryOp{internal::ops::notOp<LHS&&>{}, std::forward<LHS&&>(lhs)};
    }
} /* StealthTileMap */

#endif
