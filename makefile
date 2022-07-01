# author: Brando
# date: 6/2/22
# https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

## Compiler definitions
CC = gcc

## Compile Flags

# Includes

# Determine the OS
ifeq ($(OS),Windows_NT)
    CCFLAGS += -D WIN32
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
        CCFLAGS += -D AMD64
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
            CCFLAGS += -D AMD64
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
            CCFLAGS += -D IA32
        endif
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        CCFLAGS += -D LINUX
    endif
    ifeq ($(UNAME_S),Darwin)
        CCFLAGS += -D OSX
    endif
    UNAME_P := $(shell uname -p)
    ifeq ($(UNAME_P),x86_64)
        CCFLAGS += -D AMD64
    endif
    ifneq ($(filter %86,$(UNAME_P)),)
        CCFLAGS += -D IA32
    endif
    ifneq ($(filter arm%,$(UNAME_P)),)
        CCFLAGS += -D ARM
    endif
endif

# Warnings
CCFLAGS += -Wall

all: setup libc libcpp

setup:
	mkdir -p bin/

libc:
	$(CC) -c -o bin/clib.o clib/clib.c $(CCFLAGS)
	cp -afv clib/*.h bin/

libcpp:
	cp -afv cpplib/*.hpp bin/

clean:
	rm -rfv bin

test: setup libctests libcpptests runtests

# Tests
libctests:
	$(CC) -c -o bin/clib.o clib/clib.c $(CCFLAGS) -D TESTING
	$(CC) -o bin/runtests.c bin/clib.o clib/tests.c $(CCFLAGS) -D TESTING

libcpptests:
	g++ -o bin/runtests.cpp cpplib/tests.cpp $(CCFLAGS) -D TESTING

runtests:
	./bin/runtests.c
	./bin/runtests.cpp

