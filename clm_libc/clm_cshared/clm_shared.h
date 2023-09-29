#ifndef CLM_SHARED_GUARD_
#define CLM_SHARED_GUARD_

#include <stddef.h>
#include <stdint.h>


/******************************************************************************
 * TYPE ALIASES
 *****************************************************************************/

#ifndef CLM_TYPEDEFS
#define CLM_TYPEDEFS(CLM_TYPEDEF)       \
        CLM_TYPEDEF(uint8_t, u8)        \
        CLM_TYPEDEF(uint16_t, u16)      \
        CLM_TYPEDEF(uint32_t, u32)      \
        CLM_TYPEDEF(uint64_t, u64)      \
        CLM_TYPEDEF(int8_t, i8)         \
        CLM_TYPEDEF(int16_t, i16)       \
        CLM_TYPEDEF(int32_t, i32)       \
        CLM_TYPEDEF(int64_t, i64)       \
        CLM_TYPEDEF(uint8_t, byte)      \
        CLM_TYPEDEF(uintptr_t, usize)   \
        CLM_TYPEDEF(intptr_t, isize)    \
        CLM_TYPEDEF(uintptr_t, uptr)    \
        CLM_TYPEDEF(intptr_t, iptr)     \
        CLM_TYPEDEF(ptrdiff_t, ptrdiff) \
        CLM_TYPEDEF(float, f32)         \
        CLM_TYPEDEF(double, f64)        \
        CLM_TYPEDEF(long double, f128)
#endif /* CLM_TYPEDEFS */

#ifndef CLM_TYPEDEF
#define CLM_TYPEDEF(ty, alias) typedef ty alias;

#endif /* CLM_TYPEDEF */

CLM_TYPEDEFS(CLM_TYPEDEF)

#undef CLM_TYPEDEFS
#undef CLM_TYPEDEF

#endif /* CLM_SHARED_GUARD_ */
