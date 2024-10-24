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

# used to make universal binaries
MACOS_TARGET_X86_64 = x86_64-apple-macos10.12
MACOS_TARGET_ARM64 = arm64-apple-macos11

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

.PRECIOUS: $(R_BUILD_PATH)/%.$(MACOS_TARGET_X86_64) $(R_BUILD_PATH)/%.$(MACOS_TARGET_ARM64)

release: r-setup $(R_BIN_PATH)/$(R_BFLIBC_OUT)

r-setup:
	@mkdir -p $(R_BUILD_PATH)
	@mkdir -p $(R_BIN_PATH)

$(R_BIN_PATH)/$(R_BFLIBC_OUT): $(R_C_OBJ_FILES)
	$(COPY) $(COPY_FLAGS) src/*.h $(R_BIN_PATH)
	$(AR) -rsc $(R_BIN_PATH)/$(R_BFLIBC_OUT) $^

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
$(R_BUILD_PATH)/%.o: $(R_BUILD_PATH)/%.$(MACOS_TARGET_X86_64) $(R_BUILD_PATH)/%.$(MACOS_TARGET_ARM64)
	lipo -create -output $@ $^
else
$(R_BUILD_PATH)/%.o: src/%.c src/%.h
	$(CC) -c -o $@ $< $(R_CFLAGS)
endif

$(R_BUILD_PATH)/%.$(MACOS_TARGET_X86_64): src/%.c src/%.h
	$(CC) -c -o $@ $< $(R_CFLAGS) -target $(MACOS_TARGET_X86_64)

$(R_BUILD_PATH)/%.$(MACOS_TARGET_ARM64): src/%.c src/%.h
	$(CC) -c -o $@ $< $(R_CFLAGS) -target $(MACOS_TARGET_ARM64)

r-clean:
	rm -rfv $(R_BUILD_PATH)
	rm -rfv $(R_BIN_PATH)

### DEBUG ### 
D_CFLAGS = $(CFLAGS) -DDEBUG -g
D_BUILD_PATH = build/debug
D_BIN_PATH = bin/debug/bflibc
D_BFLIBC_OUT = libbfc-debug.a
D_C_OBJ_FILES = $(patsubst %, $(D_BUILD_PATH)/%.o, $(C_OBJECTS))

.PRECIOUS: $(D_BUILD_PATH)/%.$(MACOS_TARGET_X86_64) $(D_BUILD_PATH)/%.$(MACOS_TARGET_ARM64)

debug: d-setup $(D_BIN_PATH)/$(D_BFLIBC_OUT)

d-setup:
	@mkdir -p $(D_BUILD_PATH)
	@mkdir -p $(D_BIN_PATH)

$(D_BIN_PATH)/$(D_BFLIBC_OUT): $(D_C_OBJ_FILES)
	$(COPY) $(COPY_FLAGS) src/*.h $(D_BIN_PATH)
	$(AR) -rsc $(D_BIN_PATH)/$(D_BFLIBC_OUT) $^

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
$(D_BUILD_PATH)/%.o: $(D_BUILD_PATH)/%.$(MACOS_TARGET_X86_64) $(D_BUILD_PATH)/%.$(MACOS_TARGET_ARM64)
	lipo -create -output $@ $^
else
$(D_BUILD_PATH)/%.o: src/%.c src/%.h
	$(CC) -c -o $@ $< $(D_CFLAGS)
endif

$(D_BUILD_PATH)/%.$(MACOS_TARGET_X86_64): src/%.c src/%.h
	$(CC) -c -o $@ $< $(D_CFLAGS) -target $(MACOS_TARGET_X86_64)

$(D_BUILD_PATH)/%.$(MACOS_TARGET_ARM64): src/%.c src/%.h
	$(CC) -c -o $@ $< $(D_CFLAGS) -target $(MACOS_TARGET_ARM64)

d-clean:
	$(REMOVE) $(REMOVE_FLAGS) $(D_BUILD_PATH)
	$(REMOVE) $(REMOVE_FLAGS) $(D_BIN_PATH)

### TESTS ###
T_BFLIBC_OUT = test-bflibc
T_BIN_PATH = bin/test/bflibc
#T_SANITIZE_ADDR = -fsanitize=address

test: D_CFLAGS += $(CFLAGS) -DTESTING -Isrc/
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
	$(CC) -o $(T_BIN_PATH)/$(T_BFLIBC_OUT) testbench/tests.c $(D_BIN_PATH)/$(D_BFLIBC_OUT) $(D_CFLAGS) -lpthread $(BF_LIB_C_CHECKSUM_FLAGS) $(BF_LIB_C_UUID_FLAGS) $(T_SANITIZE_ADDR)

clean: r-clean d-clean
	$(REMOVE) $(REMOVE_FLAGS) build/
	$(REMOVE) $(REMOVE_FLAGS) bin

