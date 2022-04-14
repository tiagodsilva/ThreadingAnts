# Makefile to compile the game 
GCC=clang++-11 
CFLAGS=-g -Wall -o
LDFLAGS=-pthread -lncurses -Iinclude 

COMPILED=main 
FILENAME=main.cpp 

INCLUDES=include
HEADERS=${INCLUDES}/headers.h 
SOURCE=src

ifeq ($(OS),Windows_NT) 
	$(info "This simulation is not designed for Windows! Use Linux, please.") 
else 
.PHONY: ${COMPILED} 
all: ${COMPILED} 
	./${COMPILED} 
${COMPILED}: ${SOURCE}/*.hpp   
	# Identify header files 
	echo "#ifndef __HEADERS__" > ${HEADERS} ; \
	echo "#define __HEADERS__" >> ${HEADERS} ; \
	for filename in $^; do \
		echo '#include "../'$${filename}'"' >> ${HEADERS} ; \
	done ; \

	for filename in ${INCLUDES}/*.hpp; do \
		echo '#include "'$${$(basename filename#${INCLUDES}/)}'"' >> ${HEADERS} ; \
	done ; \
	echo "#endif" >> ${HEADERS} ; \
	${GCC} ${LDFLAGS} ${FILENAME} ${CFLAGS} ${COMPILED} 
endif 
