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

	/**
	 * returns address to our memory
	 */
	void * buffer();

	/**
	 * size of the memory returned by buffer()
	 */
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

	/**
	 * adjusts the size of our buffer's allocated memory
	 *
	 * s : new size
	 *
	 * if s is greater than our current size, this will act as a padding
	 * if s is less than our current size, this will trim the memory
	 *
	 * calling this requires caution if not used properly
	 */
	void adjustSize(size_t s);

private:
};

}

#endif // BF_DATA_HPP

