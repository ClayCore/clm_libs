#pragma once

#include <array>
#include <string>
#include <vector>

#include "chunk.hpp"
#include "clm_libcpp_shared.hpp"
#include "meta.hpp"
#include "null_allocator.hpp"
#include "traits.hpp"
#include "utils.hpp"


namespace clm::alloc::detail
{
    /*****************************************************************************************
     * BitmappedBlockHelper
     ****************************************************************************************/
    template <class Instance>
    class BitmappedBlockHelper final
    {
    private:
        Instance &m_instance;

    public:
        explicit BitmappedBlockHelper(Instance &instance) noexcept;

        auto inline &get_allocator() noexcept;
        auto inline &get_bitmap() noexcept;
    };

    /*****************************************************************************************
     * BitmapHelper
     ****************************************************************************************/
    template <class Instance>
    class BitmapHelper
    {
    private:
        Instance &m_instance;

    public:
        explicit BitmapHelper(Instance &instance) noexcept;

        auto inline get_flag(usize index) noexcept -> byte &;
        auto constexpr flags_count() const noexcept -> usize;
    };

    /*****************************************************************************************
     * Bitmap
     ****************************************************************************************/
    template <usize Size>
    class Bitmap
    {
    private:
        using self_type = Bitmap<Size>;
        friend BitmapHelper<self_type>;

        byte static constexpr m_mask_all { 0b11111111 };
        std::array<byte, Size> m_flags {};

        auto static constexpr flag_mask(byte index) -> byte;

    public:
        auto inline set(usize index) -> void;

        auto inline reset(usize index) -> void;

        auto inline reset_all() -> void;

        auto inline is_set(usize index) -> bool;

        auto inline claim(usize &index) -> bool;
    };
}  // namespace clm::alloc::detail

namespace clm::alloc
{
    template <class Allocator, usize Min, usize Max, usize Capacity, usize Align>
    requires(is_power_of_two(Capacity) and Align > 0U)
    class BitmappedBlock
      : public traits::TAllocator<BitmappedBlock<Allocator, Min, Max, Capacity, Align>,
                                  Chunk>
    {
    public:
        using self_type      = BitmappedBlock<Allocator, Min, Max, Capacity, Align>;
        using trait          = traits::TAllocator<self_type, Chunk>;
        using allocator_type = Allocator;

        using bitmap_type =
            detail::Bitmap<((Capacity % 8U) == 0U ? (Capacity / 8) : (Capacity / 8 + 1))>;

        usize static constexpr min_size { Min };
        usize static constexpr max_size { Max };
        usize static constexpr aligned_size { align_size<Align>(Max) };
        usize static constexpr blocks_count { Capacity };

        auto alloc_impl(usize size) -> trait::block_type;
        auto owns_impl(trait::block_type const &block) const -> bool;
        auto free_impl(trait::block_type &block) -> void;

        auto to_string_impl(u32 indent = 0U) const -> std::string;

    private:
        friend detail::BitmappedBlockHelper<self_type>;

        Allocator m_allocator;
        Chunk m_alloc_chunk { NULL_CHUNK };
        bitmap_type *m_bitmap { nullptr };
    };
}  // namespace clm::alloc
