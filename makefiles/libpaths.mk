# author: Brando
# date: 3/7/23
# defines variables for others to include in their makefiles.  That way we wont affect other projects much

BF_LIB_RPATH_DEBUG = bin/debug
BF_LIB_RPATH_RELEASE = bin/release

BF_LIB_RPATH_DEBUG_C = $(BF_LIB_RPATH_DEBUG)/bflibc/libbfc-debug.a
BF_LIB_RPATH_RELEASE_C = $(BF_LIB_RPATH_RELEASE)/bflibc/libbfc.a

BF_LIB_RPATH_DEBUG_CPP = $(BF_LIB_RPATH_DEBUG)/bflibcpp/libbfcpp-debug.a
BF_LIB_RPATH_RELEASE_CPP = $(BF_LIB_RPATH_RELEASE)/bflibcpp/libbfcpp.a

BF_LIB_RPATH_DEBUG_NET = $(BF_LIB_RPATH_DEBUG)/bfnet/libbfnet-debug.a
BF_LIB_RPATH_RELEASE_NET = $(BF_LIB_RPATH_RELEASE)/bfnet/libbfnet.a

