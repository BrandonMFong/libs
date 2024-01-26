# author: Brando
# date: 6/2/22
# https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

## Includes
include ../makefiles/platforms.mk
include ../bflibc/makefiles/uuid.mk

## Compiler definitions
CPP = g++

## Archiver
AR = ar

## Compile Flags

# Warnings
CPPFLAGS += -Wall -I.

# Standards
CPPFLAGS += -std=c++20

# Objects
#
# All objects defined in headers should be copied to 
# bin
CPP_OBJECTS = file string time path directory

### RELEASE ###
R_CPPFLAGS = $(CPPFLAGS) -I../bflibc/bin/release/ $(BF_LIB_C_UUID_FLAGS)
R_BUILD_PATH = build/release
R_BIN_PATH = bin/release/bflibcpp
R_BFLIBCPP_OUT = libbfcpp.a
R_BFLIBC_BIN_PATH = ../bflibc/bin/release/bflibc/libbfc.a
R_CPP_OBJ_FILES = $(patsubst %, $(R_BUILD_PATH)/%.o, $(CPP_OBJECTS))
R_C_OBJ_FILES = ../bflibc/build/release/*.o

release: r-setup build-release-c $(R_BIN_PATH)/$(R_BFLIBCPP_OUT)

build-release-c:
	@cd ../bflibc/ && make release
	cp -afv ../bflibc/bin/release/* ./bin/release/

r-setup:
	@mkdir -p $(R_BUILD_PATH)
	@mkdir -p $(R_BIN_PATH)

$(R_BIN_PATH)/$(R_BFLIBCPP_OUT): $(R_CPP_OBJ_FILES)
	cp -afv src/*.hpp $(R_BIN_PATH)
	$(AR) -rsc $(R_BIN_PATH)/$(R_BFLIBCPP_OUT) $^ $(R_BFLIBC_BIN_PATH)

$(R_BUILD_PATH)/%.o: src/%.cpp src/%.hpp
	$(CPP) -c -o $@ $< $(R_CPPFLAGS)

r-clean:
	rm -rfv $(R_BUILD_PATH)
	rm -rfv $(R_BIN_PATH)

### DEBUG ### 
D_CPPFLAGS = $(CPPFLAGS) -DDEBUG -g -I../bflibc/bin/debug/ 
D_BUILD_PATH = build/debug
D_BIN_PATH = bin/debug/bflibcpp
D_BFLIBCPP_OUT = libbfcpp-debug.a
D_BFLIBC_BIN_PATH = ../bflibc/bin/debug/bflibc/libbfc-debug.a
D_CPP_OBJ_FILES = $(patsubst %, $(D_BUILD_PATH)/%.o, $(CPP_OBJECTS))
D_C_OBJ_FILES = ../bflibc/build/debug/*.o

debug: d-setup build-debug-c $(D_BIN_PATH)/$(D_BFLIBCPP_OUT)

build-debug-c:
	@cd ../bflibc/ && make debug
	cp -afv ../bflibc/bin/debug/* ./bin/debug/

d-setup:
	@mkdir -p $(D_BUILD_PATH)
	@mkdir -p $(D_BIN_PATH)

$(D_BIN_PATH)/$(D_BFLIBCPP_OUT): $(D_CPP_OBJ_FILES)
	cp -afv src/*.hpp $(D_BIN_PATH)
	$(AR) -rsc $(D_BIN_PATH)/$(D_BFLIBCPP_OUT) $^ $(D_BFLIBC_BIN_PATH)

$(D_BUILD_PATH)/%.o: src/%.cpp src/%.hpp
	$(CPP) -c -o $@ $< $(D_CPPFLAGS)

d-clean:
	rm -rfv $(D_BUILD_PATH)
	rm -rfv $(D_BIN_PATH)

### TESTS ###
T_BFLIBCPP_OUT = test-bflibcpp
T_BIN_PATH = bin/test/bflibcpp

test: D_CPPFLAGS += $(CPPFLAGS) -DTESTING -Isrc/ -lpthread $(BF_LIB_C_UUID_FLAGS)
test: d-clean t-setup debug compile-tests
	./$(T_BIN_PATH)/$(T_BFLIBCPP_OUT)

t-setup:
	@mkdir -p $(T_BIN_PATH)

compile-tests:
	$(CPP) -o $(T_BIN_PATH)/$(T_BFLIBCPP_OUT) testbench/tests.cpp $(D_BIN_PATH)/$(D_BFLIBCPP_OUT) $(D_CPPFLAGS)

clean: r-clean d-clean
	rm -rfv build/
	rm -rfv bin

