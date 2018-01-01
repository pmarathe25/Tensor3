#ifndef BINARY_OPS_H
#define BINARY_OPS_H
#include "../OpStructs/BinaryOp.hpp"
#include "./InternalOperations.hpp"

namespace StealthTileMap {
    template <typename LHS, typename RHS>
    constexpr void checkTileMapCompatibility() noexcept {
        static_assert(internal::traits<LHS>::length == internal::traits<RHS>::length
            && internal::traits<LHS>::width == internal::traits<RHS>::width
            && internal::traits<LHS>::height == internal::traits<RHS>::height
            || (internal::traits<LHS>::size == 1 || internal::traits<RHS>::size == 1),
            "Cannot operate on incompatible arguments");
    }

    template <typename LHS, typename RHS>
    constexpr BinaryOp<LHS, RHS, internal::ops::add> operator+(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp<LHS, RHS, internal::ops::add>{lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr BinaryOp<LHS, RHS, internal::ops::subtract> operator-(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp<LHS, RHS, internal::ops::subtract>{lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr BinaryOp<LHS, RHS, internal::ops::multiply> operator*(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp<LHS, RHS, internal::ops::multiply>{lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr BinaryOp<LHS, RHS, internal::ops::divide> operator/(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp<LHS, RHS, internal::ops::divide>{lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr BinaryOp<LHS, RHS, internal::ops::eq> operator==(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp<LHS, RHS, internal::ops::eq>{lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr BinaryOp<LHS, RHS, internal::ops::less> operator<(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp<LHS, RHS, internal::ops::less>{lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr BinaryOp<LHS, RHS, internal::ops::lessEq> operator<=(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp<LHS, RHS, internal::ops::lessEq>{lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr BinaryOp<LHS, RHS, internal::ops::greater> operator>(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp<LHS, RHS, internal::ops::greater>{lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr BinaryOp<LHS, RHS, internal::ops::greaterEq> operator>=(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp<LHS, RHS, internal::ops::greaterEq>{lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr BinaryOp<LHS, RHS, internal::ops::andOp> operator&&(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp<LHS, RHS, internal::ops::andOp>{lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr BinaryOp<LHS, RHS, internal::ops::orOp> operator||(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp<LHS, RHS, internal::ops::orOp>{lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr BinaryOp<LHS, RHS, internal::ops::min> min(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp<LHS, RHS, internal::ops::min>{lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr BinaryOp<LHS, RHS, internal::ops::max> max(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp<LHS, RHS, internal::ops::max>{lhs, rhs};
    }
} /* StealthTileMap */

#endif
