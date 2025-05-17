/**
 * author: brando
 * date: 5/16/25
 */

#ifndef COMPARE_HPP
#define COMPARE_HPP

namespace BF {

template <typename T>
struct Compare {
	int operator()(const T & a, const T & b) {
		return a - b;
	}
};

}

#endif // COMPARE_HPP

