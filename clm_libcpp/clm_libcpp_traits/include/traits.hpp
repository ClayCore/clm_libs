#ifndef CLM_LIBCPP_TRAITS_GUARD
#define CLM_LIBCPP_TRAITS_GUARD

#include <concepts>
#include <type_traits>
#include <vector>

#include "clm_libcpp_shared.hpp"

namespace clm::traits
{
    template <typename T>
    struct is_vector: std::false_type
    {
    };

    template <typename T>
    struct is_vector<std::vector<T>>: std::true_type
    {
    };

    template <typename T>
    bool constexpr const static is_vector_v = is_vector<T>::value;

    template <typename T>
    concept IsVector = is_vector_v<T>;

    template <typename T>
    concept IsContainer = requires(T t) {
        // clang-format off
        { std::begin(t) };
        { std::end(t)   };
        // clang-format on
    };
}  // namespace clm::traits

#endif  // CLM_LIBCPP_TRAITS_GUARD
