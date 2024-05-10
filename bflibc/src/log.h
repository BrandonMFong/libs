/**
 * author: Brando
 * date: 7/29/22
 */

#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#ifdef DEBUG
#define BFDLog(...) printf("[%s:%d]: ",  __FILE__, __LINE__); printf(__VA_ARGS__); printf("\n")
#else 
#define BFDLog(...)
#endif // DEBUG

#endif // LOG_H

