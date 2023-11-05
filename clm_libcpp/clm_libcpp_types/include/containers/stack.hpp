#pragma once

#include <string>
#include <vector>

#include "clm_libcpp_shared.hpp"
#include "meta.hpp"
#include "traits.hpp"


namespace clm::types::containers
{
    template <clm::meta::IsPrintable T>
    class Stack: clm::traits::TDisplay<Stack<T>>
    {
    private:
        std::vector<T> m_elements;

    public:
        auto elements() const -> std::vector<T> const &;

        auto constexpr empty() const -> bool;

        auto push(T const &item) -> void;
        auto pop() -> void;

        auto top() -> T &;

        auto clear() -> void;

        auto to_string_impl(u32 indent = 0U) const -> std::string;
    };
}  // namespace clm::types::containers

#include "stack.tpp"
