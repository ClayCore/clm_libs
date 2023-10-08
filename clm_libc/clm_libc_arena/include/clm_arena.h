#ifndef CLM_LIBC_ARENA_GUARD
#define CLM_LIBC_ARENA_GUARD

#include <clm_libc_arena_export.h>

#include "clm_libc_shared.h"


struct clm_memregion_s;
typedef struct clm_memregion_s clm_memregion_t;

struct clm_arena_s;
typedef struct clm_arena_s clm_arena_t;

struct clm_memregion_s
{
    clm_memregion_t *next;
    usize count;
    usize capacity;
    uptr data[];
};

struct clm_arena_s
{
    clm_memregion_t *begin;
    clm_memregion_t *end;
};


CLM_LIBC_ARENA_EXPORT clm_memregion_t *clm_memregion_new(usize);
CLM_LIBC_ARENA_EXPORT void clm_memregion_release(clm_memregion_t *);

CLM_LIBC_ARENA_EXPORT void *clm_arena_alloc(clm_arena_t *, usize);
CLM_LIBC_ARENA_EXPORT void *clm_arena_realloc(clm_arena_t *, void *, usize, usize);
CLM_LIBC_ARENA_EXPORT void clm_arena_reset(clm_arena_t *);
CLM_LIBC_ARENA_EXPORT void clm_arena_release(clm_arena_t *);

#endif /* CLM_LIBC_ARENA_GUARD */
