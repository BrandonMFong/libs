/**
 * author: Brando
 * date: 3/8/23
 *
 * My attempt at making working with time easier
 */

#ifndef BFTIME_H
#define BFTIME_H

#include "typebftime.h"
#include "typebfdatetime.h"

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
 * Returns date for user's current timezone
 */
int BFTimeGetCurrentDateTime(BFDateTime * dt);

/**
 * Returns date for user's current timezone
 */
int BFTimeGetDateTimeFromTime(BFTime t, BFDateTime * dt);

/**
 * Returns month string representation for time
 */
const char * BFTimeGetMonthStringFromTime(const BFTime t);

/**
 * Returns month string representation for date time
 */
const char * BFTimeGetMonthStringFromDateTime(const BFDateTime * dt);

#endif // BFTIME_H

