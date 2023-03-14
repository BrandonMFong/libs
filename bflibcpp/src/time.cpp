/**
 * author: Brando
 * date: 3/9/23
 */

#include "time.hpp"

extern "C" {
#include <bflibc/bftime.h>
#include <stdio.h>
#include <bflibc/log.h>
}

using namespace BF;

Time * Time::createCurrent() {
	return new Time(BFTimeGetCurrentTime());
}

Time::Time(BFTime t) {
	this->_time = t;
	this->_dateTime = {0};

	if (BFTimeGetDateTimeFromTime(this->_time, &this->_dateTime)) {
		BFDLog("There was an issue calculating the datetime for %ld", this->_time);
	}
}

Time::~Time() {

}

BFTime Time::epoch() const {
	return this->_time;
}

int Time::year() const {
	return this->_dateTime.year;
}

int Time::month() const {
	return this->_dateTime.month;
}

const char * Time::monthString() const {
	return BFTimeGetMonthStringFromDateTime(&this->_dateTime);
}

int Time::day() const {
	return this->_dateTime.day;
}

int Time::hour() const {
	return this->_dateTime.hour;
}

int Time::minute() const {
	return this->_dateTime.minute;
}

int Time::second() const {
	return this->_dateTime.second;
}

bool Time::operator==(const Time & t) {
	return this->epoch() == t.epoch();
}

Time * Time::operator+(const Time & t) {
	return new Time(this->epoch() + t.epoch());
}

Time * Time::operator-(const Time & t) {
	return new Time(this->epoch() - t.epoch());
}

Time * Time::operator*(const Time & t) {
	return new Time(this->epoch() * t.epoch());
}

Time * Time::operator/(const Time & t) {
	return new Time(this->epoch() / t.epoch());
}

