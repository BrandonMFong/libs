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
	 *
	 * the data gets copied
	 */
	Data(const size_t s, const unsigned char * data = NULL);

	~Data();

	void * buffer();
	size_t size();

	/**
	 * clears buffer with all zeroes
	 */
	void clear();

	/**
	 * allocates buffer with size
	 *
	 * if buffer was previously set, this will erase all data
	 *
	 * please use with caution
	 *
	 * data gets copied
	 */
	int alloc(const size_t s, const unsigned char * data = NULL);

private:
};

}

#endif // BF_DATA_HPP

