include testlist

AM_CPPFLAGS = -I ${top_srcdir}/src -I @CXXTEST_INCLUDE_PATH@

CXX_TYPE := --error-printer
TESTGEN_FLAGS := --part
ROOT_FLAGS := --root

test_list := ${wildcard *.test}
test_cpps := ${patsubst %.test,%.cpp,${test_list}}


MOSTLYCLEANFILES = ${test_sources}
BUILT_SOURCES = runner.cpp

noinst_PROGRAMS = test
test_LDADD = ../FPL/libfpl.la
test_SOURCES = ${test_sources}

${test_cpps}: %.cpp: %.test
	cxxtestgen ${TESTGEN_FLAGS} -o $@ $^ ${CXX_TYPE}
	
runner.cpp: ${test_cpps}
	cxxtestgen ${ROOT_FLAGS} -o $@ ${CXX_TYPE}

    
