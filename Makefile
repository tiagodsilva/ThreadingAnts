# Makefile to compile the game 
GCC=g++ 
CFLAGS=-g -Wall -o 
LDFLAGS=-pthread 

COMPILED=main 
FILENAME=main.cpp 

INCLUDES=include
HEADERS=${INCLUDES}/headers.h 

.PHONY: ${COMPILED} 
all: ${COMPILED} 
	./${COMPILED} 
${COMPILED}: ${INCLUDES}/*.cpp   
	# Identify header files 
	echo "#ifndef __HEADERS__" > ${HEADERS} ; \
	echo "#define __HEADERS__" >> ${HEADERS} ; \
	for filename in $^; do \
		echo '#include "'$${$(basename filename##*/)}'"' >> ${HEADERS} ; \
	done ; \
	echo "#endif" >> ${HEADERS} ; \
	${GCC} ${LDFLAGS} ${FILENAME} ${CFLAGS} ${COMPILED} 
