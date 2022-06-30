/**
 * author: Brando
 * date: 6/30/22
 */

#include <stdbool.h>
#include <stdint.h>

template <typename T> class Array {
public:
	Array() {
		this->_array = 0;
		this->_count = 0;
	}

	Array(T * array, uint64_t size) : Array() {
		this->_saveArray(array, size);	
	}

	virtual ~Array() {
		if (this->_array) delete this->_array;
		this->_count = 0;
	}

	/**
	 * Returns false if argument could not be found
	 */
	virtual bool contains(T object) {
		for (uint64_t i = 0; i < this->_count; i++) {
			if (this->_array[i] == object)
				return true;
		}
		return false;
	}

	/**
	 * Returns null if argument could not be found
	 */
	T objectAtIndex(uint64_t index) {
		if ((this->_array == 0) || (this->_count == 0)) {
			return (T) 0;
		} else {
			return this->_array[index];
		}
	}

	/**
	 * Returns the argument index
	 */
	uint64_t indexForObject(T value) {
		for (uint64_t i = 0; i < this->_count; i++) {
			if (this->_array[i] == value) return i;
		}
		return 0;
	}

	/// Returns _count
	int count() {
		return (int) this->_count;
	}

private:

	/**
	 * Copies values from array
	 */
	void _saveArray(T * array, uint64_t size) {
		this->_array = new T[size];
		this->_count = size;

		if (this->_array) {
			// Load into array
			for (uint64_t i = 0; i < size; i++) {
				this->_array[i] = array[i];
			}
		}
	}

	/// Holds copy of array
	T * _array;

	/// Holds size of _array
	uint64_t _count;
};
