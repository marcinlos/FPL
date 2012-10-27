
AC_PREREQ([2.68])
AC_INIT([fplib], [1.0],, [fplib-1.0])
AC_CONFIG_SRCDIR([src/Makefile.am])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign])
AM_SILENT_RULES([yes])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
LT_INIT


# Checks for typedefs, structures, and compiler characteristics.


# Checks for library functions.


AC_SUBST([CXXTEST_PATH], ["${CXX_PATH}"])
AC_SUBST([CXXTESTGEN], ["${CXX_PATH}/bin/cxxtestgen"])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    src/fplib/Makefile
    src/test/Makefile
])
                 
AC_OUTPUT
