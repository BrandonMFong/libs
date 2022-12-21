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
#
# All objects defined in headers should be copied to 
# bin
C_OBJECTS = filesystem coreutils stringutils
CPP_OBJECTS = file string

### RELEASE ###
R_CFLAGS = $(CFLAGS)
R_CPPFLAGS = $(CPPFLAGS)
R_BUILD_PATH = build/release
R_CLIB_OUT = clib.a
R_CPPLIB_OUT = cpplib.a
R_C_OBJ_FILES = $(patsubst %, $(R_BUILD_PATH)/c/%.o, $(C_OBJECTS))
R_CPP_OBJ_FILES = $(patsubst %, $(R_BUILD_PATH)/cpp/%.o, $(CPP_OBJECTS))

release: r-setup bin/c/$(R_CLIB_OUT) bin/cpp/$(R_CPPLIB_OUT)

r-setup:
	@mkdir -p $(R_BUILD_PATH)/c
	@mkdir -p $(R_BUILD_PATH)/cpp
	@mkdir -p bin/c
	@mkdir -p bin/cpp

bin/c/$(R_CLIB_OUT): $(R_C_OBJ_FILES)
	cp -afv clib/*.h bin/c
	$(AR) -rsc bin/c/$(R_CLIB_OUT) $^

$(R_BUILD_PATH)/c/%.o: clib/%.c clib/%.h
	$(CC) -c -o $@ $< $(R_CFLAGS)

bin/cpp/$(R_CPPLIB_OUT): $(R_CPP_OBJ_FILES) $(R_C_OBJ_FILES)
	cp -afv cpplib/*.hpp bin/cpp
	cp -afv clib/*.h bin/cpp
	$(AR) -rsc bin/cpp/$(R_CPPLIB_OUT) $^

$(R_BUILD_PATH)/cpp/%.o: cpplib/%.cpp cpplib/%.hpp
	$(CPP) -c -o $@ $< $(R_CPPFLAGS)

### DEBUG ### 
D_CFLAGS = $(CFLAGS) -DDEBUG -g
D_CPPFLAGS = $(CPPFLAGS) -DDEBUG -g
D_BUILD_PATH = build/debug
D_CLIB_OUT = debug-clib.a
D_CPPLIB_OUT = debug-cpplib.a
D_C_OBJ_FILES = $(patsubst %, $(D_BUILD_PATH)/c/%.o, $(C_OBJECTS))
D_CPP_OBJ_FILES = $(patsubst %, $(D_BUILD_PATH)/cpp/%.o, $(CPP_OBJECTS))

debug: d-setup bin/c/$(D_CLIB_OUT) bin/cpp/$(D_CPPLIB_OUT)

d-setup:
	@mkdir -p $(D_BUILD_PATH)/c
	@mkdir -p $(D_BUILD_PATH)/cpp
	@mkdir -p bin/c
	@mkdir -p bin/cpp

bin/c/$(D_CLIB_OUT): $(D_C_OBJ_FILES)
	cp -afv clib/*.h bin/c
	$(AR) -rsc bin/c/$(D_CLIB_OUT) $^

$(D_BUILD_PATH)/c/%.o: clib/%.c clib/%.h
	$(CC) -c -o $@ $< $(D_CFLAGS)

bin/cpp/$(D_CPPLIB_OUT): $(D_CPP_OBJ_FILES) $(D_C_OBJ_FILES)
	cp -afv cpplib/*.hpp bin/cpp
	cp -afv clib/*.h bin/cpp
	$(AR) -rsc bin/cpp/$(D_CPPLIB_OUT) $^

$(D_BUILD_PATH)/cpp/%.o: cpplib/%.cpp cpplib/%.hpp
	$(CPP) -c -o $@ $< $(D_CPPFLAGS)

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

