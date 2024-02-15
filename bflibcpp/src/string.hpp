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
	String();
	String(const char * str);
	String(char * str);
	String(const String & str);
	virtual ~String();

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

	/**
	 * Adds char c to the end of the string
	 *
	 * similar to std::string::push_back
	 */
	int addChar(char c);

	/**
	 * removes char at the end of the string
	 */
	int remChar();

	/**
	 * makes empty string
	 */
	int clear();

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

