/**
 * author: Brando
 * date: 6/30/22
 */

#include <stdbool.h>

template <typename T> class Array {
public:
	Array() {

	}

	virtual ~Array() {

	}

	/**
	 * Returns false if argument could not be found
	 */
	bool contains(T argument) {
		return false;
	}

	/**
	 * Returns null if argument could not be found
	 */
	T objectAtIndex(char index) {
		return new T;
	}

	/**
	 * Returns the argument index
	 */
	int indexForArgument(const char * argument) {
		return 0;
	}

	/// Returns _count
	int count() {
		return (int) this->_count;
	}

private:

	/// Holds all the arguments in one array
	T * _array;

	/// Holds size of _arguments.  Only need 8 bits
	char _count;
};
