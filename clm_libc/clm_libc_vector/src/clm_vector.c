#include <math.h>

#include "clm_arena.h"
#include "clm_libc_shared.h"
#include "clm_logging.h"
#include "clm_vector.h"


/*****************************************************************************************
 * STATICS AND CONSTANTS BEGIN
 ****************************************************************************************/

#define CLM_VECTOR_DEFAULT_CAP (16U)
#define CLM_VECTOR_GROWTH_FACT (1.73205080757)

static arena CLM_VECTOR_ARENA   = { 0 };
static arena *CLM_CONTEXT_ARENA = &CLM_VECTOR_ARENA;

/*****************************************************************************************
 * UTILITY FUNCTIONS
 ****************************************************************************************/

static usize nearest_pow_2(usize val)
{
    usize next = val;
    usize prev = 0;

    next--;
    next |= next >> 1;
    next |= next >> 2;
    next |= next >> 4;
    next |= next >> 8;
    next |= next >> 16;
    next++;

    prev = next >> 1;

    return ((next - val) > (val - prev) ? (prev) : (next));
}

static usize vector_new_cap(usize cap)
{
    double trunc  = round((double)(cap)*CLM_VECTOR_GROWTH_FACT);
    usize rounded = nearest_pow_2((usize)(trunc));

    return (rounded);
}

static clm_vector *clm_vector_new(usize item_size)
{
    clm_vector *vec = NULL;
    usize bufsz     = CLM_VECTOR_DEFAULT_CAP * item_size;

    vec = clm_arena_alloc(CLM_CONTEXT_ARENA, SIZE_OF(*vec));
    // todo alloc check + logging

    vec->data = clm_arena_alloc(CLM_CONTEXT_ARENA, bufsz);
    // todo alloc check + logging

    vec->capacity = CLM_VECTOR_DEFAULT_CAP * item_size;
    vec->size     = 0;
    vec->length   = 0;

    return (vec);
}

static void clm_vector_push(clm_vector *vec, void *item, usize item_size)
{
    byte *raw_item = item;

    if (vec->size + item_size > vec->capacity) {
        usize new_cap = vector_new_cap(vec->capacity);
        vec->data =
            clm_arena_realloc(CLM_CONTEXT_ARENA, vec->data, vec->capacity, new_cap);
    }

    for (usize i = 0; i < item_size; ++i) {
        vec->data[i] = raw_item[i];
    }

    vec->size += item_size;
    vec->length += 1;
}

static void clm_vector_release(clm_vector *vec)
{
    vec->length   = 0;
    vec->size     = 0;
    vec->capacity = 0;

    vec->data = NULL;
    vec       = NULL;
}
