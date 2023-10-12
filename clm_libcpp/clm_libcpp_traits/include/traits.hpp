#ifndef CLM_LIBCPP_TRAITS_GUARD
#define CLM_LIBCPP_TRAITS_GUARD

#include <chrono>
#include <concepts>
#include <type_traits>
#include <vector>

#include "clm_libcpp_shared.hpp"

namespace clm::traits
{


    /*
    template <typename T>
    struct is_vector: std::false_type
    {
    };

    template <typename T>
    struct is_vector<std::vector<T>>: std::true_type
    {
    };

    template <typename T>
    struct is_duration: std::false_type
    {
    };

    template <typename R, typename P>
    struct is_duration<std::chrono::duration<R, P>>: std::true_type
    {
    };

    template <typename T>
    bool constexpr const static is_vector_v = is_vector<T>::value;

    template <typename T>
    bool constexpr const static is_duration_v = is_duration<T>::value;

    template <typename T>
    concept IsVector = is_vector_v<T>;

    template <typename T>
    concept IsContainer = requires(T t) {
        // clang-format off
        { std::begin(t)                };
        { std::begin(t)++              };
        { std::end(t)                  };
        { *std::begin(t)               };
        { *std::begin(t)               } -> std::same_as<void>;
        { std::begin(t) != std::end(t) } -> std::same_as<bool>;
        std::destructible<decltype(std::begin(t))> &&
        std::destructible<decltype(std::end(t))> &&
        std::copy_constructible<decltype(std::begin(t))> &&
        std::copy_constructible<decltype(std::end(t))>;
        // clang-format on
    };

    template <typename D, typename B>
    concept IsDerivedFrom = std::derived_from<B, D>;
    */
}  // namespace clm::traits

#endif  // CLM_LIBCPP_TRAITS_GUARD
