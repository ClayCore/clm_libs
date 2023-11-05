#pragma once

#include <string>

#include "clm_libcpp_shared.hpp"
#include "traits.hpp"


namespace alloc
{
    struct Chunk: clm::traits::TDisplay<Chunk>
    {
        void *ptr;
        usize size;

        constexpr Chunk() = default;
        explicit Chunk(void *p, usize s);

        constexpr operator byte *();

        auto constexpr operator<=>(Chunk const &) const        = default;
        auto constexpr operator==(Chunk const &) const -> bool = default;
        auto constexpr operator!=(Chunk const &) const -> bool = default;

        auto to_string_impl(u32 indent = 0U) const -> std::string;
    };
}  // namespace alloc

#include "chunk.tpp"
