/* compiletime extension for PHP */

#ifndef PHP_COMPILETIME_H
# define PHP_COMPILETIME_H

extern zend_module_entry compiletime_module_entry;
# define phpext_compiletime_ptr &compiletime_module_entry

# define PHP_COMPILETIME_VERSION "0.0.1"

# if defined(ZTS) && defined(COMPILE_DL_COMPILETIME)
ZEND_TSRMLS_CACHE_EXTERN()
# endif

#endif	/* PHP_COMPILETIME_H */

