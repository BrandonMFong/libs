# author: Brando
# date: 6/2/22
#

include makefiles/lib.mk 

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

COMPILER = gcc
CONFIG = release
BUILD_TYPE = archive
SOURCE_EXT = c
HEADER_EXT = h
FILES = \
filesystem coreutils stringutils \
bftime thread bfmath \
lock filewriter hash \
rand map tree

### Release settings
ifeq ($(CONFIG),release) # release
BIN_NAME = $(LIB_FILE_NAME_RELEASE)
FLAGS = $(CFLAGS) -Isrc/ -Wall
### Debug settings
else ifeq ($(CONFIG),debug) # debug
BIN_NAME = $(LIB_FILE_NAME_DEBUG)
#ADDR_SANITIZER = -fsanitize=address
FLAGS = $(CFLAGS) -Wall -DDEBUG -g -Isrc/ $(ADDR_SANITIZER)
### Test settings
else ifeq ($(CONFIG),test) # test
BIN_NAME = libbfc-test
#ADDR_SANITIZER = -fsanitize=address
FLAGS = $(CFLAGS) -Wall -DDEBUG -DTESTING -g -Isrc/ $(ADDR_SANITIZER) -I../bftest/bin/debug
LIBRARIES = ../bftest/bin/debug/bftest/libbftest-debug.a
LINKS = $(BF_LIB_C_FLAGS) -lpthread
MAIN_FILE = testbench/tests.c
endif # ($(CONFIG),...)

dependencies:
	cd ../bftest && make clean && make build && make build CONFIG=debug

LIBS_MAKEFILES_PATH:=$(shell dirname $(CURDIR))/makefiles
include ../makefiles/build.mk 

