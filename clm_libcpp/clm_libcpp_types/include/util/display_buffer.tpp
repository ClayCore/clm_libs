#pragma once

#include "display_buffer.hpp"

namespace clm::types::util
{
    template <clm::meta::RangeOver<std::string> Range>
    DisplayBuffer::DisplayBuffer(Range &&range): buffer { std::move(range) }
    {
    }

    auto DisplayBuffer::implode() const -> std::string
    {
        auto concat = [](auto const &left, auto const &right) {
            return (left + "\n" + right);
        };

        return std::accumulate(++buffer.begin(), buffer.end(), *buffer.begin(), concat);
    }

    auto DisplayBuffer::add_indent(u32 indent) -> void
    {
        for (auto it = buffer.begin(); it != buffer.end(); ++it) {
            auto idx         = std::distance(buffer.begin(), it);
            auto const count = (idx != 0U) ? (indent + 2) : (indent);

            it->insert(it->begin(), count, ' ');
        }
    }
}  // namespace clm::types::util
