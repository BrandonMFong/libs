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

BFSet BFSetCreate();
void BFSetSetCompare(BFSet set, int (*compare)(BFSetValue a, BFSetValue b));
void BFSetSetRelease(BFSet set, void (*release)(BFSetValue value));
void BFSetRelease(BFSet set);
int BFSetInsert(BFSet set, BFSetValue value);
int BFSetRemove(BFSet set, BFSetValue value);
bool BFSetContains(BFSet set, BFSetValue value);
size_t BFSetGetSize(BFSet set);

#endif // SET_H

