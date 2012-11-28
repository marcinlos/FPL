
AC_PREREQ([2.68])
AC_INIT([fplib], [1.0],, [fplib-1.0])
AC_CONFIG_SRCDIR([src/Makefile.am])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign])
AM_SILENT_RULES
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
LT_INIT


# Checks for typedefs, structures, and compiler characteristics.


# Checks for library functions.


AC_SUBST([CXXTEST_BIN_PATH], ["${CXX_BIN_PATH}"])
AC_SUBST([CXXTEST_INCLUDE_PATH], ["${CXX_INCLUDE_PATH}"])
AC_SUBST([CXXTESTGEN], ["${CXX_BIN_PATH}/cxxtestgen"])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    src/FPL/Makefile
    src/test/Makefile
    src/demo/Makefile
])
                 
AC_OUTPUT
