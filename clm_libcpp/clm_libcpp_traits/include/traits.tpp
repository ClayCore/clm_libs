#pragma once

#include "meta.hpp"
#include "traits.hpp"


namespace clm::traits
{
    template <typename Derived>
    auto inline TDisplay<Derived>::derived() -> derived_type &
    {
        return static_cast<derived_type &>(*this);
    }

    template <typename Derived>
    auto inline TDisplay<Derived>::derived() const -> derived_type const &
    {
        return static_cast<derived_type const &>(*this);
    }

    template <typename Derived>
    auto TDisplay<Derived>::to_string(u32 indent) const -> std::string
    {
        return this->derived().to_string_impl(indent);
    }

    template <typename Derived>
    auto TDisplay<Derived>::display(u32 indent) const -> void
    {
        std::cout << this->to_string(indent);
    }
}  // namespace clm::traits

template <clm::meta::IsDisplay T, typename CharT>
struct std::formatter<T, CharT>: public std::formatter<std::string>
{
    template <class FmtCtx>
    auto format(T const &object, FmtCtx &ctx) const
    {
        return std::formatter<std::string>::format(object.to_string(), ctx);
    }
};
