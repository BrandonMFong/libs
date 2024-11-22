/**
 * author: brando
 * date: 11/20/24
 */

#include "hash.h"

int BFHashDivision(long long key, long long prime) {
	return key % prime;
}

int BFHashMultiplication(long long key, double fractional, long long prime) {
	return (int) prime * ((int)(key * fractional) % 1);
}

