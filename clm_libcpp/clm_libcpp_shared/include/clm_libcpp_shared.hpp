#ifndef CLM_LIBCPP_SHARED_GUARD
#define CLM_LIBCPP_SHARED_GUARD

#include <cmath>
#include <cstddef>
#include <cstdint>

using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using i8  = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using byte = std::uint8_t;
using b32  = std::uint_fast32_t;

using uptr = std::uintptr_t;
using iptr = std::intptr_t;

using usize = std::uintptr_t;
using isize = std::ptrdiff_t;

using f32  = std::float_t;
using f64  = std::double_t;
using f128 = long double;

#endif  // CLM_LIBCPP_SHARED_GUARD
