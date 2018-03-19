#pragma once
#include "../Expressions/BlockExpr.hpp"

namespace Stealth {
    template <typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto layer(LHS&& lhs, int layerNum = 0) {
        typedef typename std::remove_reference<LHS>::type LHSRawType;
        return BlockExpr<lhs.width(), lhs.length(), 1, LHSRawType>
            {std::forward<LHS&&>(lhs), 0, 0, layerNum};
    }

    template <int width, int length, int height, typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto block(LHS&& lhs, int minX = 0, int minY = 0, int minZ = 0) noexcept {
        typedef typename std::remove_reference<LHS>::type LHSRawType;
        return BlockExpr<width, length, height, LHSRawType>{std::forward<LHS&&>(lhs), minX, minY, minZ};
    }

    template <int width, int length, typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto block(LHS&& lhs, int minX = 0, int minY = 0, int minZ = 0) {
        typedef typename std::remove_reference<LHS>::type LHSRawType;
        return BlockExpr<width, length, 1, LHSRawType>{std::forward<LHS&&>(lhs), minX, minY, minZ};
    }

    template <int width, typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto block(LHS&& lhs, int minX = 0, int minY = 0, int minZ = 0) {
        typedef typename std::remove_reference<LHS>::type LHSRawType;
        return BlockExpr<width, 1, 1, LHSRawType>{std::forward<LHS&&>(lhs), minX, minY, minZ};
    }

    template <int width = 1, int length = 1, int height = 1, typename LHS>
    constexpr STEALTH_ALWAYS_INLINE auto reshape(LHS&& lhs) {
        typedef typename std::remove_reference<LHS>::type LHSRawType;
        static_assert(width * length * height == internal::traits<LHSRawType>::size, "Cannot reshape into incompatible dimensions");
        return BlockExpr<width, length, height, LHSRawType>{std::forward<LHS&&>(lhs), 0, 0, 0};
    }

} /* Stealth */
