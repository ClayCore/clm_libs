#ifndef CLM_LIBCPP_UTILS_GUARD
#define CLM_LIBCPP_UTILS_GUARD

#include <bits/utility.h>
#include <type_traits>
#include <utility>

#include "clm_libcpp_shared.hpp"

namespace clm::utils::meta
{
    /* -------------------------------------------------------------------------------- *
     * identity_type
     * -------------------------------------------------------------------------------- */

    template <typename T>
    struct identity_type
    {
        using type = T;
    };

    template <typename T>
    using identity_type_t = typename identity_type<T>::type;

    /* -------------------------------------------------------------------------------- *
     * void_t
     * -------------------------------------------------------------------------------- */

    template <typename...>
    using void_t = void;

    /* -------------------------------------------------------------------------------- *
     * integral_constant
     * -------------------------------------------------------------------------------- */

    template <typename T, T val>
    struct integral_constant
    {
        using base_type  = integral_constant<T, val>;
        using value_type = T;
        using type       = T;

        T constexpr static value = val;

        constexpr operator value_type() const
        {
            return (value);
        }
        constexpr auto operator()() const -> value_type
        {
            return (value);
        }
    };

    template <typename T, T val>
    using integral_constant_v = integral_constant<T, val>;

    using false_type = integral_constant_v<bool, false>;
    using true_type  = integral_constant_v<bool, true>;

    /* -------------------------------------------------------------------------------- *
     * add_lvalue_ref, add_rvalue_ref
     * -------------------------------------------------------------------------------- */

    template <typename T, typename = void>
    struct add_ref
    {
        using lvalue = T;
        using rvalue = T;
    };

    template <typename T>
    struct add_ref<T, void_t<T &>>
    {
        using lvalue = T &;
        using rvalue = T &&;
    };

    template <typename T>
    using add_lvalue_ref = typename add_ref<T>::lvalue;

    template <typename T>
    using add_rvalue_ref = typename add_ref<T>::rvalue;

    /* -------------------------------------------------------------------------------- *
     * add_const, add_const_ref_type
     * -------------------------------------------------------------------------------- */

    template <typename T>
    using add_const_t = const T;

    template <typename T>
    struct add_const_ref_type
    {
        using type = T;
    };

    template <typename T>
    struct add_const_ref_type<T &>
    {
        using type = add_const_t<T> &;
    };

    template <typename T>
    struct add_const_ref_type<T &&>
    {
        using type = add_const_t<T> &&;
    };

    template <typename T>
    using add_const_ref_type_t = typename add_const_ref_type<T>::type;

    /* -------------------------------------------------------------------------------- *
     * remove_const
     * -------------------------------------------------------------------------------- */

    template <typename T>
    struct remove_const
    {
        using type = T;
    };

    template <typename T>
    struct remove_const<T const>
    {
        using type = T;
    };

    template <typename T>
    using remove_const_t = typename remove_const<T>::type;

    /* -------------------------------------------------------------------------------- *
     * remove_volatile
     * -------------------------------------------------------------------------------- */

    template <typename T>
    struct remove_volatile
    {
        using type = T;
    };

    template <typename T>
    struct remove_volatile<T volatile>
    {
        using type = T;
    };

    template <typename T>
    using remove_volatile_t = typename remove_volatile<T>::type;

    /* -------------------------------------------------------------------------------- *
     * remove_cv
     * -------------------------------------------------------------------------------- */

    template <typename T>
    using remove_cv_t = remove_volatile_t<remove_const_t<T>>;

    /* -------------------------------------------------------------------------------- *
     * remove_ref
     * -------------------------------------------------------------------------------- */

    template <typename T>
    struct remove_ref
    {
        using type = T;
    };

    template <typename T>
    struct remove_ref<T &>
    {
        using type = T;
    };

    template <typename T>
    struct remove_ref<T &&>
    {
        using type = T;
    };

    template <typename T>
    using remove_ref_t = typename remove_ref<T>::type;

    template <typename T>
    using remove_cv_ref_t = remove_cv_t<remove_ref<T>>::type;

    /* -------------------------------------------------------------------------------- *
     * is_lvalue_ref, is_rvalue_ref
     * -------------------------------------------------------------------------------- */

    template <typename T>
    bool constexpr inline is_lvalue_ref = false;

    template <typename T>
    bool constexpr inline is_lvalue_ref<T &> = true;

    template <typename T>
    bool constexpr inline is_rvalue_ref = false;

    template <typename T>
    bool constexpr inline is_rvalue_ref<T &&> = true;

    /* -------------------------------------------------------------------------------- *
     * is_pointer
     * -------------------------------------------------------------------------------- */

    namespace detail
    {
        template <typename T>
        bool constexpr inline is_pointer = false;

        template <typename T>
        bool constexpr inline is_pointer<T *> = true;
    }  // namespace detail

    template <typename T>
    bool constexpr inline is_pointer = detail::is_pointer<remove_cv_t<T>>;

    /* -------------------------------------------------------------------------------- *
     * remove_pointer
     * -------------------------------------------------------------------------------- */

    template <typename T>
    struct remove_pointer
    {
        using type = T;
    };

    template <typename T>
    struct remove_pointer<T *>
    {
        using type = T;
    };

    template <typename T>
    struct remove_pointer<T const *>
    {
        using type = T;
    };

    template <typename T>
    struct remove_pointer<T volatile *>
    {
        using type = T;
    };

    template <typename T>
    struct remove_pointer<T const volatile *>
    {
        using type = T;
    };

    template <typename T>
    using remove_pointer_t = remove_pointer<T>::type;

    /* -------------------------------------------------------------------------------- *
     * is_const
     * -------------------------------------------------------------------------------- */

    template <typename T>
    bool constexpr inline is_const = false;

    template <typename T>
    bool constexpr inline is_const<T const> = true;

    /* -------------------------------------------------------------------------------- *
     * is_same
     * -------------------------------------------------------------------------------- */

    template <typename T, typename U>
    bool constexpr inline is_same = false;

    template <typename T>
    bool constexpr inline is_same<T, T> = true;

    /* -------------------------------------------------------------------------------- *
     * conditional
     * -------------------------------------------------------------------------------- */

    template <bool cond, typename T, typename F>
    struct conditional: true_type
    {
    };

    template <typename T, typename F>
    struct conditional<false, T, F>: false_type
    {
    };

    template <bool cond, typename T, typename F>
    using conditional_t = typename conditional<cond, T, F>::type;

    /* -------------------------------------------------------------------------------- *
     * copy_const
     * -------------------------------------------------------------------------------- */

    template <typename Ref, typename T>
    using copy_const = conditional_t<is_const<Ref>, add_const_t<T>, remove_const<T>>;

    /* -------------------------------------------------------------------------------- *
     * is_nullptr
     * -------------------------------------------------------------------------------- */

    template <typename T>
    bool constexpr inline is_nullptr = is_same<decltype(nullptr), remove_cv_t<T>>;

    /* -------------------------------------------------------------------------------- *
     * is_function
     * -------------------------------------------------------------------------------- */

    template <typename>
    bool constexpr inline is_function = false;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args...)> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args...) &> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args...) &&> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args...) const> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args...) const &> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args...) const &&> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args...) volatile> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args...) volatile &> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args...) volatile &&> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args...) const volatile> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args...) const volatile &> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args...) const volatile &&> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args..., ...)> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args..., ...) &> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args..., ...) &&> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args..., ...) const> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args..., ...) const &> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args..., ...) const &&> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args..., ...) volatile> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args..., ...) volatile &> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args..., ...) volatile &&> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args..., ...) const volatile> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args..., ...) const volatile &> = true;

    template <typename Ret, typename... Args>
    bool constexpr inline is_function<Ret(Args..., ...) const volatile &&> = true;

    /* -------------------------------------------------------------------------------- *
     * make_unsigned
     * -------------------------------------------------------------------------------- */

    // clang-format off
    template <typename T>
    struct make_unsigned           { using type = void; };

    template<>
    struct make_unsigned<i8>       { using type = u8; };

    template <>
    struct make_unsigned<i16>      { using type = u16; };

    template <>
    struct make_unsigned<i32>      { using type = u32; };

    template <>
    struct make_unsigned<i64>      { using type = u64; };

    template <>
    struct make_unsigned<u8>       { using type = u8; };

    template <>
    struct make_unsigned<u16>      { using type = u16;};

    template <>
    struct make_unsigned<u32>      { using type = u32; };

    template <>
    struct make_unsigned<u64>      { using type = u64; };

    template <>
    struct make_unsigned<char>     { using type = unsigned char; };

    template <>
    struct make_unsigned<char8_t>  { using type = char8_t; };

    template <>
    struct make_unsigned<char16_t> { using type = char16_t; };

    template <>
    struct make_unsigned<char32_t> { using type = char32_t; };

    template <>
    struct make_unsigned<bool>     { using type = bool; };
    // clang-format on

    template <typename T>
    using make_unsigned_t = typename make_unsigned<T>::type;

    /* -------------------------------------------------------------------------------- *
     * make_signed
     * -------------------------------------------------------------------------------- */

    // clang-format off
    template <typename T>
    struct make_signed           { using type = void; };

    template<>
    struct make_signed<i8>       { using type = i8; };

    template <>
    struct make_signed<i16>      { using type = i16; };

    template <>
    struct make_signed<i32>      { using type = i32; };

    template <>
    struct make_signed<i64>      { using type = i64; };

    template <>
    struct make_signed<u8>       { using type = i8; };

    template <>
    struct make_signed<u16>      { using type = i16;};

    template <>
    struct make_signed<u32>      { using type = i32; };

    template <>
    struct make_signed<u64>      { using type = i64; };

    template <>
    struct make_signed<char>     { using type = signed char; };

    template <>
    struct make_signed<char8_t>  { using type = char8_t; };

    template <>
    struct make_signed<char16_t> { using type = char16_t; };

    template <>
    struct make_signed<char32_t> { using type = char32_t; };

    template <>
    struct make_signed<bool>     { using type = bool; };
    // clang-format on

    template <typename T>
    using make_signed_t = typename make_signed<T>::type;

    /* -------------------------------------------------------------------------------- *
     * common_type
     * -------------------------------------------------------------------------------- */

    template <typename...>
    struct common_type;

    template <typename T>
    struct common_type<T>
    {
        using type = T;
    };

    template <typename T1, typename T2>
    struct common_type<T1, T2>
    {
        using type = decltype(true ? std::declval<T1>() : std::declval<T2>());
    };

    template <typename T1, typename T2, typename... Ts>
    struct common_type<T1, T2, Ts...>
    {
        using type =
            typename common_type<typename common_type<T1, T2>::type, Ts...>::type;
    };

    template <typename... Ts>
    using common_type_t = typename common_type<Ts...>::type;

    /* -------------------------------------------------------------------------------- *
     * intrinsic type checks
     * -------------------------------------------------------------------------------- */

    template <typename T>
    bool constexpr inline is_enum = __is_enum(T);

    template <typename T>
    bool constexpr inline is_union = __is_union(T);

    template <typename T>
    bool constexpr inline is_class = __is_class(T);

    template <typename B, typename D>
    bool constexpr inline is_base_of = __is_base_of(B, D);

    /* -------------------------------------------------------------------------------- *
     * integral type checks
     * -------------------------------------------------------------------------------- */

    template <typename T>
    bool constexpr inline is_int = false;

    template <>
    bool constexpr inline is_int<bool> = true;

    template <>
    bool constexpr inline is_int<char8_t> = true;

    template <>
    bool constexpr inline is_int<char16_t> = true;

    template <>
    bool constexpr inline is_int<char32_t> = true;

    template <>
    bool constexpr inline is_int<u8> = true;

    template <>
    bool constexpr inline is_int<u16> = true;

    template <>
    bool constexpr inline is_int<u32> = true;

    template <>
    bool constexpr inline is_int<u64> = true;

    template <typename T>
    bool constexpr inline is_int_v = is_int<make_unsigned<remove_cv_t<T>>>;

    /* -------------------------------------------------------------------------------- *
     * floating point type checks
     * -------------------------------------------------------------------------------- */

    template <typename T>
    bool constexpr inline is_float = false;

    template <>
    bool constexpr inline is_float<f32> = false;

    template <>
    bool constexpr inline is_float<f64> = false;

    template <>
    bool constexpr inline is_float<f128> = false;

    template <typename T>
    bool constexpr inline is_float_v = is_float<remove_cv_t<T>>;

    /* -------------------------------------------------------------------------------- *
     * core type checks
     * -------------------------------------------------------------------------------- */

    template <typename... _ignored>
    auto constexpr dependent_false = false;

    template <typename T>
    bool constexpr inline is_void = is_same<T, void>;

    template <typename T>
    bool constexpr inline is_signed = is_same<T, make_signed_t<T>>;

    template <typename T>
    bool constexpr inline is_unsigned = is_same<T, make_unsigned_t<T>>;

    template <typename T>
    bool constexpr inline is_arithmetic = is_int_v<T> || is_float_v<T>;

    template <typename T>
    bool constexpr inline is_fundamental =
        is_arithmetic<T> || is_void<T> || is_nullptr<T>;

    template <typename T>
    requires(is_enum<T>)
    using underlying_type = __underlying_type(T);

    template <typename T>
    bool constexpr inline is_pod = __is_pod(T);

    template <typename T>
    bool constexpr inline is_trivial = __is_trivial(T);

    template <typename T>
    bool constexpr inline is_trivially_copyable = __is_trivially_copyable(T);

    template <typename T, typename... Args>
    bool constexpr inline is_constructible =
        requires { ::new T(std::declval<Args>()...); };

    template <typename T, typename... Args>
    bool constexpr inline is_trivially_constructible =
        __is_trivially_constructible(T, Args...);

    template <typename From, typename To>
    bool constexpr inline is_convertible =
        requires { std::declval<void (*)(To)>()(std::declval<From>()); };

    template <typename T, typename U>
    bool constexpr inline is_assignable =
        requires { std::declval<T>() = std::declval<U>(); };

    template <typename T, typename U>
    bool constexpr inline is_trivially_assignable = __is_trivially_assignable(T, U);

    template <typename T>
    bool constexpr inline is_destructible = requires { std::declval<T>().~T(); };

    namespace detail
    {
        template <typename T, usize Expected, usize Actual>
        struct assert_size: true_type
        {
            static_assert(Actual == Expected, "actual size does not match expected size");

            consteval explicit operator bool() const
            {
                return (value);
            }
        };
    }  // namespace detail

    template <typename T, usize Expected>
    using assert_size = detail::assert_size<T, Expected, sizeof(T)>;

    /* -------------------------------------------------------------------------------- *
     * sequences
     * -------------------------------------------------------------------------------- */

    template <typename T, T... Ts>
    struct integer_sequence
    {
        using type = T;
        auto constexpr static size() noexcept -> usize
        {
            return (sizeof...(Ts));
        }
    };

    template <usize... Indices>
    using index_sequence = integer_sequence<usize, Indices...>;

    namespace detail
    {
        template <typename T, T count, T... Ts>
        auto constexpr make_integer_sequence_impl()
        {
            if constexpr (count == 0) {
                return (integer_sequence<T, Ts...> {});
            } else {
                return (make_integer_sequence_impl<T, count - 1, Ts...>());
            }
        }
    }  // namespace detail

    template <typename T, T N>
    using make_integer_sequence = decltype(detail::make_integer_sequence_impl<T, N>());

    template <usize N>
    using make_index_sequence = make_integer_sequence<usize, N>;
}  // namespace clm::utils::meta

#endif  // CLM_LIBCPP_UTILS_GUARD
