#pragma once

#include "chunk.hpp"
#include "clm_libcpp_shared.hpp"
#include "meta.hpp"
#include "traits.hpp"


namespace clm::alloc
{
    template <meta::IsAllocator Primary, meta::IsAllocator Secondary>
    class FallbackAllocator
      : public traits::TAllocator<FallbackAllocator<Primary, Secondary>, Chunk>
    {
    private:
        Primary m_primary;
        Secondary m_secondary;

    public:
        using self_type      = FallbackAllocator<Primary, Secondary>;
        using trait          = traits::TAllocator<self_type, Chunk>;
        using allocator_type = self_type;

        using primary_allocator   = Primary;
        using secondary_allocator = Secondary;

        auto inline get_primary() -> primary_allocator &;
        auto inline get_secondary() -> secondary_allocator &;

        auto alloc_impl(usize size) -> trait::block_type;
        auto owns_impl(trait::block_type const &block) const -> bool;
        auto free_impl(trait::block_type &block) -> void;

        auto to_string_impl(u32 indent = 0U) const -> std::string;
    };
}  // namespace clm::alloc

#include "fallback_allocator.tpp"
