## Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([rtems-c-src-tests-smptests],[_RTEMS_VERSION],[http://www.rtems.org/bugzilla])
AC_CONFIG_SRCDIR([smp01])
AC_CONFIG_HEADER([config.h])
RTEMS_TOP([../..],[..])

RTEMS_CANONICAL_TARGET_CPU

AM_INIT_AUTOMAKE([no-define foreign 1.12.2])
AM_MAINTAINER_MODE

RTEMS_ENABLE_CXX

RTEMS_ENV_RTEMSBSP

RTEMS_PROJECT_ROOT

RTEMS_PROG_CC_FOR_TARGET

RTEMS_PROG_CXX_FOR_TARGET
RTEMS_CANONICALIZE_TOOLS

RTEMS_CHECK_CPUOPTS([RTEMS_ATOMIC])
RTEMS_CHECK_CPUOPTS([RTEMS_POSIX_API])

RTEMS_CHECK_CUSTOM_BSP(RTEMS_BSP)
RTEMS_CHECK_CPUOPTS([RTEMS_MULTIPROCESSING])
RTEMS_CHECK_CXX(RTEMS_BSP)
RTEMS_CHECK_CPUOPTS([RTEMS_NETWORKING])
RTEMS_CHECK_CPUOPTS([RTEMS_SMP])

AM_CONDITIONAL(SMPTESTS,test "$rtems_cv_RTEMS_SMP" = "yes")

AM_CONDITIONAL([ATOMIC],[test x"$rtems_cv_RTEMS_ATOMIC" = xyes])
AM_CONDITIONAL([HAS_POSIX],[test x"${rtems_cv_RTEMS_POSIX_API}" = xyes])

# Explicitly list all Makefiles here
AC_CONFIG_FILES([Makefile
smp01/Makefile
smp02/Makefile
smp03/Makefile
smp05/Makefile
smp06/Makefile
smp07/Makefile
smp08/Makefile
smp09/Makefile
smpatomic01/Makefile
smpatomic02/Makefile
smpatomic03/Makefile
smpatomic04/Makefile
smpatomic05/Makefile
smpatomic06/Makefile
smpatomic07/Makefile
smplock01/Makefile
smpmigration01/Makefile
smppsxsignal01/Makefile
smpschedule01/Makefile
smpsignal01/Makefile
smpswitchextension01/Makefile
smpunsupported01/Makefile
])
AC_OUTPUT
