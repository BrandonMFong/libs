# author: Brando
# date: 7/29/22
#

include makefiles/lib.mk 
include ../bflibc/makefiles/uuid.mk 

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
FILES = client server socket envelope connection buffer log

### Release settings
ifeq ($(CONFIG),release) # release
BIN_NAME = $(LIB_FILE_NAME_RELEASE)
FLAGS = $(CPPFLAGS) -Isrc/ $(CPPSTD) -I../bflibc/bin/release -I../bflibcpp/bin/release
### Debug settings
else ifeq ($(CONFIG),debug) # debug
BIN_NAME = $(LIB_FILE_NAME_DEBUG)
ADDR_SANITIZER = -fsanitize=address
FLAGS = $(CPPFLAGS) -DDEBUG -g -Isrc/ $(ADDR_SANITIZER) $(CPPSTD) -I../bflibc/bin/debug -I../bflibcpp/bin/debug
### Test settings
else ifeq ($(CONFIG),test) # test
BIN_NAME = libbfnet-test
ADDR_SANITIZER = -fsanitize=address
FLAGS = $(CPPFLAGS) -DDEBUG -DTESTING -g -Isrc/ $(ADDR_SANITIZER) $(CPPSTD) -I../bflibc/bin/debug -I../bflibcpp/bin/debug -I../bflibcpp/src
LIBRARIES = ../bflibc/bin/debug/bflibc/libbfc-debug.a ../bflibcpp/bin/debug/bflibcpp/libbfcpp-debug.a
CPPLINKS = $(BF_LIB_C_UUID_FLAGS)
endif # ($(CONFIG),...)

dependencies:
	cd ../bflibc/ && make debug release
	cd ../bflibcpp/ && make build CONFIG=release && make build CONFIG=debug

include ../makefiles/build.mk 

