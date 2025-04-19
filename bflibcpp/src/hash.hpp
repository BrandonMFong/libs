/**
 * author: brando
 * date: 4/17/25
 */

#ifndef HASH_HPP
#define HASH_HPP

#include "string.hpp"

extern "C" {
#include <bflibc/hash.h>
}

namespace BF {
template<typename T> struct Hash;

template<> struct Hash<String> {
	unsigned long operator()(const String & key) {
		return BFHashDjb2((unsigned char *) key.c_str());
	}
};

template<> struct Hash<char *> {
	unsigned long operator()(const char * key) {
		return BFHashDjb2((unsigned char *) key);
	}
};

}

#endif // HASH_HPP

