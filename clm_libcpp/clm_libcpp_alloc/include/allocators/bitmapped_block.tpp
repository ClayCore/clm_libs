#pragma once

#include <cstring>

#include <string>
#include <vector>

#include "bitmapped_block.hpp"
#include "chunk.hpp"


namespace clm::alloc::detail
{
    /*****************************************************************************************
     * BitmappedBlockHelper
     ****************************************************************************************/

    template <class Instance>
    BitmappedBlockHelper<Instance>::BitmappedBlockHelper(Instance &instance) noexcept
      : m_instance { instance }
    {
    }

    template <class Instance>
    auto inline &BitmappedBlockHelper<Instance>::get_allocator() noexcept
    {
        return m_instance.m_allocator;
    }

    template <class Instance>
    auto inline &BitmappedBlockHelper<Instance>::get_bitmap() noexcept
    {
        return *(m_instance.m_bitmap);
    }

    /*****************************************************************************************
     * BitmapHelper
     ****************************************************************************************/

    template <class Instance>
    BitmapHelper<Instance>::BitmapHelper(Instance &instance) noexcept
      : m_instance { instance }
    {
    }

    template <class Instance>
    auto inline BitmapHelper<Instance>::get_flag(usize index) noexcept -> byte &
    {
        return m_instance.m_flags[index];
    }

    template <class Instance>
    auto constexpr BitmapHelper<Instance>::flags_count() const noexcept -> usize
    {
        return sizeof(m_instance.m_flags);
    }

    /*****************************************************************************************
     * Bitmap
     ****************************************************************************************/

    template <usize Size>
    auto constexpr Bitmap<Size>::flag_mask(byte index) -> byte
    {
        // clang-format off
        byte flag_masks[8] = {
            0b10000000,
            0b01000000,
            0b00100000,
            0b00010000,
            0b00001000,
            0b00000100,
            0b00000010,
            0b00000001
        };
        // clang-format on

        return flag_masks[index];
    }

    template <usize Size>
    auto inline Bitmap<Size>::set(usize index) -> void
    {
        auto &flag = m_flags[index / 8U];
        flag |= this->flag_mask(index % 8U);
    }

    template <usize Size>
    auto inline Bitmap<Size>::reset(usize index) -> void
    {
        auto &flag = m_flags[index / 8U];
        flag &= ~(this->flag_mask(index % 8U));
    }

    template <usize Size>
    auto inline Bitmap<Size>::reset_all() -> void
    {
        std::memset(m_flags, 0U, Size);
    }

    template <usize Size>
    auto inline Bitmap<Size>::is_set(usize index) -> bool
    {
        auto &flag = m_flags[index % 8U];

        return (flag & this->flag_mask(index % 8U)) != 0U;
    }

    template <usize Size>
    auto inline Bitmap<Size>::claim(usize &index) -> bool
    {
        for (auto i = 0U; i < Size; ++i) {
            auto &flag = m_flags[i];

            if ((flag & m_mask_all) != m_mask_all) {
                for (auto j = 0U; j < 8U; ++j) {
                    auto mask = this->flag_mask(j);

                    if ((flag & mask) == 0U) {
                        flag |= mask;

                        index = i * 8U + j;
                        return true;
                    }
                }
            }
        }

        return false;
    }

}  // namespace clm::alloc::detail

namespace clm::alloc
{
    template <class Allocator, usize Min, usize Max, usize Capacity, usize Align>
    requires(is_power_of_two(Capacity) and Align > 0U)
    auto BitmappedBlock<Allocator, Min, Max, Capacity, Align>::alloc_impl(usize size)
        -> trait::block_type
    {
        if (size < min_size || size > max_size) {
            return NULL_CHUNK;
        }

        if (m_alloc_chunk == NULL_CHUNK) {
            auto alloc_size = sizeof(bitmap_type) + aligned_size * Capacity
                              + (Align > NO_ALIGN ? Align : 0U);
            m_alloc_chunk = m_allocator.alloc(alloc_size);

            m_bitmap = reinterpret_cast<bitmap_type *>(m_alloc_chunk.ptr);
            m_bitmap->reset_all();
        }

        usize index {};
        if (m_bitmap->claim(index) and index < Capacity) {
            byte const *ptr = m_alloc_chunk;
            auto first_addr = align_front<Align>(ptr + sizeof(bitmap_type));
            auto claim_addr = first_addr + aligned_size * index;

            return { claim_addr, aligned_size };
        } else {
            return NULL_CHUNK;
        }
    }

    template <class Allocator, usize Min, usize Max, usize Capacity, usize Align>
    requires(is_power_of_two(Capacity) and Align > 0U)
    auto BitmappedBlock<Allocator, Min, Max, Capacity, Align>::owns_impl(
        trait::block_type const &block) const -> bool
    {
        if (m_alloc_chunk == NULL_CHUNK) {
            return false;
        }

        byte const *ptr { m_alloc_chunk };
        byte const *start_addr { align_front<Align>(ptr + sizeof(bitmap_type)) };
        if (block < start_addr) {
            return false;
        }

        usize alloc_size { sizeof(bitmap_type) + aligned_size * Capacity + Align };

        byte const *end_addr { ptr + alloc_size };
        if (end_addr < block) {
            return false;
        }

        uptr const offset { block - start_addr };
        return (offset % aligned_size) == 0U;
    }
}  // namespace clm::alloc
