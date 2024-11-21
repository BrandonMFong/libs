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

int __BFMathMinMaxInt__(int op, int numargs, va_list valist) {
	int max = 0;
	for (int i = 0; i < numargs; i++) {
		int num = va_arg(valist, int);
		if (op == 1) max = (num > max) ? num : max;
		else if (op == -1) max = (num < max) ? num : max;
	}
	return max;
}

double __BFMathMinMaxDouble__(int op, int numargs, va_list valist) {
	double max = 0;
	for (int i = 0; i < numargs; i++) {
		double num = va_arg(valist, double);
		if (op == 1) max = (num > max) ? num : max;
		else if (op == -1) max = (num < max) ? num : max;
	}
	return max;
}

long __BFMathMinMaxLong__(int op, int numargs, va_list valist) {
	long max = 0;
	for (int i = 0; i < numargs; i++) {
		long num = va_arg(valist, long);
		if (op == 1) max = (num > max) ? num : max;
		else if (op == -1) max = (num < max) ? num : max;
	}
	return max;
}

double __BFMathMinMax__(int op, int datatype, int numargs,...) {
	va_list valist;
	va_start(valist, numargs);

	double res = 0;
	switch (datatype) {
	case kGetTypeLong:
		res = __BFMathMinMaxLong__(op, numargs, valist);
		break;
	case kGetTypeDouble:
	case kGetTypeFloat:
		res = __BFMathMinMaxDouble__(op, numargs, valist);
		break;
	case kGetTypeShort:
	case kGetTypeChar:
	case kGetTypeInt:
	default:
		res = __BFMathMinMaxInt__(op, numargs, valist);
		break;
	}

	va_end(valist);

	return res;
}

