/**
 * author: Brando
 * date: 3/9/23
 *
 * Class wrapper for our BFTime
 */

#ifndef TIME_HPP
#define TIME_HPP

#include <bflibc/typebftime.h>
#include <bflibc/typebfdatetime.h>
#include "access.hpp"
#include "object.hpp"

namespace BF {

class Time : public Object {
public:
	static Time * createCurrent();
	Time(BFTime t);
	Time();
	~Time();

	/**
	 * returns seconds since 1/1/70
	 */
	BFTime epoch() const;

	/// The actual calendar. not year - 1900
	int year() const;

	/// 1 - 12
	int month() const;

	/// 1 - 31
	int day() const;

	/// 0 - 23
	int hour() const;

	/// 0 - 59
	int minute() const;

	/// 0 - 59
	int second() const;

private:

	void set(BFTime t);

	// Base variables to hold time we are representing
	BFTime _time;
	BFDateTime _dateTime;

/// Overload operators
public:
	bool operator==(const Time & t);
	Time * operator+(const Time & t);
	Time * operator-(const Time & t);
	Time * operator*(const Time & t);
	Time * operator/(const Time & t);
	Time & operator=(const Time & t);
	Time & operator=(const BFTime & t);
};

}
#endif // TIME_HPP

