/**
 * author: Brando
 * date: 6/30/22
 */

#ifndef CPPLIB_TESTS_HPP
#define CPPLIB_TESTS_HPP

#include "allocator.hpp"

extern "C" {
#include <bftest/bftest.h>
#include <bflibc/free.h>
}

template<> struct BF::Allocator<char *> {
	char * create() const {
		return 0;
	}

	void release(char * obj) const {
		BFFree(obj);
	}
};

template<> struct BF::Allocator<int *> {
	int * create() const {
		return 0;
	}

	void release(int * obj) const {
		BFFree(obj);
	}
};

#endif // CPPLIB_TESTS_HPP

