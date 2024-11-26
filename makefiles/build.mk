# 
# author: brando
# date: 11/12/24
#

.SECONDEXPANSION:

# BUILD_TYPE = archive || executable
ifeq ($(LIBS_MAKEFILES_PATH),)
$(error ERROR: "please define `LIBS_MAKEFILES_PATH` in your makefile. this should be the absolute path to build.mk")
endif

include $(LIBS_MAKEFILES_PATH)/libpaths.mk 
include $(LIBS_MAKEFILES_PATH)/platforms.mk 

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

UNAME_S := $(shell uname -s)

CONFIG = release
BUILD_PATH = build/$(CONFIG)
BIN_PATH = bin/$(CONFIG)/$(LIB_NAME)

# BUILD_TYPE = archive || executable
ifeq ($(BUILD_TYPE),)
$(error ERROR: "please define `BUILD_TYPE` in your makefile")
endif

# used to make universal binaries
MACOS_TARGET_X86_64 = x86_64-apple-macos10--12
MACOS_TARGET_ARM64 = arm64-apple-macos11

### Release settings
ifeq ($(CONFIG),release) # release
OBJECTS = $(patsubst %, $(BUILD_PATH)/%.o, $(FILES))
ifeq ($(UNAME_S),Darwin)
OBJECTS_MACOS_TARGET_X86_64 = \
	$(patsubst %, $(BUILD_PATH)/%.$(MACOS_TARGET_X86_64), $(FILES))
OBJECTS_MACOS_TARGET_ARM64 = \
	$(patsubst %, $(BUILD_PATH)/%.$(MACOS_TARGET_ARM64), $(FILES))
OBJECTS_MACOS_TARGETS = \
	$(OBJECTS_MACOS_TARGET_X86_64) $(OBJECTS_MACOS_TARGET_ARM64)
BIN_MACOS_TARGETS = \
	$(BIN_PATH)/$(BIN_NAME).$(MACOS_TARGET_X86_64) \
	$(BIN_PATH)/$(BIN_NAME).$(MACOS_TARGET_ARM64)
endif # ($(UNAME_S),...)

### Debug settings
else ifeq ($(CONFIG),debug) # debug
OBJECTS = $(patsubst %, $(BUILD_PATH)/%.o, $(FILES))
ifeq ($(UNAME_S),Darwin)
OBJECTS_MACOS_TARGET_X86_64 = \
	$(patsubst %, $(BUILD_PATH)/%.$(MACOS_TARGET_X86_64), $(FILES))
OBJECTS_MACOS_TARGET_ARM64 = \
	$(patsubst %, $(BUILD_PATH)/%.$(MACOS_TARGET_ARM64), $(FILES))
OBJECTS_MACOS_TARGETS = \
	$(OBJECTS_MACOS_TARGET_X86_64) $(OBJECTS_MACOS_TARGET_ARM64)
BIN_MACOS_TARGETS = \
	$(BIN_PATH)/$(BIN_NAME).$(MACOS_TARGET_X86_64) \
	$(BIN_PATH)/$(BIN_NAME).$(MACOS_TARGET_ARM64)
endif # ($(UNAME_S),...)

### Test settings
else ifeq ($(CONFIG),test) # test
BIN_PREREQS := $(wildcard testbench/*.$(HEADER_EXT))
OBJECTS = $(patsubst %, $(BUILD_PATH)/%.o, $(FILES))
ifeq ($(UNAME_S),Darwin)
MAIN_OBJECT_MACOS_TARGET_X86_64 = \
	$(BUILD_PATH)/tests.$(MACOS_TARGET_X86_64)
MAIN_OBJECT_MACOS_TARGET_ARM64 = \
	$(BUILD_PATH)/tests.$(MACOS_TARGET_ARM64)
MAIN_OBJECT_MACOS_TARGETS = \
	$(MAIN_OBJECT_MACOS_TARGET_X86_64) $(MAIN_OBJECT_MACOS_TARGET_ARM64)
OBJECTS_MACOS_TARGET_X86_64 = \
	$(patsubst %, $(BUILD_PATH)/%.$(MACOS_TARGET_X86_64), $(FILES))
OBJECTS_MACOS_TARGET_ARM64 = \
	$(patsubst %, $(BUILD_PATH)/%.$(MACOS_TARGET_ARM64), $(FILES))
OBJECTS_MACOS_TARGETS = \
	$(OBJECTS_MACOS_TARGET_X86_64) $(OBJECTS_MACOS_TARGET_ARM64)
BIN_MACOS_TARGETS = \
	$(BIN_PATH)/$(BIN_NAME).$(MACOS_TARGET_X86_64) \
	$(BIN_PATH)/$(BIN_NAME).$(MACOS_TARGET_ARM64)
else
MAIN_OBJECT = $(BUILD_PATH)/tests.o
endif
endif # ($(CONFIG),...)

.PRECIOUS: \
	$(BUILD_PATH)/%.$(MACOS_TARGET_X86_64) $(BUILD_PATH)/%.$(MACOS_TARGET_ARM64) \
	$(BUILD_PATH)/%.$(MACOS_TARGET_X86_64) $(BUILD_PATH)/%.$(MACOS_TARGET_ARM64) \
	$(BUILD_PATH)/%.$(MACOS_TARGET_X86_64) $(BUILD_PATH)/%.$(MACOS_TARGET_ARM64)

build: setup $(BIN_PATH)/$(BIN_NAME)

ifeq ($(CONFIG),test)
run:
	./$(BIN_PATH)/$(BIN_NAME)
run-debugger:
	$(DEBUGGER) ./$(BIN_PATH)/$(BIN_NAME)
endif

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
$(BIN_MACOS_TARGETS): \
	$(MAIN_FILE) $(OBJECTS_MACOS_TARGETS) $(BIN_PREREQS)
	#$(COMPILER) -o $@ $< $(wildcard $(BUILD_PATH)/*$(suffix $@))
	$(COMPILER) -o $@ $< $(call rwildcard,$(BUILD_PATH),*$(suffix $@)) \
	$(FLAGS) $(LINKS) $(LIBRARIES) \
	-target $(subst --,.,$(subst .,,$(suffix $@)))
else # ($(CONFIG),release||debug)
ifeq ($(BUILD_TYPE),archive)
$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	cp -afv src/*.$(HEADER_EXT) $(BIN_PATH)
	ar rsc $@ $^
else ifeq ($(BUILD_TYPE),executable) # ($(BUILD_TYPE),executable)
$(BIN_PATH)/$(BIN_NAME): $(BIN_MACOS_TARGETS)
	lipo -create -output $@ $^
$(BIN_MACOS_TARGETS): \
	$(MAIN_FILE) $(OBJECTS_MACOS_TARGETS) $(BIN_PREREQS)
	#$(COMPILER) -o $@ $< $(wildcard $(BUILD_PATH)/*$(suffix $@))
	$(COMPILER) -o $@ $< $(call rwildcard,$(BUILD_PATH),*$(suffix $@)) \
	$(FLAGS) $(LINKS) $(LIBRARIES) \
	-target $(subst --,.,$(subst .,,$(suffix $@)))
endif # ($(BUILD_TYPE),...)

$(BUILD_PATH)/%.o: \
	$(BUILD_PATH)/%.$(MACOS_TARGET_X86_64) \
	$(BUILD_PATH)/%.$(MACOS_TARGET_ARM64)
	lipo -create -output $@ $^
endif # ($(CONFIG), test)

$(OBJECTS_MACOS_TARGETS): \
	$$(subst $(BUILD_PATH), src, $$(subst $$(suffix $$@),, $$@)).$(SOURCE_EXT) \
	$$(subst $(BUILD_PATH), src, $$(subst $$(suffix $$@),, $$@)).$(HEADER_EXT)
	@mkdir -p $(dir $@);
	$(COMPILER) -c -o $@ $< $(FLAGS) -target $(subst --,.,$(subst .,,$(suffix $@)))

else # ($(UNAME_S),Linux)

ifeq ($(CONFIG),test)
$(BIN_PATH)/$(BIN_NAME): \
	$(MAIN_FILE) $(OBJECTS) $(BIN_PREREQS)
	$(COMPILER) -o $@ $< $(OBJECTS) $(LIBRARIES) $(FLAGS) $(LINKS)
else # ($(CONFIG),release||debug)
ifeq ($(BUILD_TYPE),archive)
$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	cp -afv src/*.$(HEADER_EXT) $(BIN_PATH)
	ar rsc $@ $^
else ifeq ($(BUILD_TYPE),executable) # ($(BUILD_TYPE),executable)
$(BIN_PATH)/$(BIN_NAME): \
	$(MAIN_FILE) $(OBJECTS) $(BIN_PREREQS)
	$(COMPILER) -o $@ $< $(OBJECTS) $(LIBRARIES) $(FLAGS) $(LINKS)
endif # ($(BUILD_TYPE),...)
endif # ($(CONFIG), ...)

$(BUILD_PATH)/%.o: src/%.$(SOURCE_EXT) src/%.$(HEADER_EXT)
	@mkdir -p $(dir $@);
	$(COMPILER) -c $< -o $@ $(FLAGS)

endif # ($(UNAME_S),...)

