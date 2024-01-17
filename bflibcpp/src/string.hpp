/**
 * author: Brando
 * date: 10/6/22
 */

#ifndef STRING_HPP
#define STRING_HPP

#include "array.hpp"
#include "access.hpp"

namespace BF {

class String : protected Array<char, size_t> {
PUBLIC:
	virtual ~String();

	String();
	String(const char * str);
	String(char * str);
	String(const String & str);

	// both integer param constructors throw 
	// std::invalid_argument if nullstr != 0
	String(long int nullstr);
	String(int nullstr);

	// Returns raw c string
	const char * cString() const;

	/**
	 * Struns strcmp() on this and s
	 */
	int compareString(const String & s) const;

	/**
	 * Returns length of string
	 */
	size_t length() const;

	/**
	 * Creates a deep copy of object and outputs to s
	 *
	 * s will have its own string to worry about
	 */
	int copy(String & s) const;

// Overloading operators
PUBLIC:
	friend std::ostream& operator<<(std::ostream& out, const String & s) {
		return out << s.cString();
	}

	operator const char * () const; // casting overloader
	bool operator==(const String & s);
	bool operator<(const String & s);
	bool operator>(const String & s);
	bool operator!=(const String & s);
	String & operator=(const String & str);
};

} // namespace BF

#endif // STRING_HPP

