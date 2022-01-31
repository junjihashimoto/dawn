// Copyright 2020 The Dawn Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef COMMON_TYPEDINTEGER_H_
#define COMMON_TYPEDINTEGER_H_

#include "common/Assert.h"
#include "common/UnderlyingType.h"

#include <limits>
#include <type_traits>

// TypedInteger is helper class that provides additional type safety in Debug.
//  - Integers of different (Tag, BaseIntegerType) may not be used interoperably
//  - Allows casts only to the underlying type.
//  - Integers of the same (Tag, BaseIntegerType) may be compared or assigned.
// This class helps ensure that the many types of indices in Dawn aren't mixed up and used
// interchangably.
// In Release builds, when DAWN_ENABLE_ASSERTS is not defined, TypedInteger is a passthrough
// typedef of the underlying type.
//
// Example:
//     using UintA = TypedInteger<struct TypeA, uint32_t>;
//     using UintB = TypedInteger<struct TypeB, uint32_t>;
//
//  in Release:
//     using UintA = uint32_t;
//     using UintB = uint32_t;
//
//  in Debug:
//     using UintA = detail::TypedIntegerImpl<struct TypeA, uint32_t>;
//     using UintB = detail::TypedIntegerImpl<struct TypeB, uint32_t>;
//
//     Assignment, construction, comparison, and arithmetic with TypedIntegerImpl are allowed
//     only for typed integers of exactly the same type. Further, they must be
//     created / cast explicitly; there is no implicit conversion.
//
//     UintA a(2);
//     uint32_t aValue = static_cast<uint32_t>(a);
//
namespace detail {
    template <typename Tag, typename T>
    class TypedIntegerImpl;
}  // namespace detail

template <typename Tag, typename T, typename = std::enable_if_t<std::is_integral<T>::value>>
#if defined(DAWN_ENABLE_ASSERTS)
using TypedInteger = detail::TypedIntegerImpl<Tag, T>;
#else
using TypedInteger = T;
#endif

namespace detail {
    template <typename Tag, typename T>
    class alignas(T) TypedIntegerImpl {
        static_assert(std::is_integral<T>::value, "TypedInteger must be integral");
        T mValue;

      public:
        constexpr TypedIntegerImpl() : mValue(0) {
            static_assert(alignof(TypedIntegerImpl) == alignof(T));
            static_assert(sizeof(TypedIntegerImpl) == sizeof(T));
        }

        // Construction from non-narrowing integral types.
        template <typename I,
                  typename = std::enable_if_t<
                      std::is_integral<I>::value &&
                      std::numeric_limits<I>::max() <= std::numeric_limits<T>::max() &&
                      std::numeric_limits<I>::min() >= std::numeric_limits<T>::min()>>
        explicit constexpr TypedIntegerImpl(I rhs) : mValue(static_cast<T>(rhs)) {
        }

        // Allow explicit casts only to the underlying type. If you're casting out of an
        // TypedInteger, you should know what what you're doing, and exactly what type you
        // expect.
        explicit constexpr operator T() const {
            return static_cast<T>(this->mValue);
        }

// Same-tag TypedInteger comparison operators
#define TYPED_COMPARISON(op)                                        \
    constexpr bool operator op(const TypedIntegerImpl& rhs) const { \
        return mValue op rhs.mValue;                                \
    }
        TYPED_COMPARISON(<)
        TYPED_COMPARISON(<=)
        TYPED_COMPARISON(>)
        TYPED_COMPARISON(>=)
        TYPED_COMPARISON(==)
        TYPED_COMPARISON(!=)
#undef TYPED_COMPARISON

        // Increment / decrement operators for for-loop iteration
        constexpr TypedIntegerImpl& operator++() {
            ASSERT(this->mValue < std::numeric_limits<T>::max());
            ++this->mValue;
            return *this;
        }

        constexpr TypedIntegerImpl operator++(int) {
            TypedIntegerImpl ret = *this;

            ASSERT(this->mValue < std::numeric_limits<T>::max());
            ++this->mValue;
            return ret;
        }

        constexpr TypedIntegerImpl& operator--() {
            assert(this->mValue > std::numeric_limits<T>::min());
            --this->mValue;
            return *this;
        }

        constexpr TypedIntegerImpl operator--(int) {
            TypedIntegerImpl ret = *this;

            ASSERT(this->mValue > std::numeric_limits<T>::min());
            --this->mValue;
            return ret;
        }

        template <typename T2 = T>
        static constexpr std::enable_if_t<std::is_unsigned<T2>::value, decltype(T(0) + T2(0))>
        AddImpl(TypedIntegerImpl<Tag, T> lhs, TypedIntegerImpl<Tag, T2> rhs) {
            static_assert(std::is_same<T, T2>::value);

            // Overflow would wrap around
            ASSERT(lhs.mValue + rhs.mValue >= lhs.mValue);
            return lhs.mValue + rhs.mValue;
        }

        template <typename T2 = T>
        static constexpr std::enable_if_t<std::is_signed<T2>::value, decltype(T(0) + T2(0))>
        AddImpl(TypedIntegerImpl<Tag, T> lhs, TypedIntegerImpl<Tag, T2> rhs) {
            static_assert(std::is_same<T, T2>::value);

            if (lhs.mValue > 0) {
                // rhs is positive: |rhs| is at most the distance between max and |lhs|.
                // rhs is negative: (positive + negative) won't overflow
                ASSERT(rhs.mValue <= std::numeric_limits<T>::max() - lhs.mValue);
            } else {
                // rhs is postive: (negative + positive) won't underflow
                // rhs is negative: |rhs| isn't less than the (negative) distance between min
                // and |lhs|
                ASSERT(rhs.mValue >= std::numeric_limits<T>::min() - lhs.mValue);
            }
            return lhs.mValue + rhs.mValue;
        }

        template <typename T2 = T>
        static constexpr std::enable_if_t<std::is_unsigned<T>::value, decltype(T(0) - T2(0))>
        SubImpl(TypedIntegerImpl<Tag, T> lhs, TypedIntegerImpl<Tag, T2> rhs) {
            static_assert(std::is_same<T, T2>::value);

            // Overflow would wrap around
            ASSERT(lhs.mValue - rhs.mValue <= lhs.mValue);
            return lhs.mValue - rhs.mValue;
        }

        template <typename T2 = T>
        static constexpr std::enable_if_t<std::is_signed<T>::value, decltype(T(0) - T2(0))> SubImpl(
            TypedIntegerImpl<Tag, T> lhs,
            TypedIntegerImpl<Tag, T2> rhs) {
            static_assert(std::is_same<T, T2>::value);

            if (lhs.mValue > 0) {
                // rhs is positive: positive minus positive won't overflow
                // rhs is negative: |rhs| isn't less than the (negative) distance between |lhs|
                // and max.
                ASSERT(rhs.mValue >= lhs.mValue - std::numeric_limits<T>::max());
            } else {
                // rhs is positive: |rhs| is at most the distance between min and |lhs|
                // rhs is negative: negative minus negative won't overflow
                ASSERT(rhs.mValue <= lhs.mValue - std::numeric_limits<T>::min());
            }
            return lhs.mValue - rhs.mValue;
        }

        template <typename T2 = T>
        constexpr std::enable_if_t<std::is_signed<T2>::value, TypedIntegerImpl> operator-() const {
            static_assert(std::is_same<T, T2>::value);
            // The negation of the most negative value cannot be represented.
            ASSERT(this->mValue != std::numeric_limits<T>::min());
            return TypedIntegerImpl(-this->mValue);
        }

        constexpr TypedIntegerImpl operator+(TypedIntegerImpl rhs) const {
            auto result = AddImpl(*this, rhs);
            static_assert(std::is_same<T, decltype(result)>::value, "Use ityp::Add instead.");
            return TypedIntegerImpl(result);
        }

        constexpr TypedIntegerImpl operator-(TypedIntegerImpl rhs) const {
            auto result = SubImpl(*this, rhs);
            static_assert(std::is_same<T, decltype(result)>::value, "Use ityp::Sub instead.");
            return TypedIntegerImpl(result);
        }
    };

}  // namespace detail

namespace std {

    template <typename Tag, typename T>
    class numeric_limits<detail::TypedIntegerImpl<Tag, T>> : public numeric_limits<T> {
      public:
        static detail::TypedIntegerImpl<Tag, T> max() noexcept {
            return detail::TypedIntegerImpl<Tag, T>(std::numeric_limits<T>::max());
        }
        static detail::TypedIntegerImpl<Tag, T> min() noexcept {
            return detail::TypedIntegerImpl<Tag, T>(std::numeric_limits<T>::min());
        }
    };

}  // namespace std

namespace ityp {

    // These helpers below are provided since the default arithmetic operators for small integer
    // types like uint8_t and uint16_t return integers, not their same type. To avoid lots of
    // casting or conditional code between Release/Debug. Callsites should use ityp::Add(a, b) and
    // ityp::Sub(a, b) instead.

    template <typename Tag, typename T>
    constexpr ::detail::TypedIntegerImpl<Tag, T> Add(::detail::TypedIntegerImpl<Tag, T> lhs,
                                                     ::detail::TypedIntegerImpl<Tag, T> rhs) {
        return ::detail::TypedIntegerImpl<Tag, T>(
            static_cast<T>(::detail::TypedIntegerImpl<Tag, T>::AddImpl(lhs, rhs)));
    }

    template <typename Tag, typename T>
    constexpr ::detail::TypedIntegerImpl<Tag, T> Sub(::detail::TypedIntegerImpl<Tag, T> lhs,
                                                     ::detail::TypedIntegerImpl<Tag, T> rhs) {
        return ::detail::TypedIntegerImpl<Tag, T>(
            static_cast<T>(::detail::TypedIntegerImpl<Tag, T>::SubImpl(lhs, rhs)));
    }

    template <typename T>
    constexpr std::enable_if_t<std::is_integral<T>::value, T> Add(T lhs, T rhs) {
        return static_cast<T>(lhs + rhs);
    }

    template <typename T>
    constexpr std::enable_if_t<std::is_integral<T>::value, T> Sub(T lhs, T rhs) {
        return static_cast<T>(lhs - rhs);
    }

}  // namespace ityp

#endif  // COMMON_TYPEDINTEGER_H_
