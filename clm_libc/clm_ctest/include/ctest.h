#ifndef CLM_CTEST_GUARD_
#define CLM_CTEST_GUARD_

#include "clm_shared.h"

struct clm_test_result_t;
typedef struct clm_test_result_t clm_test_result;

/* NOTE: must be implemented by any given library that wants to perform tests */
struct clm_test_error_t;
typedef struct clm_test_error_t clm_test_error;

typedef i32 clm_test_error_t;

/******************************************************************************
 * Used by `CLM_CTEST_TEST_*` macros
 *****************************************************************************/
struct clm_test_result
{
        /** _always_ equal to `__func__` */
        byte const *caller_name;
        /** additional info message for logging */
        byte const *info;
        /** whether a test succeeded */
        bool success;

        /** convert a test result into a c-string */
        byte const *(*test_to_string)(clm_test_result);
        /** pretty prints a test result */
        void (*test_display)(clm_test_result);


        /** error code (usually an enum), must be implemented by user */
        clm_test_error_t *code;
        /** set a user-defined error code */
        void (*test_set_code)(clm_test_result *, clm_test_error_t *);
        /** get a user-defined error code */
        clm_test_error_t *(*test_get_code)(clm_test_result *);


        /** additional error data from a test */
        clm_test_error_t *error;
        /** set a user-defined error code */
        void (*test_set_error)(clm_test_result *, clm_test_error_t *);
        /** set a user-defined error code */
        clm_test_error_t *(*test_get_error)(clm_test_result *);
};

#ifndef CLM_CTEST_TEST_BEGIN

/******************************************************************************
 * Begin a scope used for unit testing
 *****************************************************************************/
#define CLM_CTEST_TEST_BEGIN(name)              \
        clm_test_result test_ #name(void)       \
        {                                       \
                clm_test_result result = { 0 }; \
                result.success         = TRUE;  \
                result.name            = __func__;

#endif /* CLM_CTEST_TEST_BEGIN */


#ifndef CLM_CTEST_TEST_END

/******************************************************************************
 * Ends the unit test scope
 *****************************************************************************/
#define CLM_CTEST_TEST_END(name) \
        return (result);         \
        }

#endif /* CLM_CTEST_TEST_END */

#endif /* CLM_CTEST_GUARD_ */
