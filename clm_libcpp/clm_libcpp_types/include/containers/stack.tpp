#pragma once

#include "stack.hpp"
#include "util/display_buffer.hpp"

namespace clm::types::containers
{
    template <clm::meta::IsPrintable T>
    auto Stack<T>::elements() const -> std::vector<T> const &
    {
        return m_elements;
    }

    template <clm::meta::IsPrintable T>
    auto constexpr Stack<T>::empty() const -> bool
    {
        return m_elements.empty();
    }

    template <clm::meta::IsPrintable T>
    auto Stack<T>::push(T const &item) -> void
    {
        m_elements.push_back(item);
    }

    template <clm::meta::IsPrintable T>
    auto Stack<T>::pop() -> void
    {
        m_elements.pop_back();
    }

    template <clm::meta::IsPrintable T>
    auto Stack<T>::top() -> T &
    {
        return m_elements.back();
    }

    template <clm::meta::IsPrintable T>
    auto Stack<T>::clear() -> void
    {
        m_elements.clear();
    }

    template <clm::meta::IsPrintable T>
    auto Stack<T>::to_string_impl(u32 indent) const -> std::string
    {
        std::vector<std::string> vec {};

        vec.push_back("Stack: ");

        clm::types::util::DisplayBuffer buf { vec };
        buf.add_indent(indent);

        return buf.implode();
    }
}  // namespace clm::types::containers
