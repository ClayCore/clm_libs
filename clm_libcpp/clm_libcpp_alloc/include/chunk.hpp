#pragma once

#include <string>

#include "clm_libcpp_shared.hpp"
#include "traits.hpp"


namespace clm::alloc
{
    struct Chunk: traits::TDisplay<Chunk>
    {
        using self_type = Chunk;

        void *ptr;
        usize size;

        constexpr Chunk() = default;
        explicit constexpr Chunk(void *p, usize s): ptr { p }, size { s }
        {
        }

        constexpr operator byte *();
        constexpr operator byte *() const;

        auto constexpr operator<=>(Chunk const &) const        = default;
        auto constexpr operator==(Chunk const &) const -> bool = default;
        auto constexpr operator!=(Chunk const &) const -> bool = default;

        auto to_string_impl(u32 indent = 0U) const -> std::string;
    };

    Chunk static constexpr NULL_CHUNK { nullptr, 0U };
}  // namespace clm::alloc

#include "chunk.tpp"
