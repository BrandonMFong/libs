/**
 * author: brando
 * date: 6/26/24
 */

#ifndef BF_DATA_HPP
#define BF_DATA_HPP

#include <stddef.h>
#include "array.hpp"

namespace BF {

class Data : protected Array<unsigned char, size_t> {
public:
	/**
	 * empty buffer with size 0
	 */
	Data();

	/**
	 * size: size of the buffer
	 *
	 * this will create a buffer with size. Data in buffer is random
	 */
	Data(const size_t s);
	Data(const size_t s, const unsigned char * data);
	~Data();

	void * buffer();
	size_t size();

private:
};

}

#endif // BF_DATA_HPP

