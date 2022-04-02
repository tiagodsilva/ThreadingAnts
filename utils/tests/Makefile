# Makefile to test the utils 
GCC=g++ 
CFLAGS=-Wall -g -o 
LDFLAGS=-pthread 

FILENAME=tests_geometry.cpp 
COMPILED=main 

all: ${COMPILED} 
	./${COMPILED}; \
	rm ${COMPILED} 
${COMPILED}: ${FILENAME} 
	${GCC} ${LDFLAGS} $^ ${CFLAGS} $@ 

