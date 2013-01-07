AM_CPPFLAGS = -I ${top_srcdir}/src



BUILT_SOURCES = parser.h

AM_YFLAGS = -d 

noinst_PROGRAMS = fplshell
fplshell_LDADD = ../FPL/libfpl.la -lreadline -lm
fplshell_SOURCES = \
	main.c			\
	parser.y 		\
	lexer.l 		\
	ast.c 			\
	eval.c 			\
	value_list.c 	\
	symbols.c	 	\
	value.c			\
	builtins.c		