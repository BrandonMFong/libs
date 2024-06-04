/**
 * @file
 * author: Brando
 * date: 3/8/23
 */

#include "bftime.h"
#include <unistd.h>

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

int BFTimeGetStructTMLocal(BFTime time, struct tm * tm) {
	if (tm == 0) return 1;
	time_t t = (time_t) time;
	if (localtime_r(&t, tm) == NULL) return 2;
	return 0;
}

int BFTimeGetStructTMUTC(BFTime time, struct tm * tm) {
	if (tm == 0) return 1;
	time_t t = (time_t) time;
	if (gmtime_r(&t, tm) == NULL) return 2;
	return 0;
}

int BFTimeGetCurrentDateTime(BFDateTime * dt) {
	return BFTimeGetDateTimeLocal(BFTimeGetCurrentTime(), dt);
}

void _BFTimeCastTMToBFDateTime(struct tm * tm, BFDateTime * dt) {
	dt->year = tm->tm_year + 1900;
	dt->month = tm->tm_mon + 1;
	dt->day = tm->tm_mday;
	dt->hour = tm->tm_hour;
	dt->minute = tm->tm_min;
	dt->second = tm->tm_sec;
}

int BFTimeGetDateTimeUTC(BFTime tin, BFDateTime * dt) {
	int result = 0;
	struct tm tm = {0};
	time_t t = (time_t) tin;

	if (dt == NULL) {
		result = -1;
	} else if (localtime_r(&t, &tm) == NULL) {
		result = -2;
	}

	if (result == 0) {
		_BFTimeCastTMToBFDateTime(&tm, dt);
	}

	return result;
}

int BFTimeGetDateTimeLocal(BFTime tin, BFDateTime * dt) {
	int result = 0;
	struct tm tm = {0};
	time_t t = (time_t) tin;

	if (dt == NULL) {
		result = -1;
	} else if (localtime_r(&t, &tm) == NULL) {
		result = -2;
	}

	if (result == 0) {
		_BFTimeCastTMToBFDateTime(&tm, dt);
	}

	return result;
}

void BFTimeSleep(BFTime t) {
	unsigned int ut = (t - (unsigned int) t) * 1000 * 1000;
	sleep((time_t) t);
	usleep(ut);
}

