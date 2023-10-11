#ifndef CLM_LIBCPP_TYPES_GUARD
#define CLM_LIBCPP_TYPES_GUARD

#include <ratio>

#include "clm_libcpp_shared.hpp"

namespace clm::types
{
    template <typename T, typename Tag, typename Ratio>
    class StrongTypeImpl
    {
    private:
        T m_value;

    public:
        explicit StrongTypeImpl(T const &value): m_value { value }
        {
        }
        explicit StrongTypeImpl(T &&value) noexcept: m_value { std::move(value) }
        {
        }

        operator T() const noexcept
        {
            return (m_value);
        }
        operator T() noexcept
        {
            return (m_value);
        }

        template <typename NewRatio>
        operator StrongTypeImpl<T, Tag, NewRatio>() const noexcept
        {
            auto constexpr prop_t = Ratio::num / Ratio::den;
            auto constexpr prop_u = NewRatio::den / NewRatio::num;

            return StrongTypeImpl<T, Tag, NewRatio>(m_value * (prop_t) * (prop_u));
        }
    };

    template <typename T, typename Tag>
    using StrongType = StrongTypeImpl<T, Tag, std::ratio<1>>;

    using Gram = StrongType<f64, struct GramTag>;
};     // namespace clm::types

#endif /* CLM_LIBCPP_TYPES_GUARD */
