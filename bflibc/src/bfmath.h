/**
 * author: brando
 * date: 11/17/24
 */

#ifndef BF_MATH_H
#define BF_MATH_H

#include <stdbool.h>
#include "nargs.h"

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

double __BFMathMax__(int numargs,...);
#define BFMathMax(...) \
	__BFMathMax__( BF_NARGS(__VA_ARGS__) , __VA_ARGS__)

#endif // BF_MATH_H

