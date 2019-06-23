dnl config.m4 for extension compiletime

PHP_ARG_ENABLE(compiletime, whether to enable compiletime support,
[  --enable-compiletime          Enable compiletime support], no)

if test "$PHP_COMPILETIME" != "no"; then
    AC_CHECK_FUNCS(gettimeofday)
    AC_CHECK_FUNCS(clock_gettime)
    PHP_SUBST(COMPILETIME_SHARED_LIBADD)
    PHP_NEW_EXTENSION(compiletime, compiletime.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi

