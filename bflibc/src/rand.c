/**
 * author: brando
 * date: 11/21/24
 */

#include "rand.h"
#include <stdlib.h>
#include <time.h>

static unsigned long seed = 1;

void BFRandInit(unsigned int val) {
	seed = (unsigned)val & 0x7fffffffU;
}

int BFRand() {
	return (int) BFRandLong();
}

double BFRandDouble() {
	double num = BFRandLong();
	double fractional = BFRandLong();
	while((int) fractional > 0) {
		fractional /= 10;
	}
	return num + fractional;
}

// https://en.wikipedia.org/wiki/Linear_congruential_generator
long BFRandLong() {
	seed = (seed * 1103515245U + 12345U) & 0x7fffffffU;
    return seed;
}

