#pragma once

#include <algorithm>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <vector>

#include "clm_libcpp_shared.hpp"


namespace clm::types::containers
{
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

        auto is_root() const;
        auto is_leaf() const;

        auto height() const -> usize;
        auto depth() const -> usize;
        auto size() const -> usize;

        auto index_of(TreeNode const &node) const -> usize;
        auto insert(TreeNode &&node) -> void;
        auto insert(TreeNode const &node) -> void;
        auto remove(TreeNode const &node) -> void;

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
};  // namespace clm::types::containers

#include "tree.tpp"
