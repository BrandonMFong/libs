/**
 * author: brando
 * date: 5/14/25
 */

#ifndef HASHSET_H
#define HASHSET_H

#include <stdbool.h>
#include <stddef.h>

typedef void * BFHashSet;
typedef void * BFHashSetValue;

/**
 * creates set
 *
 * caller owns memory
 */
BFHashSet BFHashSetCreate();

/**
 * sets how the values are compared to one another
 */
void BFHashSetSetCompare(BFHashSet set, int (*compare)(BFHashSetValue a, BFHashSetValue b));

/**
 * how each value memory is handled once set is released
 */
void BFHashSetSetRelease(BFHashSet set, void (*release)(BFHashSetValue value));

/**
 * sets the hash function
 */
void BFHashSetSetHashFunction(BFHashSet set, unsigned long (*hash)(BFHashSetValue key));

/**
 * releases set from memory
 */
void BFHashSetRelease(BFHashSet set);

/**
 * inserts value into set
 */
int BFHashSetInsert(BFHashSet set, BFHashSetValue value);

/**
 * removes value from set
 */
int BFHashSetRemove(BFHashSet set, BFHashSetValue value);

/**
 * checks if value is in set
 */
bool BFHashSetContains(BFHashSet set, BFHashSetValue value);

/**
 * returns the current size of set
 */
size_t BFHashSetGetSize(BFHashSet set);

#endif // HASHSET_H

