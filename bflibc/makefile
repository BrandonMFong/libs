# author: Brando
# date: 6/2/22
#

include makefiles/lib.mk 
include makefiles/uuid.mk 

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
SOURCE_EXT = c
HEADER_EXT = h
FILES = filesystem coreutils stringutils bftime checksum thread lock filewriter

### Release settings
ifeq ($(CONFIG),release) # release
BIN_NAME = $(LIB_FILE_NAME_RELEASE)
FLAGS = $(CFLAGS) -Isrc/ -Wall
### Debug settings
else ifeq ($(CONFIG),debug) # debug
BIN_NAME = $(LIB_FILE_NAME_DEBUG)
ADDR_SANITIZER = -fsanitize=address
FLAGS = $(CFLAGS) -Wall -DDEBUG -g -Isrc/ $(ADDR_SANITIZER)
### Test settings
else ifeq ($(CONFIG),test) # test
BIN_NAME = libbfc-test
ADDR_SANITIZER = -fsanitize=address
FLAGS = $(CFLAGS) -Wall -DDEBUG -DTESTING -g -Isrc/ $(ADDR_SANITIZER) 
LIBRARIES =
LINKS = $(BF_LIB_C_UUID_FLAGS) -lpthread $(BF_LIB_C_CHECKSUM_FLAGS)
MAIN_FILE = testbench/tests.c
endif # ($(CONFIG),...)

include ../makefiles/build.mk 

