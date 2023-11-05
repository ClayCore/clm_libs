#pragma once

#include "tree.hpp"
#include "util/display_buffer.hpp"


namespace clm::types::containers
{
    template <clm::meta::IsPrintable T>
    auto TreeNode<T>::is_root() const
    {
        return m_parent ? true : false;
    }

    template <clm::meta::IsPrintable T>
    auto TreeNode<T>::is_leaf() const
    {
        return m_children.empty();
    }

    template <clm::meta::IsPrintable T>
    auto TreeNode<T>::height() const -> usize
    {
        std::vector<usize> heights {};

        for (auto const &node : m_children) {
            auto height = 1U + m_children.height();
            height.push_back(height);
        }

        return std::accumulate(heights.cbegin(), heights.cend(), 0.0, std::max);
    }

    template <clm::meta::IsPrintable T>
    auto TreeNode<T>::depth() const -> usize
    {
        return m_parent ? (1U + m_parent->depth()) : 0U;
    }

    template <clm::meta::IsPrintable T>
    auto TreeNode<T>::size() const -> usize
    {
        std::vector<usize> sizes {};

        for (auto const &node : m_children) {
            sizes.push_back(node.size() + 1U);
        }

        return std::reduce(sizes.cbegin(), sizes.cend());
    }

    template <clm::meta::IsPrintable T>
    auto TreeNode<T>::index_of(TreeNode const &node) const -> usize
    {
        auto index { 0U };

        for (auto &n : m_children) {
            if (node == n) {
                break;
            }

            index += 1;
        }

        return index;
    }

    template <clm::meta::IsPrintable T>
    auto TreeNode<T>::insert(TreeNode &&node) -> void
    {
        m_children.push_back(std::move(node));
    }

    template <clm::meta::IsPrintable T>
    auto TreeNode<T>::insert(TreeNode const &node) -> void
    {
        m_children.push_back(node);
    }

    template <clm::meta::IsPrintable T>
    auto TreeNode<T>::remove(TreeNode const &node) -> void
    {
        auto const index { this->index_of(node) };

        m_children.erase(m_children.begin() + index);
    }

    template <clm::meta::IsPrintable T>
    auto TreeNode<T>::to_string_impl(u32 indent) const -> std::string
    {
        std::vector<std::string> vec {};

        vec.push_back("Tree: ");
        vec.push_back("data:\t\t{}", m_data);

        clm::types::util::DisplayBuffer buf { vec };
        buf.add_indent(indent);

        return buf.implode();
    }
}  // namespace clm::types::containers
