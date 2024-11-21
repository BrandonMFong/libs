/**
 * author: brando
 * date: 11/17/24
 */

#ifndef BF_MATH_H
#define BF_MATH_H

#include <stdbool.h>
#include <string.h>
#include "nargs.h"
#include "gettype.h"

#define kBFMathSqrtFactor 0.0001
float BFMathSqrt(float);

/**
 * Gets the `index+1`th prime number
 *
 * -1 if prime number couldn't be reached
 */
int BFMathPrimeGetNumberAtIndex(int index);

/**
 * tests if number is prime
 */
bool BFMathPrimeIsPrime(int num);

/**
 * op: 1 if finding max, -1 if min
 */
double _BFMathMinMax(int op, int datatype, int numargs,...);

/**
 * Finds max out of the number list provided
 */
#define BFMathMax(num,...) \
	_BFMathMinMax ( 1, BFGetType(num), BF_NARGS(__VA_ARGS__) + 1, num, __VA_ARGS__ )

/**
 * Finds min out of the number list provided
 */
#define BFMathMin(num,...) \
	_BFMathMinMax( -1, BFGetType(num), BF_NARGS(__VA_ARGS__) + 1, num, __VA_ARGS__ )

#endif // BF_MATH_H

