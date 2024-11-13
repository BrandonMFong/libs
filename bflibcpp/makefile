# author: Brando
# date: 7/29/22
#

include makefiles/lib.mk 
include ../makefiles/libpaths.mk 
include ../makefiles/platforms.mk 
include ../bflibc/makefiles/uuid.mk 

.SECONDEXPANSION:

UNAME_S := $(shell uname -s)

CONFIG = release
CPPSTD = -std=c++20
BUILD_PATH = build/$(CONFIG)
BIN_PATH = bin/$(CONFIG)

# used to make universal binaries
MACOS_TARGET_X86_64 = x86_64-apple-macos10--12
MACOS_TARGET_ARM64 = arm64-apple-macos11

FILES = \
data directory exception file object \
path string time

### Release settings
ifeq ($(CONFIG),release) # release
CPPFLAGS += -Isrc/ $(CPPSTD) -I../bflibc/bin/release
BIN_NAME = $(LIB_FILE_NAME_RELEASE)
OBJECTS = $(patsubst %, $(BUILD_PATH)/%.o, $(FILES))
ifeq ($(UNAME_S),Darwin)
OBJECTS_MACOS_TARGET_X86_64 = $(patsubst %, $(BUILD_PATH)/%.$(MACOS_TARGET_X86_64), $(FILES))
OBJECTS_MACOS_TARGET_ARM64 = $(patsubst %, $(BUILD_PATH)/%.$(MACOS_TARGET_ARM64), $(FILES))
OBJECTS_MACOS_TARGETS = $(OBJECTS_MACOS_TARGET_X86_64) $(OBJECTS_MACOS_TARGET_ARM64)
BIN_MACOS_TARGETS = $(BIN_PATH)/$(BIN_NAME).$(MACOS_TARGET_X86_64) $(BIN_PATH)/$(BIN_NAME).$(MACOS_TARGET_ARM64)
endif # ($(UNAME_S),...)

### Debug settings
else ifeq ($(CONFIG),debug) # debug
ADDR_SANITIZER = -fsanitize=address
CPPFLAGS += -DDEBUG -g -Isrc/ $(ADDR_SANITIZER) $(CPPSTD) -I../bflibc/bin/debug
BIN_NAME = $(LIB_FILE_NAME_DEBUG)
OBJECTS = $(patsubst %, $(BUILD_PATH)/%.o, $(FILES))
ifeq ($(UNAME_S),Darwin)
OBJECTS_MACOS_TARGET_X86_64 = $(patsubst %, $(BUILD_PATH)/%.$(MACOS_TARGET_X86_64), $(FILES))
OBJECTS_MACOS_TARGET_ARM64 = $(patsubst %, $(BUILD_PATH)/%.$(MACOS_TARGET_ARM64), $(FILES))
OBJECTS_MACOS_TARGETS = $(OBJECTS_MACOS_TARGET_X86_64) $(OBJECTS_MACOS_TARGET_ARM64)
BIN_MACOS_TARGETS = $(BIN_PATH)/$(BIN_NAME).$(MACOS_TARGET_X86_64) $(BIN_PATH)/$(BIN_NAME).$(MACOS_TARGET_ARM64)
endif # ($(UNAME_S),...)

### Test settings
else ifeq ($(CONFIG),test) # test
ADDR_SANITIZER = -fsanitize=address
CPPFLAGS += -DDEBUG -DTESTING -g -Isrc/ $(ADDR_SANITIZER) $(CPPSTD) -I../bflibc/bin/debug
BIN_NAME = libbfcpp-test
MAIN_FILE = testbench/tests.cpp
LIBRARIES += ../bflibc/bin/debug/bflibc/libbfc-debug.a 
BIN_PREREQS := $(wildcard testbench/*.hpp)
CPPLINKS = $(BF_LIB_C_UUID_FLAGS)
OBJECTS = $(patsubst %, $(BUILD_PATH)/%.o, $(FILES))
ifeq ($(UNAME_S),Darwin)
MAIN_OBJECT_MACOS_TARGET_X86_64 = $(BUILD_PATH)/tests.$(MACOS_TARGET_X86_64)
MAIN_OBJECT_MACOS_TARGET_ARM64 = $(BUILD_PATH)/tests.$(MACOS_TARGET_ARM64)
MAIN_OBJECT_MACOS_TARGETS = $(MAIN_OBJECT_MACOS_TARGET_X86_64) $(MAIN_OBJECT_MACOS_TARGET_ARM64)
OBJECTS_MACOS_TARGET_X86_64 = $(patsubst %, $(BUILD_PATH)/%.$(MACOS_TARGET_X86_64), $(FILES))
OBJECTS_MACOS_TARGET_ARM64 = $(patsubst %, $(BUILD_PATH)/%.$(MACOS_TARGET_ARM64), $(FILES))
OBJECTS_MACOS_TARGETS = $(OBJECTS_MACOS_TARGET_X86_64) $(OBJECTS_MACOS_TARGET_ARM64)
BIN_MACOS_TARGETS = $(BIN_PATH)/$(BIN_NAME).$(MACOS_TARGET_X86_64) $(BIN_PATH)/$(BIN_NAME).$(MACOS_TARGET_ARM64)
else
MAIN_OBJECT = $(BUILD_PATH)/tests.o
endif
endif # ($(CONFIG),...)

.PRECIOUS: \
	$(BUILD_PATH)/%.$(MACOS_TARGET_X86_64) $(BUILD_PATH)/%.$(MACOS_TARGET_ARM64) \
	$(BUILD_PATH)/%.$(MACOS_TARGET_X86_64) $(BUILD_PATH)/%.$(MACOS_TARGET_ARM64) \
	$(BUILD_PATH)/%.$(MACOS_TARGET_X86_64) $(BUILD_PATH)/%.$(MACOS_TARGET_ARM64)

build: build-bflibc setup $(BIN_PATH)/$(BIN_NAME)

ifeq ($(CONFIG), test)
run:
	./$(BIN_PATH)/$(BIN_NAME)
endif

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

SETUP_DIRS = $(BIN_PATH) $(BUILD_PATH)
setup: $(SETUP_DIRS)
$(SETUP_DIRS):
	mkdir -p $@

clean:
	rm -rfv build bin $(PACKAGE_NAME)

### Main build

ifeq ($(UNAME_S),Darwin)
ifeq ($(CONFIG),test)
$(BIN_PATH)/$(BIN_NAME): $(BIN_MACOS_TARGETS)
	lipo -create -output $@ $^

$(BIN_MACOS_TARGETS): $(MAIN_FILE) $(OBJECTS_MACOS_TARGETS) $(BIN_PREREQS)
	g++ -o $@ $< $(wildcard $(BUILD_PATH)/*$(suffix $@)) $(CPPFLAGS) $(CPPLINKS) $(LIBRARIES) -target $(subst --,.,$(subst .,,$(suffix $@)))
else # ($(CONFIG),...)
$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	cp -afv src/*.hpp $(BIN_PATH)
	ar rsc $@ $^

$(BUILD_PATH)/%.o: $(BUILD_PATH)/%.$(MACOS_TARGET_X86_64) $(BUILD_PATH)/%.$(MACOS_TARGET_ARM64)
	lipo -create -output $@ $^
endif # ($(CONFIG), test)

$(OBJECTS_MACOS_TARGETS): $$(subst $(BUILD_PATH), src, $$(subst $$(suffix $$@),, $$@)).cpp  $$(subst $(BUILD_PATH), src, $$(subst $$(suffix $$@),, $$@)).hpp
	g++ -c -o $@ $< $(CPPFLAGS) -target $(subst --,.,$(subst .,,$(suffix $@)))

else # ($(UNAME_S),Linux)

ifeq ($(CONFIG),test)
$(BIN_PATH)/$(BIN_NAME): $(MAIN_FILE) $(OBJECTS) $(BIN_PREREQS)
	g++ -o $@ $< $(OBJECTS) $(LIBRARIES) $(CPPFLAGS) $(CPPLINKS)
else # ($(CONFIG),...)
$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	cp -afv src/*.hpp $(BIN_PATH)
	ar rsc $@ $^
endif # ($(CONFIG), test)

$(BUILD_PATH)/%.o: src/%.cpp src/%.hpp
	g++ -c $< -o $@ $(CPPFLAGS)

endif # ($(UNAME_S),...)

build-bflibc:
	cd ../bflibc/ && make debug release

