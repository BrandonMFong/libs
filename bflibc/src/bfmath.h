/**
 * author: brando
 * date: 11/17/24
 */

#ifndef BF_MATH_H
#define BF_MATH_H

#include <stdbool.h>
#include <string.h>
#include "nargs.h"

/**
 * accuracy for BFMathSqrt
 */
#define kBFMathSqrtFactor 0.000001

/**
 * binary search approach
 */
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

#define _kBFMathGetTypeIDInt 1
#define _kBFMathGetTypeIDShort 2
#define _kBFMathGetTypeIDLong 3
#define _kBFMathGetTypeIDChar 4
#define _kBFMathGetTypeIDFloat 5
#define _kBFMathGetTypeIDDouble 6
#define _kBFMathGetTypeIDDefault _kBFMathGetTypeIDInt

#define _kBFMathGetTypeID(x) \
	_Generic(\
		(x),\
		int: _kBFMathGetTypeIDInt,\
		short: _kBFMathGetTypeIDShort,\
		long: _kBFMathGetTypeIDLong,\
		char: _kBFMathGetTypeIDChar,\
		float: _kBFMathGetTypeIDFloat,\
		double: _kBFMathGetTypeIDDouble,\
		default: _kBFMathGetTypeIDDefault \
	)

/**
 * Finds max out of the number list provided
 */
#define BFMathMax(num,...) \
	_BFMathMinMax ( 1, _kBFMathGetTypeID(num), BF_NARGS(__VA_ARGS__) + 1, num, __VA_ARGS__ )

/**
 * Finds min out of the number list provided
 */
#define BFMathMin(num,...) \
	_BFMathMinMax( -1, _kBFMathGetTypeID(num), BF_NARGS(__VA_ARGS__) + 1, num, __VA_ARGS__ )

//double BFMathAbs(double);
#define BFMathAbs(num) \
	(((num) < 0) ? (-1 * (num)) : (num))

#endif // BF_MATH_H

