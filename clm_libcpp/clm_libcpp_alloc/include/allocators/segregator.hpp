#pragma once

#include "chunk.hpp"
#include "clm_libcpp_shared.hpp"
#include "meta.hpp"
#include "null_allocator.hpp"
#include "traits.hpp"
#include "utils.hpp"


namespace clm::alloc
{
    template <usize Threshold, meta::IsAllocator Primary, meta::IsAllocator Secondary>
    class Segregator
      : public traits::TAllocator<Segregator<Threshold, Primary, Secondary>, Chunk>
    {
    private:
        Primary m_primary;
        Secondary m_secondary;

    public:
        using self_type = Segregator<Threshold, Primary, Secondary>;
        using trait     = traits::TAllocator<self_type, Chunk>;

        using primary_allocator   = Primary;
        using secondary_allocator = Secondary;

        auto inline get_primary() noexcept -> primary_allocator &;
        auto inline get_secondary() noexcept -> secondary_allocator &;

        auto alloc_impl(usize size) -> trait::block_type;
        auto owns_impl(trait::block_type const &block) const -> bool;
        auto free_impl(trait::block_type &block) -> void;

        auto to_string_impl(u32 indent = 0U) const -> std::string;
    };
}  // namespace clm::alloc

#include "segregator.tpp"
