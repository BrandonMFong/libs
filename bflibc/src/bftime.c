/**
 * author: Brando
 * date: 3/8/23
 */

#include "bftime.h"

const char * kBFTimeMonthStringJanuaryFull = "January";
const char * kBFTimeMonthStringFebruaryFull = "February";
const char * kBFTimeMonthStringMarchFull = "March";
const char * kBFTimeMonthStringAprilFull = "April";
const char * kBFTimeMonthStringMayFull = "May";
const char * kBFTimeMonthStringJuneFull = "June";
const char * kBFTimeMonthStringJulyFull = "July";
const char * kBFTimeMonthStringAugustFull = "August";
const char * kBFTimeMonthStringSeptemberFull = "September";
const char * kBFTimeMonthStringOctoberFull = "October";
const char * kBFTimeMonthStringNovemberFull = "November";
const char * kBFTimeMonthStringDecemberFull = "December";


BFTime BFTimeGetCurrentTime() {
	return (BFTime) time(NULL);
}

int BFTimeGetCurrentDateTime(BFDateTime * dt) {
	return BFTimeGetDateTimeFromTime(BFTimeGetCurrentTime(), dt);
}

int BFTimeGetDateTimeFromTime(BFTime t, BFDateTime * dt) {
	int result = 0;
	struct tm tm = {0};

	if (dt == NULL) {
		result = -1;
	} else if (localtime_r(&t, &tm) == NULL) {
		result = -2;
	}

	if (result == 0) {
		dt->year = tm.tm_year + 1900;
		dt->month = tm.tm_mon + 1;
		dt->day = tm.tm_mday;
		dt->hour = tm.tm_hour;
		dt->minute = tm.tm_min;
		dt->second = tm.tm_sec;
	}

	return result;
}

const char * BFTimeGetMonthStringFromTime(const BFTime t) {
	int result = 0;
	BFDateTime dt = {0};

	if (BFTimeGetDateTimeFromTime(t, &dt)) {
		return 2;
	}
	return 0;
}

const char * BFTimeGetMonthStringFromDateTime(const BFDateTime * dt) {
	return 0;
}

