#ifndef CLM_LIBCPP_SHARED_GUARD
#define CLM_LIBCPP_SHARED_GUARD

#include <cmath>
#include <cstddef>
#include <cstdint>

typedef std::uint8_t u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;

typedef std::int8_t i8;
typedef std::int16_t i16;
typedef std::int32_t i32;
typedef std::int64_t i64;

typedef std::uintptr_t usize;
typedef std::intptr_t isize;

typedef usize uptr;
typedef isize iptr;
typedef std::ptrdiff_t ptrdiff;

typedef std::float_t f32;
typedef std::double_t f64;
typedef long double f128;

#endif /* CLM_LIBCPP_SHARED_GUARD */
