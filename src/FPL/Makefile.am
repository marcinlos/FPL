AM_CPPFLAGS = -I ${top_srcdir}/src
AM_CFLAGS = -Wall -Wno-unused-variable

lib_LTLIBRARIES = libfpl.la

libfpl_la_SOURCES =         \
    classification.c        \
    ieee_conversions.c      \
    sign_operations.c       \
    arithmetic.c            \
    compare.c				\
    interoperability.c		\
    conversions.c			\
	exponent.c              \
	logarythm.c             \
	sqrt.c					\
	poly_eval.c				\
	io.c					\
	sincos.c				\
	atan.c					\
    utils.c


libfpl_la_includedir = ${includedir}/FPL
libfpl_la_include_HEADERS = \
    float.h                 \
    interoperability.h 		\
    functions.h				\
    io.h
