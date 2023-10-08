#include <stdlib.h>

#include "clm_arena.h"

#ifndef CLM_ARENA_DEFAULT_CAP
#define CLM_ARENA_DEFAULT_CAP (8U * 1024U)
#endif /* CLM_ARENA_DEFAULT_CAP */

clm_memregion_t *clm_memregion_new(usize capacity)
{
    usize total             = sizeof(clm_memregion_t) + sizeof(uptr) * capacity;
    clm_memregion_t *region = calloc(1, total);
    // TODO: check if region is null
    region->capacity = capacity;

    return (region);
}

void clm_memregion_release(clm_memregion_t *region)
{
    free(region);
    region = NULL;
}

void *clm_arena_alloc(clm_arena_t *arena, usize size)
{
    usize current_cap = size;
    usize total_size  = (size + sizeof(uptr) - 1) / sizeof(uptr);
    usize capacity    = CLM_ARENA_DEFAULT_CAP;

    void *mem = NULL;

    if (arena->end == NULL) {
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

void *clm_arena_realloc(clm_arena_t *arena, void *ptr, usize old_size, usize new_size)
{
    void *new_ptr     = NULL;
    u8 *new_ptr_bytes = NULL;
    u8 *old_ptr_bytes = NULL;

    if (new_size <= old_size) {
        return (ptr);
    }

    new_ptr       = clm_arena_alloc(arena, new_size);
    new_ptr_bytes = new_ptr;
    old_ptr_bytes = ptr;

    for (usize i = 0; i < old_size; ++i) {
        new_ptr_bytes[i] = old_ptr_bytes[i];
    }

    return (new_ptr);
}

void clm_arena_reset(clm_arena_t *arena)
{
    clm_memregion_t *region = NULL;

    for (region = arena->begin; region != NULL; region = region->next) {
        region->count = 0;
    }

    arena->end = arena->begin;
}

void clm_arena_release(clm_arena_t *arena)
{
    clm_memregion_t *region = NULL;
    while (region) {
        clm_memregion_t *treg = region;

        region = region->next;
        clm_memregion_release(treg);
    }

    arena->begin = NULL;
    arena->end   = NULL;
}
