#ifndef CLM_LIBCPP_TYPES_GUARD
#define CLM_LIBCPP_TYPES_GUARD

#include <algorithm>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <vector>

#include "clm_libcpp_shared.hpp"

namespace clm::types
{
    class Debug
    {
    public:
        virtual ~Debug() = default;

        virtual auto to_string() const -> std::string = 0;

        friend auto inline operator<<(std::ostream &os, Debug const &dbg)
            -> std::ostream &
        {
            return os << dbg.to_string();
        }
    };

    template <typename T>
    class TreeNode
    {
    private:
        TreeNode *m_parent { nullptr };
        std::vector<TreeNode> m_children {};
        T m_data;

    public:
        explicit TreeNode(TreeNode *parent): m_parent { parent }
        {
        }

        auto is_root() const
        {
            return (m_parent ? true : false);
        }

        auto is_leaf() const
        {
            return (m_children.empty());
        }

        auto insert(TreeNode &&node) -> void
        {
            m_children.emplace_back(std::move(node));
        }

        auto insert(TreeNode const &node) -> void
        {
            m_children.push_back(node);
        }

        auto remove(TreeNode const &node) -> void
        {
            auto const index { this->index_of(node) };

            m_children.erase(m_children.begin() + index);
        }

        auto index_of(TreeNode const &node) const -> usize
        {
            auto index { 0U };

            for (auto &n : m_children) {
                if (node == n) {
                    break;
                }

                index += 1;
            }

            return (index);
        }

        auto height() const -> usize
        {
            std::vector<usize> heights {};

            for (auto const &node : m_children) {
                auto height = 1U + m_children.height();
                height.push_back(height);
            }

            return (std::accumulate(heights.cbegin(), heights.cend(), 0.0, std::max));
        }

        auto depth() const -> usize
        {
            return m_parent ? (1U + m_parent->depth()) : 0U;
        }

        auto size() const -> usize
        {
            std::vector<usize> sizes {};

            for (auto const &node : m_children) {
                sizes.push_back(node.size() + 1U);
            }

            return (std::reduce(sizes.cbegin(), sizes.cend()));
        }

        template <typename U>
        auto static ancestor(TreeNode<U> *first, TreeNode<U> *second) -> TreeNode<U> *
        {
            if (!first || !second) {
                return (nullptr);
            }

            if (*first == *second) {
                return (first);
            }

            auto const p = first->m_parent;
            auto const q = second->m_parent;

            if (!p || first == q || (!p->m_parent && q)) {
                return (TreeNode<U>::ancestor(first, q));
            } else if (!q || second == p || (!q->m_parent && p)) {
                return (TreeNode<U>::ancestor(p, second));
            }

            return TreeNode<U>::ancestor(p, q);
        }
    };
};      // namespace clm::types

#endif  // CLM_LIBCPP_TYPES_GUARD
