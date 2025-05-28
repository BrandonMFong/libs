/**
 * author: brando
 * date: 5/16/25
 */

#ifndef COMPARE_HPP
#define COMPARE_HPP

#include <string.h>

namespace BF {

template <typename T>
struct Compare {
	int operator()(const T & a, const T & b) {
		return a - b;
	}
};

template <>
struct Compare<char *> {
	int operator()(const char * const & a, const char * const & b) {
		return strcmp(a, b);
	}
};

}

#endif // COMPARE_HPP

