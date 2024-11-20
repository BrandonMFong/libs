# author: Brando
# date: 11/12/24

LIB_NAME = bflibc
LIB_FILE_NAME_RELEASE = libbfc.a
LIB_FILE_NAME_DEBUG = libbfc-debug.a

# Determine the OS
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	BF_LIB_C_FLAGS += -luuid -lm
endif
ifeq ($(UNAME_S),Darwin)
endif

