/**
 * author: brando
 * date: 11/21/24
 */

#include "rand.h"
#include <stdlib.h>
#include <time.h>

#define kBFRandSeedMod 4294967296U

static unsigned long seed = 1;

void BFRandInit(unsigned long val) {
	seed = val % kBFRandSeedMod;
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

#define kBFRandMultiplier 1103515245U 
#define kBFRandIncrement 12345U
// https://en.wikipedia.org/wiki/Linear_congruential_generator
long BFRandLong() {
	seed = (seed * kBFRandMultiplier + kBFRandIncrement) % kBFRandSeedMod;
    return seed;
}

