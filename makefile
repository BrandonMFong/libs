# author: Brando
# date: 6/2/22
# https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

## Includes
include makefiles/platforms.mk

## Compiler definitions
CC = gcc
CPP = g++

## Compile Flags

# Warnings
CCFLAGS += -Wall -Iclib/
CPPFLAGS += -Wall -Iclib/

# Standards
CPPFLAGS += -std=c++20

## Other
BUILD_PATH = build/release
CLIB_OUT = clib.a
CPPLIB_OUT = cpplib.a

### RELEASE ###
all: setup libc libcpp archive

### DEBUG ### 
debug: CFLAGS += -DDEBUG
debug: CPPFLAGS += -DDEBUG
debug: BUILD_PATH = build/debug
debug: CLIB_OUT = debug-clib.a
debug: CPPLIB_OUT = debug-cpplib.a
debug: setup libc libcpp archive

### TESTS ###
test: CFLAGS += -DTESTING
test: CPPFLAGS += -DTESTING
test: BUILD_PATH = build/tests
test: CLIB_OUT = test-clib
test: CPPLIB_OUT = test-cpplib
test: setup libc libcpp compile
	./bin/$(CLIB_OUT)
	./bin/$(CPPLIB_OUT)

setup:
	mkdir -p $(BUILD_PATH)/c
	mkdir -p $(BUILD_PATH)/cpp
	mkdir -p bin/

libc:
	$(CC) -c -o $(BUILD_PATH)/c/coreutils.o clib/coreutils.c $(CCFLAGS)
	$(CC) -c -o $(BUILD_PATH)/c/filesystem.o clib/filesystem.c $(CCFLAGS)
	cp -afv clib/*.h bin/

libcpp:
	$(CPP) -c -o $(BUILD_PATH)/cpp/file.o cpplib/file.cpp $(CPPFLAGS)
	cp -afv cpplib/*.hpp bin/

archive:
	ar -rsc bin/$(CLIB_OUT) $(BUILD_PATH)/c/coreutils.o $(BUILD_PATH)/c/filesystem.o
	ar -rsc bin/$(CPPLIB_OUT) $(BUILD_PATH)/cpp/file.o $(BUILD_PATH)/c/coreutils.o $(BUILD_PATH)/c/filesystem.o

compile:
	$(CC) -o bin/$(CLIB_OUT) clib/testbench/tests.c $(BUILD_PATH)/c/coreutils.o $(BUILD_PATH)/c/filesystem.o $(CCFLAGS)
	$(CPP) -o bin/$(CPPLIB_OUT) cpplib/testbench/tests.cpp $(BUILD_PATH)/cpp/file.o $(BUILD_PATH)/c/coreutils.o $(BUILD_PATH)/c/filesystem.o $(CPPFLAGS) 

clean:
	rm -rfv build/
	rm -rfv bin

