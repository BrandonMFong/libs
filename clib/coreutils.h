/**
 * author: Brando
 * date: 6/2/22
 */

#ifndef COREUTILS_H
#define COREUTILS_H

#include <stdbool.h>

// MACROS

#define KILOBYTE 1024
#define MEGABYTE (long long) (KILOBYTE * 1024)
#define GIGABYTE (long long) (MEGABYTE * 1024)
#define TERABYTE (long long) (GIGABYTE * 1024)

/**
 * Prints std error
 */
void Error(const char * format, ...);

/**
 * Creates a string that represents the byteSize
 *
 * outStr needs to be a valid string buffer that can accept at least 10 bytes of data
 *
 * returns 0 on success, anything else is an error
 */
int GetByteStringRepresentation(unsigned long long byteSize, char * outStr);

/**
 * Sweeps the array to see of it contains the string specified by element
 */
bool DoesStringArrayContain(char ** strArr, int arrSize, const char * element);

/**
 * returns -1 if string could not be found
 */
int IndexOfStringInArray(char ** strArr, int arrSize, const char * element);

/**
 * returns the ip address for the hostname
 */
int ResolveHostname(const char * hostname, char * ip);

/**
 * Creates binary string representation of the num variable (you know since printf() can't print in binary format
 *
 * size: byte size of num
 *
 * Caller must free
 */
char * CreateBinaryStringFromNumber(long long num, int size, int * err);

#endif // CLIB_H

