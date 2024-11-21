/**
 * author: brando
 * date: 11/21/24
 */

#include "rand.h"
#include <stdlib.h>
#include <time.h>

void BFRandInit(unsigned int val) {
	srand(val);
}

int BFRand() {
	return rand();
}

double BFRandDouble() {
	return rand();
}


