#pragma once

#include <concepts>
#include <format>
#include <iostream>
#include <ranges>
#include <string>
#include <type_traits>



#include "clm_libcpp_shared.hpp"


namespace clm::meta
{
    template <class R, class V>
    concept RangeOver =
        std::ranges::range<R> && std::same_as<std::ranges::range_value_t<R>, V>;

    template <class T>
    concept Formattable = requires(T &object, std::format_context &ctx) {
        std::formatter<std::remove_cvref_t<T>>().format(object, ctx);
    };

    template <class T>
    concept IsPrintable = requires(T object) {
        // clang-format off
        requires Formattable<T>;
        { std::cout << object };
        // clang-format on
    };

    template <class T>
    concept IsDisplay = requires(T object) {
        // clang-format off
        requires IsPrintable<T>;
        { std::cout << object.to_string() };
        { object.display() };
        // clang-format on
    };

    template <class T>
    concept IsAllocator = requires(T object) {
        // clang-format off
        requires IsDisplay<T>;
        { T::allocator_type };
        { T::block_type };
        { object.alloc(32) };
        // clang-format on
    };
}  // namespace clm::meta
