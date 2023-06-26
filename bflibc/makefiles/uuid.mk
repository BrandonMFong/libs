# author: Brando
# date: 6/2/23
#

BF_LIB_C_UUID_FLAGS =

# Determine the OS
ifeq ($(OS),Windows_NT)
    ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
    else
        ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
        endif
        ifeq ($(PROCESSOR_ARCHITECTURE),x86)
        endif
    endif
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
		BF_LIB_C_UUID_FLAGS += -luuid
    endif
    ifeq ($(UNAME_S),Darwin)
    endif
endif

