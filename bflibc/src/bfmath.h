/**
 * author: brando
 * date: 11/17/24
 */

#ifndef BF_MATH_H
#define BF_MATH_H

#include <stdbool.h>

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

#endif // BF_MATH_H

