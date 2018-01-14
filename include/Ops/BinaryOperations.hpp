#ifndef BINARY_OPS_H
#define BINARY_OPS_H
#include "../OpStructs/BinaryOp.hpp"
#include "./InternalOperations.hpp"

namespace StealthTileMap {
    template <typename LHS, typename RHS>
    constexpr void checkTileMapCompatibility() noexcept {
        static_assert(internal::traits<LHS>::size == internal::traits<RHS>::size
            || (internal::traits<LHS>::size == 1 || internal::traits<RHS>::size == 1),
            "Cannot operate on incompatible arguments");
    }

    template <typename LHS, typename RHS>
    constexpr auto operator+(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp{internal::ops::add<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr auto operator-(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp{internal::ops::subtract<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr auto operator*(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp{internal::ops::multiply<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr auto operator/(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp{internal::ops::divide<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr auto operator==(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp{internal::ops::eq<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr auto operator<(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp{internal::ops::less<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr auto operator<=(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp{internal::ops::lessEq<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr auto operator>(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp{internal::ops::greater<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr auto operator>=(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp{internal::ops::greaterEq<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr auto operator&&(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp{internal::ops::andOp<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr auto operator||(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp{internal::ops::orOp<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr auto min(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp{internal::ops::min<LHS, RHS>{}, lhs, rhs};
    }

    template <typename LHS, typename RHS>
    constexpr auto max(const LHS& lhs, const RHS& rhs) noexcept {
        checkTileMapCompatibility<LHS, RHS>();
        return BinaryOp{internal::ops::max<LHS, RHS>{}, lhs, rhs};
    }
} /* StealthTileMap */

#endif
