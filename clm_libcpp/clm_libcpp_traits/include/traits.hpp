#pragma once

#include <chrono>
#include <concepts>
#include <iostream>
#include <type_traits>
#include <vector>

#include "clm_libcpp_shared.hpp"


namespace clm::traits
{
    template <typename Derived>
    class TDisplay
    {
    private:
        using base_type    = TDisplay<Derived>;
        using derived_type = Derived;

        auto inline derived() -> derived_type &;
        auto inline derived() const -> derived_type const &;

    public:
        constexpr TDisplay() = default;

        TDisplay(TDisplay const &)                     = default;
        auto operator=(TDisplay const &) -> TDisplay & = default;

        TDisplay(TDisplay &&)                     = default;
        auto operator=(TDisplay &&) -> TDisplay & = default;

        ~TDisplay() = default;

        auto to_string(u32 indent = 0U) const -> std::string;
        auto display(u32 indent = 0U) const -> void;

        auto constexpr operator<=>(TDisplay const &) const        = default;
        auto constexpr operator==(TDisplay const &) const -> bool = default;
        auto constexpr operator!=(TDisplay const &) const -> bool = default;

        auto friend inline operator<<(std::ostream &os, TDisplay const &display)
            -> std::ostream &
        {
            return os << display.to_string();
        }
    };
}  // namespace clm::traits

#include "traits.tpp"
