#ifndef CLM_LIBC_ARENA_GUARD
#define CLM_LIBC_ARENA_GUARD

#include <clm_libc_arena_export.h>

#include "clm_libc_shared.h"


typedef struct memregion memregion;
struct memregion
{
    memregion *next;
    usize count;
    usize capacity;
    uptr data[];
};

CLM_LIBC_ARENA_EXPORT memregion *clm_memregion_new(usize);
CLM_LIBC_ARENA_EXPORT void clm_memregion_release(memregion *);


typedef struct arena arena;
struct arena
{
    memregion *begin;
    memregion *end;
};

CLM_LIBC_ARENA_EXPORT void *clm_arena_alloc(arena *, usize);
CLM_LIBC_ARENA_EXPORT void *clm_arena_realloc(arena *, void *, usize, usize);
CLM_LIBC_ARENA_EXPORT void clm_arena_reset(arena *);
CLM_LIBC_ARENA_EXPORT void clm_arena_release(arena *);

#endif /* CLM_LIBC_ARENA_GUARD */
