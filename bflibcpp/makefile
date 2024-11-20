# author: Brando
# date: 7/29/22
#

include ../bflibc/makefiles/lib.mk

help:
	@echo "Usage:"
	@echo "	make <targets> <variables>"
	@echo ""
	@echo "Target(s):"
	@echo "	clean			cleans build and bin folder"
	@echo "	build 			builds release verions"
	@echo ""
	@echo "Variable(s):"
	@echo "	CONFIG		use this to change the build config. Accepts \"release\" (default), \"debug\", or \"test\""

COMPILER = g++
CPPSTD = -std=c++20
CONFIG = release
BUILD_TYPE = archive
SOURCE_EXT = cpp
HEADER_EXT = hpp
FILES = \
data directory exception file object \
path string time

### Release settings
ifeq ($(CONFIG),release) # release
BIN_NAME = $(LIB_FILE_NAME_RELEASE)
FLAGS = $(CPPFLAGS) -Isrc/ $(CPPSTD) -I../bflibc/bin/release
### Debug settings
else ifeq ($(CONFIG),debug) # debug
BIN_NAME = $(LIB_FILE_NAME_DEBUG)
#ADDR_SANITIZER = -fsanitize=address
FLAGS = $(CPPFLAGS) -DDEBUG -g -Isrc/ $(ADDR_SANITIZER) $(CPPSTD) -I../bflibc/bin/debug
### Test settings
else ifeq ($(CONFIG),test) # test
BIN_NAME = libbfcpp-test
#ADDR_SANITIZER = -fsanitize=address
FLAGS = $(CPPFLAGS) -DDEBUG -DTESTING -g -Isrc/ $(ADDR_SANITIZER) $(CPPSTD) -I../bflibc/bin/debug -I../bftest/bin/debug 
LIBRARIES = ../bflibc/bin/debug/bflibc/libbfc-debug.a ../bftest/bin/debug/bftest/libbftest-debug.a
LINKS = $(BF_LIB_C_FLAGS) -lpthread
MAIN_FILE = testbench/tests.cpp
endif # ($(CONFIG),...)

dependencies:
	cd ../bflibc/ && make clean && make build && make build CONFIG=debug
	cd ../bftest/ && make clean && make build && make build CONFIG=debug

include makefiles/lib.mk 
LIBS_MAKEFILES_PATH:=$(shell dirname $(CURDIR))/makefiles
include ../makefiles/build.mk 

