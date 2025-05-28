/**
 * author: brando
 * date: 5/14/25
 */

#ifndef SET_H
#define SET_H

#include <stdbool.h>
#include <stddef.h>

typedef void * BFSet;
typedef void * BFSetValue;

/**
 * creates set
 *
 * caller owns memory
 */
BFSet BFSetCreate();

/**
 * sets how the values are compared to one another
 */
void BFSetSetCompare(BFSet set, int (*compare)(BFSetValue a, BFSetValue b));

/**
 * how each value memory is handled once set is released
 */
void BFSetSetRelease(BFSet set, void (*release)(BFSetValue value));

/**
 * releases set from memory
 */
void BFSetRelease(BFSet set);

/**
 * inserts value into set
 */
int BFSetInsert(BFSet set, BFSetValue value);

/**
 * removes value from set
 */
int BFSetRemove(BFSet set, BFSetValue value);

/**
 * checks if value is in set
 */
bool BFSetContains(BFSet set, BFSetValue value);

/**
 * returns the current size of set
 */
size_t BFSetGetSize(BFSet set);

#endif // SET_H

