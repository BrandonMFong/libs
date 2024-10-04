/**
 * author: brando
 * date: 6/26/24
 */

#ifndef BF_DATA_HPP
#define BF_DATA_HPP

#include <stddef.h>
#include "array.hpp"

namespace BF {

class String;

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

	/**
	 * copies in data
	 */
	Data(const Data & in);

	/**
	 * creates data with string
	 *
	 * this copies the string data
	 */
	Data(const String & str);

	virtual ~Data();

	/**
	 * if size is 0, buffer will be NULL
	 *
	 * returns address to our memory
	 *
	 * warning - if you are intending to use this as a string, unless you know for sure this buffer
	 * contains a null termination, convert this to BF::String using the string's BF::Data constructor
	 */
	void * buffer() const;

	/**
	 * returns hex string
	 */
	String hex() const;

	/**
	 * size of the memory returned by buffer()
	 */
	size_t size() const;

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
	void resize(size_t s);

	/**
	 * does memcmp
	 */
	int compare(const Data & in) const;

private:
public:
	bool operator==(const Data & s);
};

}

#endif // BF_DATA_HPP

