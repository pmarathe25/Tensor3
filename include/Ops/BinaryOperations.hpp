#ifndef BINARY_OPS_H
#define BINARY_OPS_H
#include "../OpStructs/BinaryOp.hpp"
#include "./InternalOperations.hpp"

namespace StealthTileMap {
    template <typename Derived, typename OtherDerived>
    constexpr void checkTileMapCompatibility() noexcept {
        static_assert(internal::traits<Derived>::length == internal::traits<OtherDerived>::length
            && internal::traits<Derived>::width == internal::traits<OtherDerived>::width
            && internal::traits<Derived>::height == internal::traits<OtherDerived>::height
            || (internal::traits<Derived>::size == 1 || internal::traits<OtherDerived>::size == 1),
            "Cannot operate on incompatible arguments");
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::add> operator+(const Derived& lhs, const OtherDerived& rhs) noexcept {
        checkTileMapCompatibility<Derived, OtherDerived>();
        return BinaryOp<Derived, OtherDerived, internal::ops::add>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::subtract> operator-(const Derived& lhs, const OtherDerived& rhs) noexcept {
        checkTileMapCompatibility<Derived, OtherDerived>();
        return BinaryOp<Derived, OtherDerived, internal::ops::subtract>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::multiply> operator*(const Derived& lhs, const OtherDerived& rhs) noexcept {
        checkTileMapCompatibility<Derived, OtherDerived>();
        return BinaryOp<Derived, OtherDerived, internal::ops::multiply>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::divide> operator/(const Derived& lhs, const OtherDerived& rhs) noexcept {
        checkTileMapCompatibility<Derived, OtherDerived>();
        return BinaryOp<Derived, OtherDerived, internal::ops::divide>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::eq> operator==(const Derived& lhs, const OtherDerived& rhs) noexcept {
        checkTileMapCompatibility<Derived, OtherDerived>();
        return BinaryOp<Derived, OtherDerived, internal::ops::eq>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::less> operator<(const Derived& lhs, const OtherDerived& rhs) noexcept {
        checkTileMapCompatibility<Derived, OtherDerived>();
        return BinaryOp<Derived, OtherDerived, internal::ops::less>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::lessEq> operator<=(const Derived& lhs, const OtherDerived& rhs) noexcept {
        checkTileMapCompatibility<Derived, OtherDerived>();
        return BinaryOp<Derived, OtherDerived, internal::ops::lessEq>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::greater> operator>(const Derived& lhs, const OtherDerived& rhs) noexcept {
        checkTileMapCompatibility<Derived, OtherDerived>();
        return BinaryOp<Derived, OtherDerived, internal::ops::greater>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::greaterEq> operator>=(const Derived& lhs, const OtherDerived& rhs) noexcept {
        checkTileMapCompatibility<Derived, OtherDerived>();
        return BinaryOp<Derived, OtherDerived, internal::ops::greaterEq>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::andOp> operator&&(const Derived& lhs, const OtherDerived& rhs) noexcept {
        checkTileMapCompatibility<Derived, OtherDerived>();
        return BinaryOp<Derived, OtherDerived, internal::ops::andOp>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::orOp> operator||(const Derived& lhs, const OtherDerived& rhs) noexcept {
        checkTileMapCompatibility<Derived, OtherDerived>();
        return BinaryOp<Derived, OtherDerived, internal::ops::orOp>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::min> min(const Derived& lhs, const OtherDerived& rhs) noexcept {
        checkTileMapCompatibility<Derived, OtherDerived>();
        return BinaryOp<Derived, OtherDerived, internal::ops::min>{lhs, rhs};
    }

    template <typename Derived, typename OtherDerived>
    constexpr BinaryOp<Derived, OtherDerived, internal::ops::max> max(const Derived& lhs, const OtherDerived& rhs) noexcept {
        checkTileMapCompatibility<Derived, OtherDerived>();
        return BinaryOp<Derived, OtherDerived, internal::ops::max>{lhs, rhs};
    }
} /* StealthTileMap */

#endif
