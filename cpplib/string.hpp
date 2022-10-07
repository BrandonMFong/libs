/**
 * author: Brando
 * date: 10/6/22
 */

#ifndef STRING_HPP
#define STRING_HPP

#include "array.hpp"
#include "accessorspecifiers.hpp"

class String : public Array<char, size_t> {
PUBLIC:
	String();
	String(const char * str);
	String(char * str);
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

// Overloading operators
PUBLIC:
	friend std::ostream& operator<<(std::ostream& out, const String & s);
	operator const char * () const; // casting overloader
	bool operator==(const String & s);
	bool operator!=(const String & s);
};

#endif // STRING_HPP

