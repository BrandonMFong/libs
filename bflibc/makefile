# author: Brando
# date: 6/2/22
# https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

## Includes
include ../makefiles/platforms.mk
include makefiles/checksum.mk
include ../bflibc/makefiles/uuid.mk

## tools
CC = gcc # compiler
AR = ar # Archiver
COPY = cp # copy tool
COPY_FLAGS = -afv
REMOVE = rm # remove tool
REMOVE_FLAGS = -rfv

## Compile Flags

# Warnings
CFLAGS += -Wall -I.

# Objects
#
# All objects defined in headers should be copied to 
# bin
C_OBJECTS = filesystem coreutils stringutils bftime checksum thread lock filewriter

### RELEASE ###
R_CFLAGS = $(CFLAGS)
R_BUILD_PATH = build/release
R_BIN_PATH = bin/release/bflibc
R_BFLIBC_OUT = libbfc.a
R_C_OBJ_FILES = $(patsubst %, $(R_BUILD_PATH)/%.o, $(C_OBJECTS))

release: r-setup $(R_BIN_PATH)/$(R_BFLIBC_OUT)

r-setup:
	@mkdir -p $(R_BUILD_PATH)
	@mkdir -p $(R_BIN_PATH)

$(R_BIN_PATH)/$(R_BFLIBC_OUT): $(R_C_OBJ_FILES)
	$(COPY) $(COPY_FLAGS) src/*.h $(R_BIN_PATH)
	$(AR) -rsc $(R_BIN_PATH)/$(R_BFLIBC_OUT) $^

$(R_BUILD_PATH)/%.o: src/%.c src/%.h
	$(CC) -c -o $@ $< $(R_CFLAGS)

r-clean:
	rm -rfv $(R_BUILD_PATH)
	rm -rfv $(R_BIN_PATH)

### DEBUG ### 
D_CFLAGS = $(CFLAGS) -DDEBUG -g
D_BUILD_PATH = build/debug
D_BIN_PATH = bin/debug/bflibc
D_BFLIBC_OUT = libbfc-debug.a
D_C_OBJ_FILES = $(patsubst %, $(D_BUILD_PATH)/%.o, $(C_OBJECTS))

debug: d-setup $(D_BIN_PATH)/$(D_BFLIBC_OUT)

d-setup:
	@mkdir -p $(D_BUILD_PATH)
	@mkdir -p $(D_BIN_PATH)

$(D_BIN_PATH)/$(D_BFLIBC_OUT): $(D_C_OBJ_FILES)
	$(COPY) $(COPY_FLAGS) src/*.h $(D_BIN_PATH)
	$(AR) -rsc $(D_BIN_PATH)/$(D_BFLIBC_OUT) $^

$(D_BUILD_PATH)/%.o: src/%.c src/%.h
	$(CC) -c -o $@ $< $(D_CFLAGS)

d-clean:
	$(REMOVE) $(REMOVE_FLAGS) $(D_BUILD_PATH)
	$(REMOVE) $(REMOVE_FLAGS) $(D_BIN_PATH)

### TESTS ###
T_BFLIBC_OUT = test-bflibc
T_BIN_PATH = bin/test/bflibc

test: D_CFLAGS += $(CFLAGS) -DTESTING -Isrc/ $(BF_LIB_C_CHECKSUM_FLAGS) $(BF_LIB_C_UUID_FLAGS)
test: CC = @gcc
test: AR = @ar
test: COPY = @cp
test: REMOVE = @rm
test: REMOVE_FLAGS = -rf
test: COPY_FLAGS = -af
test: t-setup d-clean debug compile-tests
	./$(T_BIN_PATH)/$(T_BFLIBC_OUT)

t-setup:
	@mkdir -p $(T_BIN_PATH)

compile-tests:
	$(CC) -o $(T_BIN_PATH)/$(T_BFLIBC_OUT) testbench/tests.c $(D_BIN_PATH)/$(D_BFLIBC_OUT) $(D_CFLAGS) -lpthread

clean: r-clean d-clean
	$(REMOVE) $(REMOVE_FLAGS) build/
	$(REMOVE) $(REMOVE_FLAGS) bin

