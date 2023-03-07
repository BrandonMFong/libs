/**
 * author: Brando
 * date: 10/5/22
 */

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

/**
 * Creates a copy of string
 *
 * Caller must free
 */
char * BFStringCopyString(const char * string, int * err);

#endif // STRING_UTILS_H

