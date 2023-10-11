#ifndef CLM_LIBC_VECTOR_GUARD
#define CLM_LIBC_VECTOR_GUARD

#include <clm_libc_arena_export.h>

#include "clm_libc_shared.h"

struct clm_vector_s;
typedef struct clm_vector_s clm_vector;

struct clm_vector_s
{
    byte *data;
    usize capacity;
    usize size;
    usize length;
};

#endif /* CLM_LIBC_VECTOR_GUARD */
