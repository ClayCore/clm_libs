#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clm_arena.h"
#include "clm_logging.h"
#include "clm_string.h"

/*****************************************************************************************
 * STATICS AND CONSTANTS BEGIN
 ****************************************************************************************/

static clm_arena_t CLM_STRING_ARENA   = { 0 };
static clm_arena_t *CLM_CONTEXT_ARENA = &CLM_STRING_ARENA;

void static *clm_alloc(usize size)
{
    return clm_arena_alloc(CLM_CONTEXT_ARENA, size);
}

/** Look-up table for fast 'to_lower' method */
u8 static const CLM_STRING_LOWER_LUT[] = {
    [' '] = ' ',   ['!'] = '!',   ['\"'] = '\"', ['#'] = '#', ['$'] = '$', ['%'] = '%',
    ['&'] = '&',   ['\''] = '\'', ['*'] = '*',   ['+'] = '+', [','] = ',', ['-'] = '-',
    ['='] = '=',   ['`'] = '`',   ['^'] = '^',   ['~'] = '~', ['.'] = '.', ['/'] = '/',
    ['\\'] = '\\', ['|'] = '|',   [':'] = ':',   [';'] = ';', ['('] = '(', [')'] = ')',
    ['{'] = '{',   ['}'] = '}',   ['<'] = '<',   ['>'] = '>', ['['] = '[', [']'] = ']',
    ['_'] = '_',   ['0'] = '0',   ['1'] = '1',   ['2'] = '2', ['3'] = '3', ['4'] = '4',
    ['5'] = '5',   ['6'] = '6',   ['7'] = '7',   ['8'] = '8', ['9'] = '9', ['a'] = 'a',
    ['A'] = 'a',   ['b'] = 'b',   ['B'] = 'b',   ['c'] = 'c', ['C'] = 'c', ['d'] = 'd',
    ['D'] = 'd',   ['e'] = 'e',   ['E'] = 'e',   ['f'] = 'f', ['F'] = 'f', ['g'] = 'g',
    ['G'] = 'g',   ['h'] = 'h',   ['H'] = 'h',   ['i'] = 'i', ['I'] = 'i', ['j'] = 'j',
    ['J'] = 'j',   ['k'] = 'k',   ['K'] = 'k',   ['l'] = 'l', ['L'] = 'l', ['m'] = 'm',
    ['M'] = 'm',   ['n'] = 'n',   ['N'] = 'n',   ['o'] = 'o', ['O'] = 'o', ['p'] = 'p',
    ['P'] = 'p',   ['q'] = 'q',   ['Q'] = 'q',   ['r'] = 'r', ['R'] = 'r', ['s'] = 's',
    ['S'] = 's',   ['t'] = 't',   ['T'] = 't',   ['u'] = 'u', ['U'] = 'u', ['v'] = 'v',
    ['V'] = 'v',   ['w'] = 'w',   ['W'] = 'w',   ['x'] = 'x', ['X'] = 'x', ['y'] = 'y',
    ['Y'] = 'y',   ['z'] = 'z',   ['Z'] = 'z',   [1] = 1,     [2] = 2,     [3] = 3,
    [4] = 4,       [5] = 5,       [6] = 6,       [7] = 7,     [8] = 8,     [9] = 9,
    [10] = 10,     [11] = 11,     [12] = 12,     [13] = 13,   [14] = 14,   [15] = 15,
    [16] = 16,     [17] = 17,     [18] = 18,     [19] = 19,   [20] = 20,   [21] = 21,
    [22] = 22,     [23] = 23,     [24] = 24,     [25] = 25,   [26] = 26,   [27] = 27,
    [28] = 28,     [29] = 29,     [30] = 30,     [31] = 31,   [127] = 127,
};

/** Look-up table for fast 'to_upper' method */
u8 static const CLM_STRING_UPPER_LUT[] = {
    [' '] = ' ',   ['!'] = '!',   ['\"'] = '\"', ['#'] = '#', ['$'] = '$', ['%'] = '%',
    ['&'] = '&',   ['\''] = '\'', ['*'] = '*',   ['+'] = '+', [','] = ',', ['-'] = '-',
    ['='] = '=',   ['`'] = '`',   ['^'] = '^',   ['~'] = '~', ['.'] = '.', ['/'] = '/',
    ['\\'] = '\\', ['|'] = '|',   [':'] = ':',   [';'] = ';', ['('] = '(', [')'] = ')',
    ['{'] = '{',   ['}'] = '}',   ['<'] = '<',   ['>'] = '>', ['['] = '[', [']'] = ']',
    ['_'] = '_',   ['0'] = '0',   ['1'] = '1',   ['2'] = '2', ['3'] = '3', ['4'] = '4',
    ['5'] = '5',   ['6'] = '6',   ['7'] = '7',   ['8'] = '8', ['9'] = '9', ['a'] = 'A',
    ['A'] = 'A',   ['b'] = 'B',   ['B'] = 'B',   ['c'] = 'C', ['C'] = 'C', ['d'] = 'D',
    ['D'] = 'D',   ['e'] = 'E',   ['E'] = 'E',   ['f'] = 'F', ['F'] = 'F', ['g'] = 'G',
    ['G'] = 'G',   ['h'] = 'H',   ['H'] = 'H',   ['i'] = 'I', ['I'] = 'I', ['j'] = 'J',
    ['J'] = 'J',   ['k'] = 'K',   ['K'] = 'K',   ['l'] = 'L', ['L'] = 'L', ['m'] = 'M',
    ['M'] = 'M',   ['n'] = 'N',   ['N'] = 'N',   ['o'] = 'O', ['O'] = 'O', ['p'] = 'P',
    ['P'] = 'P',   ['q'] = 'Q',   ['Q'] = 'Q',   ['r'] = 'R', ['R'] = 'R', ['s'] = 'S',
    ['S'] = 'S',   ['t'] = 'T',   ['T'] = 'T',   ['u'] = 'U', ['U'] = 'U', ['v'] = 'V',
    ['V'] = 'V',   ['w'] = 'W',   ['W'] = 'W',   ['x'] = 'X', ['X'] = 'X', ['y'] = 'Y',
    ['Y'] = 'Y',   ['z'] = 'Z',   ['Z'] = 'Z',   [1] = 1,     [2] = 2,     [3] = 3,
    [4] = 4,       [5] = 5,       [6] = 6,       [7] = 7,     [8] = 8,     [9] = 9,
    [10] = 10,     [11] = 11,     [12] = 12,     [13] = 13,   [14] = 14,   [15] = 15,
    [16] = 16,     [17] = 17,     [18] = 18,     [19] = 19,   [20] = 20,   [21] = 21,
    [22] = 22,     [23] = 23,     [24] = 24,     [25] = 25,   [26] = 26,   [27] = 27,
    [28] = 28,     [29] = 29,     [30] = 30,     [31] = 31,   [127] = 127,
};

/*****************************************************************************************
 * STATICS AND CONSTANTS END
 ****************************************************************************************/

/*****************************************************************************************
 * IMPLEMENTATION BEGIN
 ****************************************************************************************/

clm_string *clm_string_new(void)
{
    clm_string *str = NULL;

    str = clm_alloc(sizeof *str);
    if (!str) {
        clm_log_error("clm_string_new: failed to alloc string\n");
        exit(EXIT_FAILURE);
    }

    return (str);
}

clm_string *clm_string_from(u8 const *buf, usize len)
{
    clm_string *str = NULL;
    usize length    = len;

    if (buf && len == 0) {
        u8 const *cpy = buf;
        while (*cpy++) {
            length += 1;
        }

        assert(length > 0);
    }

    if (!buf) {
        clm_log_error("clm_string_from: buf is null\n");
        exit(EXIT_FAILURE);
    }

    str = clm_alloc(sizeof *str);
    if (!str) {
        clm_log_error("clm_string_from: failed to alloc string\n");
        exit(EXIT_FAILURE);
    }

    str->len = length;
    str->ptr = clm_alloc(str->len * sizeof *str->ptr);
    if (!str->ptr) {
        clm_log_error("clm_string_from: failed to alloc string buffer\n");
        exit(EXIT_FAILURE);
    }

    memcpy(str->ptr, buf, str->len);

    return (str);
}

clm_string *clm_string_copy(clm_string *src)
{
    clm_string *cpy = NULL;

    if (!src) {
        clm_log_error("clm_string_copy: src is null\n");
        exit(EXIT_FAILURE);
    }

    if (!src->ptr || src->len == 0) {
        clm_log_error("clm_string_copy: src is empty\n");
        exit(EXIT_FAILURE);
    }

    cpy = clm_alloc(sizeof *cpy);
    if (!cpy) {
        clm_log_error("clm_string_copy: failed to alloc string\n");
        exit(EXIT_FAILURE);
    }

    cpy->len = src->len;
    cpy->ptr = clm_alloc(src->len * sizeof *cpy->ptr);
    if (!cpy->ptr) {
        clm_log_error("clm_string_copy: failed to alloc string buffer\n");
        exit(EXIT_FAILURE);
    }

    memcpy(cpy->ptr, src->ptr, cpy->len);

    return (cpy);
}

void clm_string_init(clm_string *str, u8 const *buf, usize len)
{
    if (!str) {
        clm_log_error("clm_string_init: src is null\n");
        exit(EXIT_FAILURE);
    }

    if (str->ptr || str->len != 0) {
        clm_log_error("clm_string_init: src is not empty\n");
        exit(EXIT_FAILURE);
    }

    if (!buf || len == 0) {
        clm_log_error("clm_string_init: c-string is empty\n");
        exit(EXIT_FAILURE);
    }

    str->len = len;
    str->ptr = clm_alloc(str->len * sizeof *str->ptr);
    if (!str->ptr) {
        clm_log_error("clm_string_init: failed to alloc string buffer\n");
        exit(EXIT_FAILURE);
    }

    memcpy(str->ptr, buf, str->len);
}

u64 clm_string_raw_hash(u8 const *buf, usize len)
{
    u64 res   = (2 << 12) - 21;
    usize cnt = 0;

    if (!buf) {
        clm_log_error("clm_string_raw_hash: buf is null\n");
        exit(EXIT_FAILURE);
    }

    if (len == 0) {
        clm_log_error("clm_string_raw_hash: zero length buffer\n");
        exit(EXIT_FAILURE);
    }

    while (cnt < len) {
        res = ((res << 5) + res) + buf[cnt];
        cnt += 1;
    }

    return (res);
}

u64 clm_string_hash(clm_string *str)
{
    u64 res = clm_string_raw_hash(str->ptr, str->len);

    return (res);
}

bool clm_string_eq(clm_string *lhs, clm_string *rhs)
{
    u64 lhash = 0, rhash = 0;

    if (!lhs) {
        clm_log_error("clm_string_eq: lhs is null\n");
        exit(EXIT_FAILURE);
    }

    if (!lhs->ptr || lhs->len == 0) {
        clm_log_error("clm_string_eq: lhs is empty\n");
        exit(EXIT_FAILURE);
    }

    if (!rhs) {
        clm_log_error("clm_string_eq: rhs is null\n");
        exit(EXIT_FAILURE);
    }

    if (!rhs->ptr || rhs->len == 0) {
        clm_log_error("clm_string_eq: rhs is empty\n");
        exit(EXIT_FAILURE);
    }

    lhash = clm_string_hash(lhs);
    rhash = clm_string_hash(rhs);

    return (lhash == rhash);
}

bool clm_string_eq_icase(clm_string *lhs, clm_string *rhs)
{
    clm_string *llhs = NULL, *rrhs = NULL;
    u64 lhash = 0, rhash = 0;

    llhs = clm_string_to_lower(lhs);
    rrhs = clm_string_to_lower(rhs);

    lhash = clm_string_hash(llhs);
    rhash = clm_string_hash(rrhs);

    clm_string_free(llhs);
    clm_string_free(rrhs);

    return (lhash == rhash);
}

isize clm_string_find(clm_string *str, u8 chr)
{
    bool found  = false;
    isize index = 0;
    usize iter  = 0;

    if (!str) {
        clm_log_error("clm_string_find: str is null\n");
        exit(EXIT_FAILURE);
    }

    if (!str->ptr || str->len == 0) {
        clm_log_error("clm_string_find: str is empty\n");
        exit(EXIT_FAILURE);
    }

    if (chr == '\0' || !isprint(chr)) {
        clm_log_error("clm_string_find: invalid character\n");
        exit(EXIT_FAILURE);
    }

    for (; iter < str->len; ++iter) {
        if (str->ptr[iter] == chr) {
            found = true;
            break;
        }
    }

    return (!found ? -1 : index);
}

isize clm_string_find_rev(clm_string *str, u8 chr)
{
    bool found  = false;
    isize index = 0;
    usize iter  = str->len;

    if (!str) {
        clm_log_error("clm_string_find_rev: str is null\n");
        exit(EXIT_FAILURE);
    }

    if (!str->ptr || str->len == 0) {
        clm_log_error("clm_string_find_rev: str is empty\n");
        exit(EXIT_FAILURE);
    }

    if (chr == '\0' || !isprint(chr)) {
        clm_log_error("clm_string_find_rev: invalid character\n");
        exit(EXIT_FAILURE);
    }

    while (iter--) {
        if (str->ptr[iter] == chr) {
            found = true;
            break;
        }
    }

    return (!found ? -1 : index);
}

usize clm_string_span(clm_string *str, clm_string *filter)
{
    usize run = 0, iter = 0;
    isize idx = 0;

    if (!str) {
        clm_log_error("clm_string_span: str is null\n");
        exit(EXIT_FAILURE);
    }

    if (!str->ptr || str->len == 0) {
        clm_log_error("clm_string_span: str is empty\n");
        exit(EXIT_FAILURE);
    }

    if (!filter) {
        clm_log_error("clm_string_span: filter is null\n");
        exit(EXIT_FAILURE);
    }

    if (!filter->ptr || filter->len == 0) {
        clm_log_error("clm_string_span: filter is empty\n");
        exit(EXIT_FAILURE);
    }

    for (; iter < str->len; ++iter) {
        idx = clm_string_find(filter, str->ptr[iter]);
        if (str->ptr[iter] == filter->ptr[idx]) {
            run += 1;
        }
    }

    return (run);
}

usize clm_string_span_rev(clm_string *str, clm_string *filter)
{
    usize run = 0, iter = str->len;
    isize idx = 0;

    if (!str) {
        clm_log_error("clm_string_span_rev: str is null\n");
        exit(EXIT_FAILURE);
    }

    if (!str->ptr || str->len == 0) {
        clm_log_error("clm_string_span_rev: str is empty\n");
        exit(EXIT_FAILURE);
    }

    if (!filter) {
        clm_log_error("clm_string_span_rev: filter is null\n");
        exit(EXIT_FAILURE);
    }

    if (!filter->ptr || filter->len == 0) {
        clm_log_error("clm_string_span_rev: filter is empty\n");
        exit(EXIT_FAILURE);
    }

    while (iter--) {
        idx = clm_string_find_rev(filter, str->ptr[iter]);
        if (str->ptr[iter] == filter->ptr[idx]) {
            run += 1;
        }
    }

    return (run);
}

clm_string *clm_string_to_lower(clm_string *str)
{
    clm_string *res = clm_string_copy(str);
    clm_string_to_lower_mut(res);

    return (res);
}

clm_string *clm_string_to_upper(clm_string *str)
{
    clm_string *res = clm_string_copy(str);
    clm_string_to_upper_mut(res);

    return (res);
}

void clm_string_to_lower_mut(clm_string *str)
{
    usize iter = 0, idx = 0;
    u8 chr = 0;

    if (!str) {
        clm_log_error("clm_string_to_lower_mut: str is null\n");
        exit(EXIT_FAILURE);
    }

    if (!str->ptr || str->len == 0) {
        clm_log_error("clm_string_to_lower_mut: str is empty\n");
        exit(EXIT_FAILURE);
    }

    for (; iter < str->len; ++iter) {
        idx = (usize)(str->ptr[iter]);
        chr = CLM_STRING_LOWER_LUT[idx];

        str->ptr[iter] = chr;
    }
}

void clm_string_to_upper_mut(clm_string *str)
{
    usize iter = 0, idx = 0;
    u8 chr = 0;

    if (!str) {
        clm_log_error("clm_string_to_upper_mut: str is null\n");
        exit(EXIT_FAILURE);
    }

    if (!str->ptr || str->len == 0) {
        clm_log_error("clm_string_to_upper_mut: str is empty\n");
        exit(EXIT_FAILURE);
    }

    for (; iter < str->len; ++iter) {
        idx = (usize)(str->ptr[iter]);
        chr = CLM_STRING_UPPER_LUT[idx];

        str->ptr[iter] = chr;
    }
}

void clm_string_free(clm_string *str)
{
    str->ptr = NULL;
    str->len = 0;

    str = NULL;
}


/*****************************************************************************************
 * IMPLEMENTATION END
 ****************************************************************************************/
