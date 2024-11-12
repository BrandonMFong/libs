# author: Brando
# date: 7/29/22
#

include ../makefiles/libpaths.mk 
include ../makefiles/platforms.mk 
include ../bflibc/makefiles/uuid.mk 

.SECONDEXPANSION:

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
PLATFORM = linux
PACKAGE_MODE = package-linux
endif
ifeq ($(UNAME_S),Darwin)
PLATFORM = macos
PACKAGE_MODE = package-macos
endif

CONFIG = release
CPPLINKS = 
CPPSTD = -std=c++20
PACKAGE_NAME = bflibcpp 

# used to make universal binaries
MACOS_TARGET_X86_64 = x86_64-apple-macos10--12
MACOS_TARGET_ARM64 = arm64-apple-macos11

FILES = \
data directory exception file object \
path string time

### Release settings
ifeq ($(CONFIG), release) # release
CPPFLAGS += -Isrc/ \
	$(CPPSTD)
BIN_NAME = libbfcpp.a
BUILD_PATH = build/release
BIN_PATH = bin/release
LIBRARIES += 
ifeq ($(UNAME_S),Darwin)
OBJECTS_MACOS_TARGET_X86_64 = $(patsubst %, $(BUILD_PATH)/%.$(MACOS_TARGET_X86_64), $(FILES))
OBJECTS_MACOS_TARGET_ARM64 = $(patsubst %, $(BUILD_PATH)/%.$(MACOS_TARGET_ARM64), $(FILES))
OBJECTS_MACOS_TARGETS = $(OBJECTS_MACOS_TARGET_X86_64) $(OBJECTS_MACOS_TARGET_ARM64)
BIN_MACOS_TARGETS = $(BIN_PATH)/$(BIN_NAME).$(MACOS_TARGET_X86_64) $(BIN_PATH)/$(BIN_NAME).$(MACOS_TARGET_ARM64)
else # ($(UNAME_S),Linux)
OBJECTS = $(patsubst %, $(BUILD_PATH)/%.o, $(FILES))
endif # ($(UNAME_S),...)
### Debug settings
else ifeq ($(CONFIG), debug) # debug
ADDR_SANITIZER = -fsanitize=address
CPPFLAGS += -DDEBUG -g -Isrc/ \
	$(ADDR_SANITIZER) $(CPPSTD)
BIN_NAME = libbfcpp-debug.a
BUILD_PATH = build/debug
BIN_PATH = bin/debug
LIBRARIES += 
ifeq ($(UNAME_S),Darwin)
OBJECTS_MACOS_TARGET_X86_64 = $(patsubst %, $(BUILD_PATH)/%.$(MACOS_TARGET_X86_64), $(FILES))
OBJECTS_MACOS_TARGET_ARM64 = $(patsubst %, $(BUILD_PATH)/%.$(MACOS_TARGET_ARM64), $(FILES))
OBJECTS_MACOS_TARGETS = $(OBJECTS_MACOS_TARGET_X86_64) $(OBJECTS_MACOS_TARGET_ARM64)
BIN_MACOS_TARGETS = $(BIN_PATH)/$(BIN_NAME).$(MACOS_TARGET_X86_64) $(BIN_PATH)/$(BIN_NAME).$(MACOS_TARGET_ARM64)
else # ($(UNAME_S),Linux)
D_OBJECTS = $(patsubst %, $(BUILD_PATH)/%.o, $(FILES))
endif # ($(UNAME_S),...)
### Test settings
else ifeq ($(CONFIG), test) # test
ADDR_SANITIZER = -fsanitize=address
CPPFLAGS += -DDEBUG -DTESTING -g -Isrc/ \
	$(ADDR_SANITIZER) $(CPPSTD)
BIN_NAME = libbfcpp-test
BUILD_PATH = build/test
BIN_PATH = bin/test
MAIN_FILE = testbench/tests.cpp
LIBRARIES += external/libs/$(BF_LIB_RPATH_DEBUG_NET)
BIN_PREREQS := $(wildcard testbench/*.hpp)
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
OBJECTS = $(patsubst %, $(BUILD_PATH)/%.o, $(FILES))
endif
endif # ($(CONFIG),...)

.PRECIOUS: \
	$(BUILD_PATH)/%.$(MACOS_TARGET_X86_64) $(BUILD_PATH)/%.$(MACOS_TARGET_ARM64) \
	$(BUILD_PATH)/%.$(MACOS_TARGET_X86_64) $(BUILD_PATH)/%.$(MACOS_TARGET_ARM64) \
	$(BUILD_PATH)/%.$(MACOS_TARGET_X86_64) $(BUILD_PATH)/%.$(MACOS_TARGET_ARM64)

build: setup $(BIN_PATH)/$(BIN_NAME)

run:
	./$(BIN_PATH)/$(BIN_NAME)

help:
	@echo "Usage:"
	@echo "	make [target] variables"
	@echo ""
	@echo "Target(s):"
	@echo "	clean			cleans build and bin folder"
	@echo "	build 			builds release verions"
	@echo "	package			compresses build"
	@echo "	dependecies		builds all dependencies in the external directory"
	@echo "	clean-dependecies	builds all dependencies in the external directory"
	@echo "	clean-all		cleans local and dependency builds"
	@echo ""
	@echo "Variable(s):"
	@echo "	CONFIG		use this to change the build config. Accepts \"release\" (default), \"debug\", or \"test\""
	@echo "	IDENTITY	(macos only) \"Developer ID Application\" common name"
	@echo "	TEAMID 		(macos only) Organizational Unit"
	@echo "	EMAIL 		(macos only) Developer account email"
	@echo "	PW		(macos only) Developer account password"
	@echo ""
	@echo "Example(s):"
	@echo "	Build for release for macOS distribution"
	@echo "		make clean build codesign package notarize staple IDENTITY=\"\" TEAMID=\"\" EMAIL=\"\" PW=\"\""
	@echo "	Build for release for Linux distribution"
	@echo "		make clean build package"

SETUP_DIRS = $(BIN_PATH) $(BUILD_PATH)
setup: $(SETUP_DIRS)
$(SETUP_DIRS):
	mkdir -p $@

clean:
	rm -rfv $(BUILD_PATH)
	rm -rfv $(BIN_PATH)
	rm -rfv $(PACKAGE_NAME)

### Main build

ifeq ($(UNAME_S),Darwin)
$(BIN_PATH)/$(BIN_NAME): $(BIN_MACOS_TARGETS)
	lipo -create -output $@ $^

$(BIN_MACOS_TARGETS): $(MAIN_FILE) $(OBJECTS_MACOS_TARGETS) $(BIN_PREREQS)
	g++ -o $@ $< $(wildcard $(BUILD_PATH)/*$(suffix $@)) $(CPPFLAGS) $(CPPLINKS) $(LIBRARIES) -target $(subst --,.,$(subst .,,$(suffix $@)))

$(BUILD_PATH)/%.o: $(BUILD_PATH)/%.$(MACOS_TARGET_X86_64) $(BUILD_PATH)/%.$(MACOS_TARGET_ARM64)
	lipo -create -output $@ $^

$(OBJECTS_MACOS_TARGETS): $$(subst $(BUILD_PATH), src, $$(subst $$(suffix $$@),, $$@)).cpp  $$(subst $(BUILD_PATH), src, $$(subst $$(suffix $$@),, $$@)).hpp src/*.h
	g++ -c -o $@ $< $(CPPFLAGS) -target $(subst --,.,$(subst .,,$(suffix $@)))

else # ($(UNAME_S),Linux)

ifeq ($(CONFIG), test)
$(BIN_PATH)/$(BIN_NAME): $(MAIN_FILE) $(OBJECTS)
	g++ -o $@ $^ $(LIBRARIES) $(CPPFLAGS) $(CPPLINKS)
else # ($(CONFIG),...)
$(BIN_PATH)/$(BIN_NAME): $(MAIN_FILE) $(OBJECTS)
	$(CP) -afv src/*.hpp $(BIN_PATH)
	$(AR) rsc $@ $^ $(OBJECTS)
endif # ($(CONFIG), test)

$(BUILD_PATH)/%.o: src/%.cpp src/%.hpp src/*.h
	g++ -c $< -o $@ $(CPPFLAGS)

endif # ($(UNAME_S),...)

