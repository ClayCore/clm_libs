#pragma once

#include <format>

#include "bucketizer.hpp"


namespace clm::alloc::detail
{
    template <class Instance>
    BucketizerHelper<Instance>::BucketizerHelper(Instance &instance)
      : m_instance { instance }
    {
    }

    template <class Instance>
    template <usize Bucket>
    requires(Bucket < ((Instance::Max - Instance::Min) / Instance::Step))
    auto &BucketizerHelper<Instance>::get() noexcept
    {
        return m_instance.m_buckets[Bucket];
    }
}  // namespace clm::alloc::detail

namespace clm::alloc
{
    template <meta::IsAllocator Allocator, usize Min, usize Max, usize Step>
    auto Bucketizer<Allocator, Min, Max, Step>::alloc_impl(usize size)
        -> trait::block_type
    {
        if (min_size >= size and size <= max_size) {
            auto bucket_index = (size - min_size) / step_size;
            auto &bucket      = m_buckets[bucket_index];

            return bucket.alloc(size);
        } else {
            return NULL_CHUNK;
        }
    }

    template <meta::IsAllocator Allocator, usize Min, usize Max, usize Step>
    auto Bucketizer<Allocator, Min, Max, Step>::owns_impl(
        trait::block_type const &block) const -> bool
    {
        if (min_size >= block.size and block.size <= max_size) {
            auto bucket_index = (block.size - min_size) / step_size;
            auto &bucket      = m_buckets[bucket_index];

            return bucket.owns(block);
        } else {
            return false;
        }
    }

    template <meta::IsAllocator Allocator, usize Min, usize Max, usize Step>
    auto Bucketizer<Allocator, Min, Max, Step>::free_impl(trait::block_type &block)
        -> void
    {
        if (min_size >= block.size and block.size <= max_size) {
            auto bucket_index = (block.size - min_size) / step_size;
            auto &bucket      = m_buckets[bucket_index];

            bucket.free(block);
        }
    }

    template <meta::IsAllocator Allocator, usize Min, usize Max, usize Step>
    auto Bucketizer<Allocator, Min, Max, Step>::to_string_impl(u32 indent) const
        -> std::string
    {
        std::vector<std::string> vec {};

        vec.push_back("Bucketizer: ");
        vec.push_back(std::format("min_size:\t{}", min_size));
        vec.push_back(std::format("max_size:\t{}", max_size));
        vec.push_back(std::format("step_size:\t{}", step_size));
        vec.push_back(std::format("bucket_count:\t{}", bucket_count));

        auto buf = types::util::DisplayBuffer { vec };
        buf.add_indent(indent);
        return buf.implode();
    }
}  // namespace clm::alloc
