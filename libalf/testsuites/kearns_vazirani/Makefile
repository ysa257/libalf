# $Id$
# ASO testsuite Makefile

.PHONY: all clean run jpegs test

PREFIX ?= /usr/local

CPPFLAGS+=-I../../include -I../../../libAMoRE++/include -I${PREFIX}/include -Wall -ggdb3

LDFLAGS+=-L../../src -L../../../libAMoRE++/src

#all: learn_regex learn_file

clean: nodata
	-rm -f *.o learn_regex

nodata:
	-rm -f *.dot *.jpg *.KVtree testlog

run:
	LD_LIBRARY_PATH=../../src ./learn_regex

jpegs:
	for EACH in *.dot; do dot -Tjpg "$$EACH" -o "$$EACH.jpg"; done;

learn_regex: learn_regex.cpp
	g++ ${CPPFLAGS} ${LDFLAGS} learn_regex.cpp -o learn_regex -static -lAMoRE++ -lalf 

learn_regex: learn_file.cpp