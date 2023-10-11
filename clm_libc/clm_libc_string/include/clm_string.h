#ifndef CLM_LIBC_STRING_GUARD
#define CLM_LIBC_STRING_GUARD

#include <clm_libc_string_export.h>

#include "clm_libc_shared.h"


struct clm_string_s;
typedef struct clm_string_s clm_string;

struct clm_string_list_s;
typedef struct clm_string_list_s clm_string_list;

struct clm_string_s
{
    u8 *ptr;
    usize len;
};

struct clm_string_list_s
{
    clm_string *ptr;
    usize len;
};


CLM_LIBC_STRING_EXPORT clm_string *clm_string_new(void);
CLM_LIBC_STRING_EXPORT clm_string *clm_string_from(u8 const *, usize);
CLM_LIBC_STRING_EXPORT clm_string *clm_string_copy(clm_string *);
CLM_LIBC_STRING_EXPORT void clm_string_init(clm_string *, u8 const *, usize);

CLM_LIBC_STRING_EXPORT u64 clm_string_raw_hash(u8 const *, usize);
CLM_LIBC_STRING_EXPORT u64 clm_string_hash(clm_string *);

CLM_LIBC_STRING_EXPORT b32 clm_string_eq(clm_string *, clm_string *);
CLM_LIBC_STRING_EXPORT b32 clm_string_eq_icase(clm_string *, clm_string *);

CLM_LIBC_STRING_EXPORT isize clm_string_find(clm_string *, u8);
CLM_LIBC_STRING_EXPORT isize clm_string_find_rev(clm_string *, u8);

CLM_LIBC_STRING_EXPORT usize clm_string_span(clm_string *, clm_string *);
CLM_LIBC_STRING_EXPORT usize clm_string_span_rev(clm_string *, clm_string *);

CLM_LIBC_STRING_EXPORT clm_string *clm_string_to_lower(clm_string *);
CLM_LIBC_STRING_EXPORT clm_string *clm_string_to_upper(clm_string *);

CLM_LIBC_STRING_EXPORT void clm_string_to_lower_mut(clm_string *);
CLM_LIBC_STRING_EXPORT void clm_string_to_upper_mut(clm_string *);

CLM_LIBC_STRING_EXPORT void clm_string_filter(clm_string *, clm_string *);
CLM_LIBC_STRING_EXPORT clm_string_list *clm_string_split(clm_string *, clm_string *);

CLM_LIBC_STRING_EXPORT void clm_string_free(clm_string *);


CLM_LIBC_STRING_EXPORT clm_string_list *clm_string_list_new(void);

CLM_LIBC_STRING_EXPORT void clm_string_list_push(clm_string_list *, clm_string *);
CLM_LIBC_STRING_EXPORT clm_string *clm_string_list_pop(clm_string_list *);

CLM_LIBC_STRING_EXPORT void clm_string_list_free(clm_string_list *);

#endif /* CLM_LIBC_STRING_GUARD */
