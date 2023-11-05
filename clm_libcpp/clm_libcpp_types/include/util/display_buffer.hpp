#pragma once

#include <algorithm>
#include <format>
#include <numeric>
#include <ranges>
#include <string>
#include <vector>

#include "clm_libcpp_shared.hpp"
#include "meta.hpp"


namespace clm::types::util
{
    struct DisplayBuffer
    {
        std::vector<std::string> buffer;

        template <clm::meta::RangeOver<std::string> Range>
        explicit DisplayBuffer(Range &&range);

        auto implode() const -> std::string;
        auto add_indent(u32 indent = 0U) -> void;
    };
}  // namespace clm::types::util
