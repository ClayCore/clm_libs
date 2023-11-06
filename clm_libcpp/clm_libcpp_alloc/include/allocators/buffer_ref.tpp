#pragma once

#include <format>

#include "buffer_ref.hpp"


namespace clm::alloc
{
    template <class Pointer>
    BufferRef::BufferRef(Pointer *data, usize length)
      : m_data { reinterpret_cast<void *>(data) }, m_length { length }
    {
    }

    template <class Pointer>
    BufferRef::BufferRef(Pointer const *data, usize length)
      : m_data { reinterpret_cast<void *>(const_cast<Pointer *>(data)) }
      , m_length { length }
    {
    }

    template <class Pointer>
    BufferRef::BufferRef(Pointer *data): BufferRef { data, sizeof(Pointer) }
    {
    }

    BufferRef::BufferRef(Chunk const &chunk): BufferRef { chunk.ptr, chunk.size }
    {
    }

    BufferRef::BufferRef(std::string const &str)
      : BufferRef { reinterpret_cast<void *>(const_cast<char *>(str.data())),
                    str.length() }
    {
    }

    template <class Reference>
    requires std::is_reference_v<Reference>
    auto inline BufferRef::as_ref() -> Reference
    {
        using base_type    = std::remove_reference_t<Reference>;
        using pointer_type = std::add_pointer_t<base_type>;

        return *reinterpret_cast<pointer_type>(m_data);
    }

    template <class Pointer>
    requires std::is_pointer_v<Pointer>
    auto inline BufferRef::as_ptr() -> Pointer
    {
        return reinterpret_cast<Pointer>(m_data);
    }

    auto inline BufferRef::length() const -> usize
    {
        return m_length;
    }

    auto inline BufferRef::sub_buf(usize length) const -> BufferRef
    {
        return BufferRef { m_data, length };
    }

    auto BufferRef::to_string_impl(u32 indent) const -> std::string
    {
        std::vector<std::string> vec {};

        vec.push_back("BufferRef: ");
        vec.push_back(std::format("length:\t{}", m_length));
        vec.push_back(std::format("data:\t\t{}", (m_data) ? "present" : "<null>"));

        auto buf = types::util::DisplayBuffer { vec };
        buf.add_indent(indent);

        return buf.implode();
    }
}  // namespace clm::alloc
