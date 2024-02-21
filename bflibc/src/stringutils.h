/**
 * author: Brando
 * date: 10/5/22
 */

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <uuid/uuid.h>
#include <stdarg.h>

#define kBFStringUUIDStringLength 36

#define BFStringIntegerToChar(i) i + '0'

/**
 * Creates a copy of string
 *
 * Caller must free
 *
 * returns NULL if unsuccessful
 */
char * BFStringCopyString(const char * string);

/**
 * Creates a uuid string for you
 */
void BFStringGetRandomUUIDString(char * uuidString);

/**
 * compares uuid0 with uuid1
 *
 * if params are null, undefined behavior will occur
 *
 * returns memcmp results. see memcmp documentation
 */
int BFStringCompareUUID(const char * uuid0, const char * uuid1);

/**
 * creates a string defined by format
 */
char * BFStringCreateFormatString(const char * format, ...);

/**
 * see `BFStringCreateFormatString`
 */
char * BFStringCreateFormatArgListString(const char * format, va_list valist);

#endif // STRING_UTILS_H

