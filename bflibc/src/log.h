/**
 * author: Brando
 * date: 7/29/22
 */

#ifdef DEBUG

#include <stdio.h>

#define BFDLog(...) printf("[%s:%d]: ",  __FILE__, __LINE__); printf(__VA_ARGS__); printf("\n")

#else 

#define BFDLog(...)

#endif // DEBUG

