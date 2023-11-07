#pragma once

#include <format>

#include "segregator.hpp"


namespace clm::alloc
{
    template <usize Threshold, meta::IsAllocator Primary, meta::IsAllocator Secondary>
    auto inline Segregator<Threshold, Primary, Secondary>::get_primary() noexcept
        -> primary_allocator &
    {
        return m_primary;
    }

    template <usize Threshold, meta::IsAllocator Primary, meta::IsAllocator Secondary>
    auto inline Segregator<Threshold, Primary, Secondary>::get_secondary() noexcept
        -> secondary_allocator &
    {
        return m_secondary;
    }

    template <usize Threshold, meta::IsAllocator Primary, meta::IsAllocator Secondary>
    auto Segregator<Threshold, Primary, Secondary>::alloc_impl(usize size)
        -> trait::block_type
    {
        if (size <= Threshold) {
            return m_primary.alloc(size);
        } else {
            return m_secondary.alloc(size);
        }
    }

    template <usize Threshold, meta::IsAllocator Primary, meta::IsAllocator Secondary>
    auto Segregator<Threshold, Primary, Secondary>::owns_impl(
        trait::block_type const &block) const -> bool
    {
        return (block.size <= Threshold) ? m_primary.owns(block)
                                         : m_secondary.owns(block);
    }

    template <usize Threshold, meta::IsAllocator Primary, meta::IsAllocator Secondary>
    auto Segregator<Threshold, Primary, Secondary>::free_impl(trait::block_type &block)
        -> void
    {
        if (block.size <= Threshold) {
            m_primary.free(block);
        } else {
            m_secondary.free(block);
        }
    }

    template <usize Threshold, meta::IsAllocator Primary, meta::IsAllocator Secondary>
    auto Segregator<Threshold, Primary, Secondary>::to_string_impl(u32 indent) const
        -> std::string
    {
        std::vector<std::string> vec {};

        vec.push_back("Segregator: ");
        vec.push_back(std::format("threshold:\t{}", Threshold));
        vec.push_back(std::format("m_primary:\t{}", m_primary));
        vec.push_back(std::format("m_secondary:\t{}", m_secondary));

        auto buf = types::util::DisplayBuffer { vec };
        buf.add_indent(indent);

        return buf.implode();
    }
}  // namespace clm::alloc
