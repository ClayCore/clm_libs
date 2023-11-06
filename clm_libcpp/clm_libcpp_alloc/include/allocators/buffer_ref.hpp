#pragma once

#include <string>
#include <vector>

#include "chunk.hpp"
#include "clm_libcpp_shared.hpp"
#include "meta.hpp"
#include "traits.hpp"


namespace clm::alloc
{
    class BufferRef: traits::TDisplay<BufferRef>
    {
    private:
        void *m_data;
        usize m_length;

    public:
        using self_type = BufferRef;

        template <class Pointer>
        explicit BufferRef(Pointer *data, usize length);

        template <class Pointer>
        explicit BufferRef(Pointer const *data, usize length);

        template <class Pointer>
        explicit BufferRef(Pointer *data);

        explicit BufferRef(Chunk const &chunk);
        explicit BufferRef(std::string const &str);

        template <class Reference>
        requires std::is_reference_v<Reference>
        auto inline as_ref() -> Reference;

        template <class Pointer>
        requires std::is_pointer_v<Pointer>
        auto inline as_ptr() -> Pointer;

        auto inline length() const -> usize;

        auto inline sub_buf(usize length) const -> BufferRef;

        auto to_string_impl(u32 indent = 0U) const -> std::string;
    };
}  // namespace clm::alloc

#include "buffer_ref.tpp"
