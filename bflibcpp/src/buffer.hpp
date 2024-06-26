/**
 * author: brando
 * date: 6/26/24
 */

#ifndef BF_BUFFER_HPP
#define BF_BUFFER_HPP

#include <stddef.h>

namespace BF {

class Buffer {
public:
	/**
	 * empty buffer with size 0
	 */
	Buffer();

	/**
	 * size: size of the buffer
	 *
	 * this will create a buffer with size. Data in buffer is random
	 */
	Buffer(size_t s);
	Buffer(size_t s, void * data);
	~Buffer();
};

}

#endif // BF_BUFFER_HPP

