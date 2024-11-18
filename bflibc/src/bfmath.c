/**
 * author: brando
 * date: 11/17/24
 */

#include "bfmath.h"

float BFMathSqrt(float n) {
	float x = n, y = 1;
	while ((x - y) > 0) {
		x = (x + y) / 2;
		y = n / x;
	}
	return x;
}

