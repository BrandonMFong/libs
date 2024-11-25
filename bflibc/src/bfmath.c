/**
 * author: brando
 * date: 11/17/24
 */

#include "bfmath.h"
#include <limits.h>
#include <stdarg.h>

/*
double BFMathAbsDouble(double num) {
	if (num < 0) return -1 * num;
	return num;
}

int BFMathAbs(int num) {
	return BFMathAbsDouble(num);
}
*/

float BFMathSqrt(float n) {
    if (n < 2) {
        return n;
    }
    float low = 1, high = n;
    while (low <= high) {
        float mid = (low + high) / 2;
        if (mid * mid == n) {
            return mid;
        } else if (mid * mid < n) {
            low = mid + kBFMathSqrtFactor;
        } else {
            high = mid - kBFMathSqrtFactor;
        }
    }
    return high;
}

// primesTable[n] = prime number at n
int primesTable[kBFMathPrimeCachedPrimesCount] = {0};
int primesTableSize = 0;

int BFMathPrimeGetNumberAtIndex(int nth) {
	if (nth < primesTableSize) {
		return primesTable[nth];
	}

	for (int i = 0; i < INT_MAX; i++) {
		if (BFMathPrimeIsPrime(i)) {

			// save the prime number if we have space
			if (nth < kBFMathPrimeCachedPrimesCount) {
				primesTable[primesTableSize++] = i;
			}

			if (primesTableSize - 1 == nth) {
				return i;
			}
		}
	}
	return -1;
}

bool BFMathPrimeIsPrime(int num) {
	int n = num, i;

	// 0 and 1 are not prime numbers
	// change flag to 1 for non-prime number
	if (n == 0 || n == 1) {
		return false;
	}

	if (num % 2 == 0) {
		return false;
	}

	for (i = 2; i <= n / 2; ++i) {
		// if n is divisible by i, then n is not prime
		// change flag to 1 for non-prime number
		if (n % i == 0) {
			return false;
		}
	}

	return true;
}

#define _BFMathMinMaxForType(type, op, numargs, valist) ({\
	type __res__ = va_arg(valist, type);\
	for (int i = 1; i < numargs; i++) {\
		type num = va_arg(valist, type);\
		if (op == 1) {\
			__res__ = (num > __res__) ? num : __res__;\
		} else if (op == -1) {\
			__res__ = (num < __res__) ? num : __res__;\
		}\
	}\
	__res__;\
})

double _BFMathMinMax(int op, int datatype, int numargs, ...) {
	va_list valist;
	va_start(valist, numargs);

	double res = 0;
	switch (datatype) {
	case _kBFMathGetTypeIDLong:
		res = _BFMathMinMaxForType(long, op, numargs, valist);
		break;
	case _kBFMathGetTypeIDDouble:
	case _kBFMathGetTypeIDFloat:
		res = _BFMathMinMaxForType(double, op, numargs, valist);
		break;
	case _kBFMathGetTypeIDShort:
	case _kBFMathGetTypeIDChar:
	case _kBFMathGetTypeIDInt:
	default:
		res = _BFMathMinMaxForType(int, op, numargs, valist);
		break;
	}

	va_end(valist);

	return res;
}
