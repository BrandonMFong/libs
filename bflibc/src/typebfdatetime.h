/**
 * author: Brando
 * date: 3/8/23
 */

#ifndef TYPEDATETIME_H
#define TYPEDATETIME_H

typedef struct {
	int year; // actual year, not the year - 1900
	int month; // 1 - 12
	int day; // 1 - 31
	int hour; // 0 - 23
	int minute; // 0 - 59
	int second; // 0 - 59
} BFDateTime;

#endif // TYPEDATETIME_H

