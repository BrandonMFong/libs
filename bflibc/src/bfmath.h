/**
 * author: brando
 * date: 11/17/24
 */

#ifndef BF_MATH_H
#define BF_MATH_H

#include <stdbool.h>
#include <string.h>
#include "nargs.h"

#define kBFMathSqrtFactor 0.000001
float BFMathSqrt(float);

/**
 * Gets the `index+1`th prime number
 *
 * -1 if prime number couldn't be reached
 */
int BFMathPrimeGetNumberAtIndex(int index);

#define kBFMathPrimeCachedPrimesCount (2 << 20)

/**
 * tests if number is prime
 */
bool BFMathPrimeIsPrime(int num);

/**
 * op: 1 if finding max, -1 if min
 */
double _BFMathMinMax(int op, int datatype, int numargs,...);

#define _kBFMathGetTypeInt 1
#define _kBFMathGetTypeShort 2
#define _kBFMathGetTypeLong 3
#define _kBFMathGetTypeChar 4
#define _kBFMathGetTypeFloat 5
#define _kBFMathGetTypeDouble 6
#define _kBFMathGetTypeDefault _kBFMathGetTypeInt

#define _kBFMathGetType(x) \
	_Generic(\
		(x),\
		int: _kBFMathGetTypeInt,\
		short: _kBFMathGetTypeShort,\
		long: _kBFMathGetTypeLong,\
		char: _kBFMathGetTypeChar,\
		float: _kBFMathGetTypeFloat,\
		double: _kBFMathGetTypeDouble,\
		default: _kBFMathGetTypeDefault \
	)

/**
 * Finds max out of the number list provided
 */
#define BFMathMax(num,...) \
	_BFMathMinMax ( 1, _kBFMathGetType(num), BF_NARGS(__VA_ARGS__) + 1, num, __VA_ARGS__ )

/**
 * Finds min out of the number list provided
 */
#define BFMathMin(num,...) \
	_BFMathMinMax( -1, _kBFMathGetType(num), BF_NARGS(__VA_ARGS__) + 1, num, __VA_ARGS__ )

#endif // BF_MATH_H

