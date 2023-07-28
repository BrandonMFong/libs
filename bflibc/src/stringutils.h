/**
 * author: Brando
 * date: 10/5/22
 */

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <uuid/uuid.h>

#define kBFStringUUIDStringLength UUID_STR_LEN - 1

#define BFStringIntegerToChar(i) i + '0'

/**
 * Creates a copy of string
 *
 * Caller must free
 */
char * BFStringCopyString(const char * string, int * err);

/**
 * Creates a uuid string for you
 */
void BFStringGetRandomUUIDString(char * uuidString);

#endif // STRING_UTILS_H

