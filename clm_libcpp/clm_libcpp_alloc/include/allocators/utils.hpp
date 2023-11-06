#pragma once

#include "clm_libcpp_shared.hpp"


namespace clm::alloc
{
#if defined(__amd64__) || defined(__arch64__)
    usize static constexpr WORD_ALIGN { 8U };
#elif defined(__i686__) || defined(__arm__)
    usize static constexpr WORD_ALIGN { 4U };
#else
#error Unsupported CPU architecture
#endif

    usize static constexpr NO_ALIGN { 1U };
    usize static constexpr HALF_WORD_ALIGN { WORD_ALIGN / 2U };

    auto static constexpr is_power_of_two(usize value) noexcept -> bool;

    template <usize Align>
    requires(is_power_of_two(Align))
    auto constexpr align_front(byte *addr) noexcept -> byte *;

    template <usize Align>
    requires(is_power_of_two(Align))
    auto constexpr align_back(byte *addr) noexcept -> byte *;

    template <usize Align>
    requires(is_power_of_two(Align))
    auto constexpr align_size(usize size) noexcept -> usize;

}  // namespace clm::alloc

#include "utils.tpp"
