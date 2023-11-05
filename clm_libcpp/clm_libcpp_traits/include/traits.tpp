#pragma once

#include "meta.hpp"
#include "traits.hpp"


namespace clm::traits
{
    /* #region TDisplayImplementation */

    template <class Derived>
    auto inline TDisplay<Derived>::derived() -> derived_type &
    {
        return static_cast<derived_type &>(*this);
    }

    template <class Derived>
    auto inline TDisplay<Derived>::derived() const -> derived_type const &
    {
        return static_cast<derived_type const &>(*this);
    }

    template <class Derived>
    auto TDisplay<Derived>::to_string(u32 indent) const -> std::string
    {
        return this->derived().to_string_impl(indent);
    }

    template <class Derived>
    auto TDisplay<Derived>::display(u32 indent) const -> void
    {
        std::cout << this->to_string(indent);
    }

    /* #endregion TDisplayImplementation */

    /* #region TAllocatorImplementation */

    template <class D, class Block>
    auto inline TAllocator<D, Block>::derived() -> derived_type &
    {
        return static_cast<derived_type &>(*this);
    }

    template <class D, class Block>
    auto inline TAllocator<D, Block>::derived() const -> derived_type const &
    {
        return static_cast<derived_type const &>(*this);
    }

    template <class D, class Block>
    auto TAllocator<D, Block>::alloc(usize size) -> block_type
    {
        return this->derived().alloc_impl(size);
    }

    template <class D, class Block>
    auto TAllocator<D, Block>::owns(block_type const &block) const -> bool
    {
        return this->derived().owns_impl(block);
    }

    template <class D, class Block>
    auto TAllocator<D, Block>::free(block_type &block) -> void
    {
        this->derived().free_impl(block);
    }

    template <class D, class Block>
    auto TAllocator<D, Block>::to_string_impl(u32 indent) const -> std::string
    {
        return this->derived().to_string(indent);
    }

    /* #endregion TAllocatorImplementation */

}  // namespace clm::traits

template <clm::meta::IsDisplay T, class CharT>
struct std::formatter<T, CharT>: public std::formatter<std::string>
{
    template <class FmtCtx>
    auto format(T const &object, FmtCtx &ctx) const
    {
        return std::formatter<std::string>::format(object.to_string(), ctx);
    }
};
