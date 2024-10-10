# author: Brando
# date: 6/2/22
# https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/

## Includes
include ../makefiles/platforms.mk
include ../bflibc/makefiles/uuid.mk

LIB_NAME = bfnet

## tools
CPP = g++
AR = ar
CP = cp
RM = rm

## Compile Flags

# Warnings
CPPFLAGS += -Wall -I.

# Standards
CPPFLAGS += -std=c++20

# Objects
#
# All objects defined in headers should be copied to 
# bin
CPP_OBJECTS = client server socket envelope connection buffer

### RELEASE ###
R_CPPFLAGS = $(CPPFLAGS) -Ibin/release/ -I../bflibc/bin/release -I../bflibcpp/bin/release $(BF_LIB_C_UUID_FLAGS)
R_BUILD_PATH = build/release
R_BIN_PATH = bin/release/$(LIB_NAME)
R_BFLIBCPP_OUT = lib$(LIB_NAME).a
R_NET_OBJ_FILES = $(patsubst %, $(R_BUILD_PATH)/%.o, $(CPP_OBJECTS))
R_C_OBJ_FILES = ../bflibc/build/release/*.o
R_CPP_OBJ_FILES = ../bflibcpp/build/release/*.o

release: r-setup build-release-c build-release-cpp $(R_BIN_PATH)/$(R_BFLIBCPP_OUT)

all: release debug

build-release-c:
	@cd ../bflibc/ && make release

build-release-cpp:
	@cd ../bflibcpp/ && make release

r-setup:
	@mkdir -p $(R_BUILD_PATH)
	@mkdir -p $(R_BIN_PATH)

$(R_BIN_PATH)/$(R_BFLIBCPP_OUT): $(R_NET_OBJ_FILES)
	cp -afv src/*.hpp $(R_BIN_PATH)
	$(AR) rsc $(R_BIN_PATH)/$(R_BFLIBCPP_OUT) $^ $(R_C_OBJ_FILES) $(R_CPP_OBJ_FILES)

$(R_BUILD_PATH)/%.o: src/%.cpp src/%.hpp
	$(CPP) -c -o $@ $< $(R_CPPFLAGS)

r-clean:
	rm -rfv $(R_BUILD_PATH)
	rm -rfv $(R_BIN_PATH)

### DEBUG ### 
D_CPPFLAGS = $(CPPFLAGS) -DDEBUG -g -Ibin/debug/ -I../bflibc/bin/debug -I../bflibcpp/bin/debug
D_BUILD_PATH = build/debug
D_BIN_PATH = bin/debug/$(LIB_NAME)
D_BFLIBCPP_OUT = lib$(LIB_NAME)-debug.a
D_NET_OBJ_FILES = $(patsubst %, $(D_BUILD_PATH)/%.o, $(CPP_OBJECTS))
D_C_OBJ_FILES = ../bflibc/build/debug/*.o
D_CPP_OBJ_FILES = ../bflibcpp/build/debug/*.o

debug: d-setup build-debug-c build-debug-cpp $(D_BIN_PATH)/$(D_BFLIBCPP_OUT)

build-debug-c:
	@cd ../bflibc/ && make debug

build-debug-cpp:
	@cd ../bflibcpp/ && make debug

d-setup:
	@mkdir -p $(D_BUILD_PATH)
	@mkdir -p $(D_BIN_PATH)

$(D_BIN_PATH)/$(D_BFLIBCPP_OUT): $(D_NET_OBJ_FILES)
	$(CP) -afv src/*.hpp $(D_BIN_PATH)
	$(AR) rsc $(D_BIN_PATH)/$(D_BFLIBCPP_OUT) $^ $(D_C_OBJ_FILES) $(D_CPP_OBJ_FILES)

$(D_BUILD_PATH)/%.o: src/%.cpp src/%.hpp
	$(CPP) -c -o $@ $< $(D_CPPFLAGS)

d-clean:
	$(RM) -rfv $(D_BUILD_PATH)
	$(RM) -rfv $(D_BIN_PATH)

### TESTS ###
T_BFLIBCPP_OUT = test-$(LIB_NAME)
T_BIN_PATH = bin/test/$(LIB_NAME)
#T_SANITIZE_ADDR = -fsanitize=address

test: D_CPPFLAGS += -DTESTING -Isrc/ -lpthread $(BF_LIB_C_UUID_FLAGS) $(T_SANITIZE_ADDR) -I../bflibcpp/src
test: AR = @ar
test: CP = @cp
test: RM = @rm
test: d-clean t-setup debug compile-tests
	./$(T_BIN_PATH)/$(T_BFLIBCPP_OUT)

t-setup:
	@mkdir -p $(T_BIN_PATH)

compile-tests: testbench/*.hpp
	$(CPP) -o $(T_BIN_PATH)/$(T_BFLIBCPP_OUT) testbench/tests.cpp $(D_BIN_PATH)/$(D_BFLIBCPP_OUT) $(D_CPPFLAGS)

clean: r-clean d-clean
	$(RM) -rfv build/
	$(RM) -rfv bin

clean-all: clean
	@cd ../bflibc/ && make clean
	@cd ../bflibcpp/ && make clean

