/**
 * author: brando
 * date: 11/17/24
 */

#include "bfmath.h"
#include "var.h"
#include <limits.h>
#include <stdarg.h>

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

//double __BFMathMaxDouble__(int numargs,...) { 
double __BFMathMax__(int numargs,...) {
	va_list valist;
	va_start(valist, numargs);
	va_end(valist);

	double max = 0;
	for (int i = 0; i < numargs; i++) {
		double num = va_arg(valist, double);
		if (num > max) {
			max = num;
		}
	}

	return max;
}

