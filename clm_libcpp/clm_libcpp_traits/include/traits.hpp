#pragma once

#include <chrono>
#include <concepts>
#include <iostream>
#include <type_traits>
#include <vector>

#include "clm_libcpp_shared.hpp"


namespace clm::traits
{
    /*****************************************************************************************
     * NonCopyable interface
     *
     * Derive to ensure a type cannot be copied or copy-assigned
     ****************************************************************************************/
    class INonCopyable
    {
    public:
        constexpr INonCopyable() = default;
        virtual ~INonCopyable()  = default;

        INonCopyable(INonCopyable const &)                     = delete;
        auto operator=(INonCopyable const &) -> INonCopyable & = delete;
    };

    /*****************************************************************************************
     * NonMovable interface
     *
     * Derive to ensure a type cannot be moved or move-assigned
     ****************************************************************************************/
    class INonMovable
    {
    public:
        constexpr INonMovable() = default;
        virtual ~INonMovable()  = default;

        INonMovable(INonMovable &&)                     = delete;
        auto operator=(INonMovable &&) -> INonMovable & = delete;
    };

    /*****************************************************************************************
     * NonCopyMovable interface
     *
     * Derive to ensure a type cannot be moved, copied, copy-assigned or move-assigned
     ****************************************************************************************/
    class INonCopyMovable
      : public INonCopyable
      , public INonMovable
    {
    };

    /*****************************************************************************************
     * Display trait
     *
     * Used for types that are meant to be displayed or logged nicely.
     ****************************************************************************************/
    template <class Derived>
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

    /*****************************************************************************************
     * Allocator trait
     *
     * Used as a common base type for memory allocators
     ****************************************************************************************/
    template <class Derived, class Block>
    class TAllocator
      : public TDisplay<TAllocator<Derived, Block>>
      , public INonCopyMovable
    {
    private:
        using base_type    = TAllocator<Derived, Block>;
        using derived_type = Derived;

        auto inline derived() -> derived_type &;
        auto inline derived() const -> derived_type const &;

    public:
        using allocator_type = derived_type;
        using block_type     = Block;

        auto alloc(usize size) -> block_type;
        auto owns(block_type const &block) const -> bool;
        auto free(block_type &block) -> void;

        auto to_string_impl(u32 indent = 0) const -> std::string;
    };
}  // namespace clm::traits

#include "traits.tpp"
