dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(MDST, 0.1)


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()

CFLAGS="$CFLAGS -Wall"

AC_PROG_RANLIB
AC_PROG_CC

##########################################################################
# profiling compilation support
##########################################################################
AC_MSG_CHECKING([whether to build with profiling information])
AC_ARG_ENABLE([profiling],
    [AS_HELP_STRING([--enable-profiling],
        [enable profiling data generation (def=no)])],
    [profilingit="$enableval"],
    [profilingit=no])
AC_MSG_RESULT([$profilingit])

if test x"$profilingit" = x"yes"; then
    if test x"$CC" = x"gcc"; then
    	CFLAGS="$CFLAGS -pg"
    else
    	CFLAGS="$CFLAGS -pg"
    fi
fi
##########################################################################

##########################################################################
# debug compilation support
##########################################################################
AC_MSG_CHECKING([whether to build with debug information])
AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--enable-debug],
        [enable debug data generation (def=no)])],
    [debugit="$enableval"],
    [debugit=no])
AC_MSG_RESULT([$debugit])

if test x"$debugit" = x"yes"; then
    AC_DEFINE([DEBUG])
    if test x"$CC" = x"gcc"; then
    	CFLAGS="$CFLAGS -g -O0 -Wno-uninitialized -DDEBUG"
    else
    	CFLAGS="$CFLAGS -g -Wno-uninitialized"
    fi
else
    if test x"$CC" = x"gcc"; then
    	CFLAGS="$CFLAGS -O3"
    fi
fi
##########################################################################

AC_CONFIG_FILES(Makefile lib/Makefile tools/Makefile receiver/Makefile sender/Makefile)
AC_OUTPUT

