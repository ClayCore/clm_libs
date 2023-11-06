#pragma once

#include <format>

#include "fallback_allocator.hpp"


namespace clm::alloc
{
    template <meta::IsAllocator Primary, meta::IsAllocator Secondary>
    auto inline FallbackAllocator<Primary, Secondary>::get_primary()
        -> primary_allocator &
    {
        return m_primary;
    }

    template <meta::IsAllocator Primary, meta::IsAllocator Secondary>
    auto inline FallbackAllocator<Primary, Secondary>::get_secondary()
        -> secondary_allocator &
    {
        return m_secondary;
    }

    template <meta::IsAllocator Primary, meta::IsAllocator Secondary>
    auto FallbackAllocator<Primary, Secondary>::alloc_impl(usize size)
        -> trait::block_type
    {
        typename trait::block_type block = m_primary.alloc(size);
        if (block.ptr != nullptr) {
            return block;
        } else {
            return m_secondary.alloc(size);
        }
    }

    template <meta::IsAllocator Primary, meta::IsAllocator Secondary>
    auto FallbackAllocator<Primary, Secondary>::owns_impl(
        trait::block_type const &block) const -> bool
    {
        return m_primary.owns(block) or m_secondary.owns(block);
    }

    template <meta::IsAllocator Primary, meta::IsAllocator Secondary>
    auto FallbackAllocator<Primary, Secondary>::free_impl(trait::block_type &block)
        -> void
    {
        if (m_primary.owns(block)) {
            m_primary.free(block);
        } else {
            m_secondary.free(block);
        }
    }

    template <meta::IsAllocator Primary, meta::IsAllocator Secondary>
    auto FallbackAllocator<Primary, Secondary>::to_string_impl(u32 indent) const
        -> std::string
    {
        std::vector<std::string> vec {};

        vec.push_back("FallbackAllocator: ");
        vec.push_back(std::format("primary:\t{}", m_primary));
        vec.push_back(std::format("secondary:\t{}", m_secondary));

        auto buf = types::util::DisplayBuffer { vec };
        buf.add_indent(indent);

        return buf.implode();
    }
}  // namespace clm::alloc
