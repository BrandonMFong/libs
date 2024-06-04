/**
 * @file
 *
 * author: Brando
 * date: 3/8/23
 *
 * My attempt at making working with time easier
 */

#ifndef BFTIME_H
#define BFTIME_H

#include "typebftime.h"
#include "typebfdatetime.h"

#define BFTimeMS(x) ((BFTime) (x / 1000))
#define BFTimeUS(x) ((BFTime) (x / (1000*1000)))
#define BFTimeNS(x) ((BFTime) (x / (1000*1000*1000)))

#define BFTimeGetS(x) ((unsigned int) x)
#define BFTimeGetNS(x) ((x - (unsigned int) x) * (1000*1000*1000))

extern const char * kBFTimeMonthStringJanuaryFull;
extern const char * kBFTimeMonthStringFebruaryFull;
extern const char * kBFTimeMonthStringMarchFull;
extern const char * kBFTimeMonthStringAprilFull;
extern const char * kBFTimeMonthStringMayFull;
extern const char * kBFTimeMonthStringJuneFull;
extern const char * kBFTimeMonthStringJulyFull;
extern const char * kBFTimeMonthStringAugustFull;
extern const char * kBFTimeMonthStringSeptemberFull;
extern const char * kBFTimeMonthStringOctoberFull;
extern const char * kBFTimeMonthStringNovemberFull;
extern const char * kBFTimeMonthStringDecemberFull;

/**
 * Returns current time since 1970 Jan 1st
 */
BFTime BFTimeGetCurrentTime();

/**
 * Converts time to struct tm expressed in user's local time
 */
int BFTimeGetStructTMLocal(BFTime time, struct tm * tm);

/**
 * Converts time to struct tm expressed in UTC
 */
int BFTimeGetStructTMUTC(BFTime time, struct tm * tm);

/**
 * Returns date for user's current timezone
 */
int BFTimeGetCurrentDateTime(BFDateTime * dt);

/**
 * Returns date for user's current timezone
 */
int BFTimeGetDateTimeLocal(BFTime t, BFDateTime * dt);

/**
 * sleeps for BFTime
 */
void BFTimeSleep(BFTime);

#endif // BFTIME_H

