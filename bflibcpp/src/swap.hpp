/**
 * author: brando
 * date: 4/22/25
 */

#ifndef SWAP_HPP
#define SWAP_HPP

namespace BF {

template<typename T>
void swap(T & a, T & b) {
	T tmp = a;
	a = b;
	b = tmp;
}

}

#endif // SWAP_HPP

