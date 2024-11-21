/**
 * author: brando
 * date: 11/17/24
 */

#include "bfmath.h"
#include <limits.h>
#include <stdarg.h>
#include <bftest/bftest.h>

float BFMathSqrt(float n) {
	float x = n, y = 1;
	while ((x - y) > kBFMathSqrtFactor) {
		x = (x + y) / 2;
		y = n / x;
	}
	return x;
}

int BFMathPrimeGetNumberAtIndex(int index) {
	for (int i = 0; i < INT_MAX; i++) {
		if (BFMathPrimeIsPrime(i)) {
			if (index == 0) {
				return i;
			}

			index--;
		}
	}
	return -1;
}

bool BFMathPrimeIsPrime(int num) {
	if (num <= 1) {
		return false; 
	} else if (num % 2 == 0) {
		return num == 2;
	}

	int maxdiv = (int) BFMathSqrt(num);
	for (int i = 3; i < maxdiv + 1; i += 2) {
		if (num % i == 0) {
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
	case kGetTypeLong:
		res = _BFMathMinMaxForType(long, op, numargs, valist);
		break;
	case kGetTypeDouble:
	case kGetTypeFloat:
		res = _BFMathMinMaxForType(double, op, numargs, valist);
		break;
	case kGetTypeShort:
	case kGetTypeChar:
	case kGetTypeInt:
	default:
		res = _BFMathMinMaxForType(int, op, numargs, valist);
		break;
	}

	va_end(valist);

	return res;
}
