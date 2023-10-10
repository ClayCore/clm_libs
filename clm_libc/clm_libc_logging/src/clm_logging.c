#include <errno.h>
#include <stdlib.h>

#include "clm_logging.h"

/*****************************************************************************************
 * STATICS AND CONSTANTS BEGIN
 ****************************************************************************************/

#ifndef CLM_LOG_MAX_CALLBACKS
#define CLM_LOG_MAX_CALLBACKS (64)
#endif /* CLM_LOG_MAX_CALLBACKS */

typedef struct
{
    clm_log_logfn fn;
    void *data;
    i32 level;
} callback_t;

static struct
{
    void *data;
    clm_log_lockfn lock;
    i32 level;
    b32 quiet;
    callback_t callbacks[CLM_LOG_MAX_CALLBACKS];
} CLM_LOGGER;

// clang-format off
char static const *CLM_LOG_LEVELS[] = {
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR"
};
// clang-format on

// clang-format off
char static const *CLM_LOG_COLORS[] = {
    "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m"
};
// clang-format on

/*****************************************************************************************
 * STATICS AND CONSTANTS END
 ****************************************************************************************/

/*****************************************************************************************
 * UTILITY FUNCTIONS BEGIN
 ****************************************************************************************/

static void clm_log_lock(void)
{
    if (CLM_LOGGER.lock) {
        CLM_LOGGER.lock(TRUE, CLM_LOGGER.data);
    }
}

static void clm_log_unlock(void)
{
    if (CLM_LOGGER.lock) {
        CLM_LOGGER.lock(FALSE, CLM_LOGGER.data);
    }
}

static void clm_stdout_callback(clm_log_record *record)
{
    char buf[16] = { 0 };

    buf[strftime(buf, sizeof(buf), "%H:%M:%S", record->time)] = '\0';

    // clang-format off
    fprintf(
        record->stream,
        "%s %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ",
        buf,
        CLM_LOG_COLORS[record->level],
        CLM_LOG_LEVELS[record->level],
        record->file, record->line
    );
    // clang-format on

    vfprintf(record->stream, record->fmt, record->ap);
    fprintf(record->stream, "\n");
    fflush(record->stream);
}

static void clm_file_callback(clm_log_record *record)
{
    char buf[64] = { 0 };

    buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", record->time)] = '\0';

    // clang-format off
    fprintf(
        record->stream,
        "%s %-5s %s:%d: ",
        buf,
        CLM_LOG_LEVELS[record->level],
        record->file,
        record->line
    );
    // clang-format on

    vfprintf(record->stream, record->fmt, record->ap);
    fprintf(record->stream, "\n");
    fflush(record->stream);
}

static void clm_init_record(clm_log_record *record, void *stream)
{
    if (!record->time) {
        time_t tm    = time(NULL);
        record->time = localtime(&tm);
    }

    record->stream = stream;
}

/*****************************************************************************************
 * UTILITY FUNCTIONS END
 ****************************************************************************************/

/*****************************************************************************************
 * IMPLEMENTATION BEGIN
 ****************************************************************************************/

char *clm_log_level_string(i32 level)
{
    return (CLM_LOG_LEVELS[level]);
}


void clm_log_set_lock(clm_log_lockfn fn, void *data)
{
    CLM_LOGGER.lock = fn;
    CLM_LOGGER.data = data;
}


void clm_log_set_level(i32 level)
{
    CLM_LOGGER.level = level;
}


void clm_log_set_quiet(b32 enable)
{
    CLM_LOGGER.quiet = enable;
}


i32 clm_log_add_callback(clm_log_logfn fn, void *data, i32 level)
{
    for (i32 i = 0; i < CLM_LOG_MAX_CALLBACKS; ++i) {
        if (!CLM_LOGGER.callbacks[i].fn) {
            CLM_LOGGER.callbacks[i] = (callback_t) { fn, data, level };
            return (0);
        }
    }

    return (-1);
}


i32 clm_log_add_fp(FILE *fp, i32 level)
{
    return (clm_log_add_callback(clm_file_callback, fp, level));
}


void clm_log(i32 level, char *file, i32 line, char *fmt, ...)
{
    // clang-format off
    clm_log_record record = {
        .fmt = fmt,
        .file = file,
        .line = line,
        .level = level
    };
    // clang-format on

    clm_log_lock();

    if (!CLM_LOGGER.quiet && level >= CLM_LOGGER.level) {
        clm_init_record(&record, stderr);
        va_start(record.ap, fmt);
        clm_stdout_callback(&record);
        va_end(record.ap);
    }

    for (i32 i = 0; i < CLM_LOG_MAX_CALLBACKS && CLM_LOGGER.callbacks[i].fn; ++i) {
        callback_t *cb = &CLM_LOGGER.callbacks[i];
        if (level >= cb->level) {
            clm_init_record(&record, cb->data);
            va_start(record.ap, fmt);
            cb->fn(&record);
            va_end(record.ap);
        }
    }

    clm_log_unlock();
}

/*****************************************************************************************
 * IMPLEMENTATION END
 ****************************************************************************************/
