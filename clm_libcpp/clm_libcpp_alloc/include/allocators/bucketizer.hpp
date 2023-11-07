#pragma once

#include <array>
#include <string>

#include "chunk.hpp"
#include "clm_libcpp_shared.hpp"
#include "meta.hpp"
#include "traits.hpp"
#include "utils.hpp"


namespace clm::alloc::detail
{
    template <class Instance>
    class BucketizerHelper final
    {
    private:
        Instance &m_instance;

    public:
        explicit BucketizerHelper(Instance &instance);

        template <usize Bucket>
        requires(Bucket < ((Instance::Max - Instance::Min) / Instance::Step))
        auto &get() noexcept;
    };
}  // namespace clm::alloc::detail

namespace clm::alloc
{
    template <meta::IsAllocator Allocator, usize Min, usize Max, usize Step>
    class Bucketizer
      : public traits::TAllocator<Bucketizer<Allocator, Min, Max, Step>, Chunk>
    {
    private:
        using self_type_priv = Bucketizer<Allocator, Min, Max, Step>;
        friend detail::BucketizerHelper<self_type_priv>;

        std::array<Allocator, (Max - Min) / Step> m_buckets;

    public:
        using self_type      = self_type_priv;
        using trait          = traits::TAllocator<self_type, Chunk>;
        using allocator_type = Allocator;

        usize static constexpr min_size { Min };
        usize static constexpr max_size { Max };
        usize static constexpr step_size { Step };
        usize static constexpr bucket_count { (max_size - min_size) / step_size };

        auto alloc_impl(usize size) -> trait::block_type;
        auto owns_impl(trait::block_type const &block) const -> bool;
        auto free_impl(trait::block_type &block) -> void;

        auto to_string_impl(u32 indent = 0U) const -> std::string;
    };
}  // namespace clm::alloc

#include "bucketizer.tpp"
