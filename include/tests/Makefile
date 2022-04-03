# Makefile to check whether the objects' implementation is consistent 
GCC=g++ 
CFLAGS=-g -Wall -o 
LDFLAGS=-pthread 

FILENAME=testAnts.cpp  
COMPILED=main 

all: ${COMPILED} 
	./${COMPILED}; \
	rm ${COMPILED} 
${COMPILED}: ${FILENAME} 
	${GCC} $^ ${LDFLAGS} ${CFLAGS} $@ 
