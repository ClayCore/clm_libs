#pragma once

#include "chunk.hpp"
#include "clm_libcpp_shared.hpp"
#include "meta.hpp"
#include "traits.hpp"


namespace clm::alloc
{
    struct NullAllocator: public traits::TAllocator<NullAllocator, Chunk>
    {
        using self_type      = NullAllocator;
        using trait          = traits::TAllocator<self_type, Chunk>;
        using allocator_type = self_type;

        auto alloc_impl(usize size) -> trait::block_type;
        auto owns_impl(trait::block_type const &block) const -> bool;
        auto free_impl(trait::block_type &block) -> void;

        auto to_string_impl(u32 indent = 0U) const -> std::string;
    };
}  // namespace clm::alloc

#include "null_allocator.tpp"
