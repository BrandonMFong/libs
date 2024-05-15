/**
 * author: Brando
 * date: 6/2/22
 */

#ifndef COREUTILS_H
#define COREUTILS_H

#include <stdbool.h>

// MACROS

#define kBFByteKiloByte 1000
#define kBFByteMegaByte (long long) (kBFByteKiloByte * kBFByteKiloByte)
#define kBFByteGigaByte (long long) (kBFByteMegaByte * kBFByteKiloByte)
#define kBFByteTeraByte (long long) (kBFByteGigaByte * kBFByteKiloByte)

#define kBFByteKibiByte 1024
#define kBFByteMebiByte (long long) (kBFByteKibiByte * kBFByteKibiByte)
#define kBFByteGibiByte (long long) (kBFByteMebiByte * kBFByteKibiByte)
#define kBFByteTebiByte (long long) (kBFByteGibiByte * kBFByteKibiByte)

/**
 * Prints std error
 */
void BFErrorPrint(const char * format, ...);

/**
 * Creates a string that represents the byteSize
 *
 * outStr needs to be a valid string buffer that can accept at least 10 bytes of data
 * options : ||||||||0=KiloByte, 1=KibiByte|
 *
 * returns 0 on success, anything else is an error
 */
int BFByteGetString(unsigned long long byteSize, unsigned char options, char * outStr);

/**
 * Sweeps the array to see of it contains the string specified by element
 */
bool BFArrayStringContainsString(char ** strArr, int arrSize, const char * element);

/**
 * returns -1 if string could not be found
 */
int BFArrayStringGetIndexForString(char ** strArr, int arrSize, const char * element);

/**
 * returns the ip address for the hostname
 */
int BFNetGetIPForHostname(const char * hostname, char * ip);

/**
 * Creates binary string representation of the num variable (you know since printf() can't print in binary format
 *
 * size: byte size of num
 *
 * Caller must free
 */
char * BFStringCreateBinaryStringForNumber(long long num, int size, int * err);

#endif // CLIB_H

