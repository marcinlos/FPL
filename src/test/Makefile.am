include testlist

AM_CPPFLAGS = -I ${top_srcdir}/src -I @CXXTEST_PATH@

TESTGEN_FLAGS := --part --error-printer
ROOT_FLAGS := --root --error-printer

test_list := ${wildcard *.test}
test_cpps := ${patsubst %.test,%.cpp,${test_list}}


MOSTLYCLEANFILES = ${test_sources}
BUILT_SOURCES = runner.cpp

noinst_PROGRAMS = test
test_LDADD = ../fplib/libfp.la
test_SOURCES := ${test_sources}

${test_cpps}: %.cpp: %.test
	@CXXTESTGEN@ ${TESTGEN_FLAGS} -o $@ $^
	
runner.cpp: ${test_cpps}
	@CXXTESTGEN@ ${ROOT_FLAGS} -o $@ 

    
    
