#pragma once

#include "chunk.hpp"
#include "util/display_buffer.hpp"


namespace clm::alloc
{
    constexpr Chunk::operator byte *()
    {
        return static_cast<byte *>(ptr);
    }

    constexpr Chunk::operator byte *() const
    {
        return static_cast<byte *>(ptr);
    }

    auto Chunk::to_string_impl(u32 indent) const -> std::string
    {
        std::vector<std::string> vec {};

        auto const addr = reinterpret_cast<uptr>(ptr);

        vec.push_back("Chunk: ");
        vec.push_back(std::format("address:\t0x{:x}", addr));
        vec.push_back(std::format("size:\t\t0x{:x}", size));

        auto buf = types::util::DisplayBuffer { vec };
        buf.add_indent(indent);

        return buf.implode();
    }
}  // namespace clm::alloc
