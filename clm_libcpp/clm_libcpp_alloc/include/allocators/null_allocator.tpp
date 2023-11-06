#pragma once

#include <string>
#include <vector>

#include "null_allocator.hpp"


namespace clm::alloc
{
    auto NullAllocator::alloc_impl(usize) -> trait::block_type
    {
        return NULL_CHUNK;
    }

    auto NullAllocator::owns_impl(trait::block_type const &block) const -> bool
    {
        return (block.ptr == nullptr) and (block.size == 0U);
    }

    auto NullAllocator::free_impl(trait::block_type &block) -> void
    {
        block.ptr  = nullptr;
        block.size = 0U;
    }

    auto NullAllocator::to_string_impl(u32 indent) const -> std::string
    {
        std::vector<std::string> vec {};

        vec.push_back("NullAllocator");

        auto buf = types::util::DisplayBuffer { vec };
        buf.add_indent(indent);
        return buf.implode();
    }
}  // namespace clm::alloc
