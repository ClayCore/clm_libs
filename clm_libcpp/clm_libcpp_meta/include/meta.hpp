#pragma once

#include <iostream>
#include <string>

#include "clm_libcpp_shared.hpp"


namespace clm::meta
{
    template <typename T>
    concept IsPrintable = requires(T object) {
        // clang-format off
        { std::cout << object };
        // clang-format on
    };

    template <typename T>
    concept IsDisplay = requires(T object) {
        // clang-format off
        requires IsPrintable<T>;
        { std::cout << object.to_string() };
        { object.display() };
        // clang-format on
    };
}  // namespace clm::meta
