#include <stdlib.h>

#include "clm_arena.h"

#ifndef CLM_ARENA_DEFAULT_CAP
#define CLM_ARENA_DEFAULT_CAP (8U * 1024U)
#endif /* CLM_ARENA_DEFAULT_CAP */

memregion *clm_memregion_new(usize capacity)
{
    usize total       = SIZE_OF(memregion) + SIZE_OF(uptr) * capacity;
    memregion *region = calloc(1, total);
    // TODO: check if region is null
    region->capacity = capacity;

    return (region);
}

void clm_memregion_release(memregion *region)
{
    free(region);
    region = NULL;
}

void *clm_arena_alloc(arena *arena, usize size)
{
    usize current_cap = size;
    usize total_size  = (size + SIZE_OF(uptr) - 1) / SIZE_OF(uptr);
    usize capacity    = CLM_ARENA_DEFAULT_CAP;

    void *mem = NULL;

    if (!arena->end) {
        if (capacity < total_size) {
            capacity = total_size;
        }

        arena->end   = clm_memregion_new(capacity);
        arena->begin = arena->end;
    }

    current_cap = arena->end->count + total_size;
    while ((current_cap > arena->end->capacity) && arena->end->next) {
        arena->end = arena->end->next;
    }

    if ((arena->end->count + total_size) > arena->end->capacity) {
        capacity = CLM_ARENA_DEFAULT_CAP;
        if (capacity < total_size) {
            capacity = total_size;
        }

        arena->end->next = clm_memregion_new(capacity);
        arena->end       = arena->end->next;
    }

    mem = &arena->end->data[arena->end->count];
    arena->end->count += total_size;

    return (mem);
}

void *clm_arena_realloc(arena *arena, void *ptr, usize old_size, usize new_size)
{
    void *new_ptr       = NULL;
    byte *new_ptr_bytes = NULL;
    byte *old_ptr_bytes = NULL;

    if (new_size <= old_size) {
        return (ptr);
    }

    new_ptr = clm_arena_alloc(arena, new_size);
    ASSERT(new_ptr);

    new_ptr_bytes = (byte *)(new_ptr);
    old_ptr_bytes = (byte *)(ptr);

    for (usize i = 0; i < old_size; ++i) {
        new_ptr_bytes[i] = old_ptr_bytes[i];
    }

    return (new_ptr);
}

void clm_arena_reset(arena *arena)
{
    memregion *region = NULL;

    for (region = arena->begin; region; region = region->next) {
        region->count = 0;
    }

    arena->end = arena->begin;
}

void clm_arena_release(arena *arena)
{
    memregion *region = arena->begin;
    memregion *treg   = region;

    while (region) {
        treg = region;

        region = region->next;
        clm_memregion_release(treg);
    }

    arena->begin = NULL;
    arena->end   = NULL;
}
