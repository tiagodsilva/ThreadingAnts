# Makefile to check whether the objects' implementation is consistent 
GCC=g++ 
CFLAGS=-g -Wall -o 
LDFLAGS=-pthread 

FILENAME?=game.cpp
COMPILED=main 

.PHONY: ${COMPILED} 
all: ${COMPILED} 
	./${COMPILED}; \
	rm ${COMPILED} 
${COMPILED}: ${FILENAME} 
	${GCC} $^ ${LDFLAGS} ${CFLAGS} $@ 
