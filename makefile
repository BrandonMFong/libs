# author: Brando
# date: 6/2/22
# https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

## Includes
include makefiles/platforms.mk

## Compiler definitions
CC = gcc
CPP = g++

## Archiver
AR = ar

## Compile Flags

# Warnings
CFLAGS += -Wall -Iclib/
CPPFLAGS += -Wall -Icpplib/ -Iclib/

# Standards
CPPFLAGS += -std=c++20

# Objects
C_OBJECTS = filesystem coreutils
CPP_OBJECTS = file

### RELEASE ###
R_CFLAGS = $(CFLAGS)
R_CPPFLAGS = $(CPPFLAGS)
R_BUILD_PATH = build/release
R_CLIB_OUT = clib.a
R_CPPLIB_OUT = cpplib.a

release: r-setup bin/$(R_CLIB_OUT) bin/$(R_CPPLIB_OUT)

r-setup:
	@mkdir -p $(R_BUILD_PATH)/c
	@mkdir -p $(R_BUILD_PATH)/cpp
	@mkdir -p bin/c
	@mkdir -p bin/cpp

bin/$(R_CLIB_OUT):
	$(CC) -c -o $(R_BUILD_PATH)/c/filesystem.o clib/filesystem.c $(R_CFLAGS)
	$(CC) -c -o $(R_BUILD_PATH)/c/coreutils.o clib/coreutils.c $(R_CFLAGS)
	cp -afv clib/*.h bin/c
	$(AR) -rsc bin/c/$(R_CLIB_OUT) $(R_BUILD_PATH)/c/coreutils.o $(R_BUILD_PATH)/c/filesystem.o

bin/$(R_CPPLIB_OUT):
	$(CPP) -c -o $(R_BUILD_PATH)/cpp/file.o cpplib/file.cpp $(R_CPPFLAGS)
	cp -afv cpplib/*.hpp bin/cpp
	$(AR) -rsc bin/cpp/$(R_CPPLIB_OUT) $(R_BUILD_PATH)/cpp/file.o

### DEBUG ### 
D_CFLAGS = $(CFLAGS) -DDEBUG -g
D_CPPFLAGS = $(CPPFLAGS) -DDEBUG -g
D_BUILD_PATH = build/debug
D_CLIB_OUT = debug-clib.a
D_CPPLIB_OUT = debug-cpplib.a

debug: d-setup bin/$(D_CLIB_OUT) bin/$(D_CPPLIB_OUT)

d-setup:
	@mkdir -p $(D_BUILD_PATH)/c
	@mkdir -p $(D_BUILD_PATH)/cpp
	@mkdir -p bin/c
	@mkdir -p bin/cpp

bin/$(D_CLIB_OUT):
	$(CC) -c -o $(D_BUILD_PATH)/c/filesystem.o clib/filesystem.c $(D_CFLAGS)
	$(CC) -c -o $(D_BUILD_PATH)/c/coreutils.o clib/coreutils.c $(D_CFLAGS)
	cp -afv clib/*.h bin/c
	$(AR) -rsc bin/c/$(D_CLIB_OUT) $(D_BUILD_PATH)/c/coreutils.o $(D_BUILD_PATH)/c/filesystem.o

bin/$(D_CPPLIB_OUT):
	$(CPP) -c -o $(D_BUILD_PATH)/cpp/file.o cpplib/file.cpp $(D_CPPFLAGS)
	cp -afv cpplib/*.hpp bin/cpp
	$(AR) -rsc bin/cpp/$(D_CPPLIB_OUT) $(D_BUILD_PATH)/cpp/file.o

### TESTS ###
T_CLIB_OUT = test-clib
T_CPPLIB_OUT = test-cpplib

test: D_CFLAGS = $(CFLAGS) -DTESTING -g
test: D_CPPFLAGS = $(CPPFLAGS) -DTESTING -g
test: debug compile-tests
	./bin/c/$(T_CLIB_OUT)
	./bin/cpp/$(T_CPPLIB_OUT)

compile-tests:
	$(CC) -o bin/c/$(T_CLIB_OUT) clib/testbench/tests.c bin/c/$(D_CLIB_OUT) $(D_CFLAGS)
	$(CPP) -o bin/cpp/$(T_CPPLIB_OUT) cpplib/testbench/tests.cpp bin/cpp/$(D_CPPLIB_OUT) bin/c/$(D_CLIB_OUT) $(D_CPPFLAGS)

clean:
	rm -rfv build/
	rm -rfv bin

