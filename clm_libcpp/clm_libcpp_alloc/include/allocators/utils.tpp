#pragma once

#include "utils.hpp"


namespace clm::alloc
{
    auto static constexpr is_power_of_two(usize value) noexcept -> bool
    {
        while (((value & 1U) == 0U) and value > 1) {
            value >>= 1U;
        }

        return value == 1U;
    }

    template <usize Align>
    requires(is_power_of_two(Align))
    auto constexpr align_front(byte *addr) noexcept -> byte *
    {
        auto ptr = reinterpret_cast<uptr>(addr);
        auto mod = ptr % Align;

        return (mod == 0U) ? addr : reinterpret_cast<byte *>(ptr + (Align - (mod)));
    }

    template <usize Align>
    requires(is_power_of_two(Align))
    auto constexpr align_back(byte *addr) noexcept -> byte *
    {
        auto ptr = reinterpret_cast<uptr>(addr);
        auto mod = ptr % Align;

        return (mod == 0U) ? addr : reinterpret_cast<byte *>(ptr - (mod));
    }

    template <usize Align>
    requires(is_power_of_two(Align))
    auto constexpr align_size(usize size) noexcept -> usize
    {
        auto mod = size % Align;

        return mod == 0U ? size : size + (Align - mod);
    }

    template <>
    auto constexpr align_front<NO_ALIGN>(byte *addr) noexcept -> byte *
    {
        return addr;
    }
    template <>
    auto constexpr align_back<NO_ALIGN>(byte *addr) noexcept -> byte *
    {
        return addr;
    }
    template <>
    auto constexpr align_size<NO_ALIGN>(usize size) noexcept -> usize
    {
        return size;
    }
}  // namespace clm::alloc
