/* compiletime extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <time.h>
#include <signal.h>
#include <pthread.h>
#include "php.h"
#include "ext/standard/info.h"
#include "php_compiletime.h"

#define COMPILETIME_G(e) php_compiletime_globals.e

typedef struct _php_compiletime_globals_t {
    zend_long compile_time;
} php_compiletime_globals_t;

ZEND_TLS php_compiletime_globals_t php_compiletime_globals;

ZEND_API zend_op_array *(*_zend_compile_file)(zend_file_handle *file_handle, int type);
ZEND_DLEXPORT zend_op_array * compiletime_compile_file (zend_file_handle *file_handle, int type);

static zend_long time_milliseconds()
{
#if HAVE_CLOCK_GETTIME
    struct timespec s;

    if (clock_gettime(CLOCK_MONOTONIC, &s) == 0) {
        return s.tv_sec * 1000000 + s.tv_nsec / 1000;
    } else {
        struct timeval now;
        if (gettimeofday(&now, NULL) == 0) {
            return now.tv_sec * 1000000 + now.tv_usec;
        }
    }
#elif HAVE_GETTIMEOFDAY
    struct timeval now;
    if (gettimeofday(&now, NULL) == 0) {
        return now.tv_sec * 1000000 + now.tv_usec;
    }
#endif
    return 0;
}

ZEND_DLEXPORT zend_op_array * compiletime_compile_file (zend_file_handle *file_handle, int type)
{
    zend_op_array *ret;
    zend_long start = time_milliseconds();

    ret = _zend_compile_file (file_handle, type);

    COMPILETIME_G(compile_time) += (time_milliseconds() - start);

    return ret;
}

PHP_FUNCTION(compiletime_get_current)
{
    RETURN_LONG(COMPILETIME_G(compile_time));
}

PHP_MINIT_FUNCTION(compiletime)
{
    _zend_compile_file = zend_compile_file;
    zend_compile_file = compiletime_compile_file;

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(compiletime)
{
    return SUCCESS;
}

PHP_RINIT_FUNCTION(compiletime)
{
#if defined(ZTS) && defined(COMPILE_DL_COMPILETIME)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif

    COMPILETIME_G(compile_time) = 0;

    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(compiletime)
{
    return SUCCESS;
}

PHP_MINFO_FUNCTION(compiletime)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "compiletime support", "enabled");
    php_info_print_table_end();
}

zend_function_entry php_compiletime_functions[] = {
    PHP_FE(compiletime_get_current, NULL)
    PHP_FE_END
};

zend_module_entry compiletime_module_entry = {
    STANDARD_MODULE_HEADER,
    "compiletime",
    php_compiletime_functions,
    PHP_MINIT(compiletime),
    PHP_MSHUTDOWN(compiletime),
    PHP_RINIT(compiletime),
    PHP_RSHUTDOWN(compiletime),
    PHP_MINFO(compiletime),
    PHP_COMPILETIME_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_COMPILETIME
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(compiletime)
#endif
