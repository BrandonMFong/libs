/**
 * author: Brando
 * date: 10/6/22
 */

#ifndef STRING_HPP
#define STRING_HPP

#include "array.hpp"
#include "access.hpp"

#include <string>

namespace BF {
	
class Data;

class String : public Array<char, size_t> {
public:

	const char * className() const;

	virtual ~String();

	/**
	 * caller must release
	 */
	static String * createWithFormat(const char * format, ...);

	String();
	String(char * str);
	String(const std::string & str);
	String(const String & str);
	String(const char * format, ...);
	String(const char * format, va_list valist);

	// both integer param constructors throw 
	// std::invalid_argument if nullstr != 0
	String(long int nullstr);
	String(int nullstr);

	/**
	 * converts data to string
	 *
	 * this copies the memory
	 *
	 * if data is not null terminated, an extra byte is added to 
	 * buffer
	 */
	String(const Data & data);

	// Returns raw c string
	const char * cString() const;
	const char * c_str() const;

	bool starts_with(const char * sstr) const;

	// returns a copy of string
	//
	// caller must free()
	char * cStringCopy() const;

	/**
	 * Struns strcmp() on this and s
	 */
	int compareString(const String & s) const;
	virtual int compare(const Object & obj) const;

	/**
	 * Returns length of string
	 */
	size_t length() const;
	
	/**
	 * length() == 0
	 */	
	bool empty() const;

	/**
	 * Creates a deep copy of object and outputs to s
	 *
	 * s will have its own string to worry about
	 */
	int copy(String & s) const;

	/**
	 * loads file content into string
	 */
	int readFromFile(const char * file);

	/**
	 * Adds char c to the end of the string
	 *
	 * similar to std::string::push_back
	 */
	int addChar(char c);
	void push_back(char c);

	/**
	 * removes char at the end of the string
	 *
	 * similar to std::string::pop_back
	 */
	int remChar();
	void pop_back();

	/**
	 * adds a character at index
	 */
	int addCharAtIndex(char c, size_t index);

	/**
	 * removes a character at index
	 */
	int remCharAtIndex(size_t index);

	/**
	 * concatenates string to the end of our string
	 */
	void append(const String & suffix);
	void append(const char * format, ...);
	void append(const char * format, va_list valist);

	/**
	 * makes empty string
	 */
	int clear();

// Overloading operators
public:
	friend std::ostream& operator<<(std::ostream& out, const String & s) {
		return out << s.cString();
	}

	virtual bool operator==(const String & s) const;
	virtual bool operator==(const char * s) const;
	virtual bool operator<(const String & s) const;
	virtual bool operator>(const String & s) const;
	virtual bool operator!=(const String & s) const;
	virtual bool operator!=(const char * s) const;
	virtual String & operator=(const String & str);

// Conversions
public:

	/// similar to std::atoi
	static int toi(const String & s);
};

} // namespace BF

#endif // STRING_HPP

