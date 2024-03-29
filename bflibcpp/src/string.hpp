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

	/**
	 * caller must release
	 */
	static String * createWithFormat(const char * format, ...);

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
	 *
	 * similar to std::string::pop_back
	 */
	int remChar();

	/**
	 * adds a character at index
	 */
	int addCharAtIndex(char c, size_t index);

	/**
	 * removes a character at index
	 */
	int remCharAtIndex(size_t index);

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
	const char operator[](size_t index);

// Conversions
public:

	/// similar to std::atoi
	static int toi(const String & s);
};

} // namespace BF

#endif // STRING_HPP

