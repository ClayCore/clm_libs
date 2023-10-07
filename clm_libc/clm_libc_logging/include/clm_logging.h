#ifndef CLM_LIBC_LOGGING_GUARD
#define CLM_LIBC_LOGGING_GUARD

#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include "clm_libc_shared.h"

struct clm_log_record_t;
typedef struct clm_log_record_t clm_log_record;

struct clm_log_record_t
{
    va_list ap;
    char const *fmt;
    char const *file;
    struct tm *time;
    void *data;
    i32 line;
    i32 level;
};

typedef void (*clm_log_logfn)(clm_log_record *);
typedef void (*clm_log_lockfn)(bool, void *);

enum
{
    CLM_LOG_TRACE,
    CLM_LOG_DEBUG,
    CLM_LOG_INFO,
    CLM_LOG_WARN,
    CLM_LOG_ERROR,
};

#define clm_log_trace(...) (clm_log(CLM_LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__))
#define clm_log_debug(...) (clm_log(CLM_LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__))
#define clm_log_info(...) (clm_log(CLM_LOG_INFO, __FILE__, __LINE__, __VA_ARGS__))
#define clm_log_warn(...) (clm_log(CLM_LOG_WARN, __FILE__, __LINE__, __VA_ARGS__))
#define clm_log_error(...) (clm_log(CLM_LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__))

char const *clm_log_level_string(i32);
void clm_log_set_lock(clm_log_lockfn, void *);
void clm_log_set_level(i32);
void clm_log_set_quiet(bool);
i32 clm_log_add_callback(clm_log_logfn, void *, i32);
i32 clm_log_add_fp(FILE *, i32);

void clm_log(i32, char const *, i32, char const *, ...);

#endif /* CLM_LIBC_LOGGING_GUARD */
