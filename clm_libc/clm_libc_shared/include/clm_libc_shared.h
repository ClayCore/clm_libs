#ifndef CLM_LIBC_SHARED_GUARD
#define CLM_LIBC_SHARED_GUARD

#include <math.h>
#include <stddef.h>
#include <stdint.h>

#define FALSE       (0U)
#define TRUE        (1U)
#define SIZE_OF(x)  ((usize)(sizeof(x)))
#define ALIGN_OF(x) ((usize)(_Alignof(x)))
#define COUNT_OF(x) ((sizeof(x)) / (sizeof(*(x))))
#define ASSERT(c) \
    while (!(c)) __builtin_unreachable()

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint_fast32_t b32;
typedef uint8_t byte;

typedef float_t f32;
typedef double_t f64;
typedef long double f128;

typedef uintptr_t uptr;
typedef intptr_t iptr;

typedef uintptr_t usize;
typedef ptrdiff_t isize;

#endif /* CLM_LIBC_SHARED_GUARD */
