/**
 * author: brando
 * date: 11/22/24
 *
 * looks like I am going to implement the map in c. I am going to 
 * use a self balancing tree since search/insert/delete are O(logn)
 */

#ifndef MAP_H
#define MAP_H

#include <stddef.h>
#include <stdbool.h>

typedef void * BFMap;
typedef void * BFMapKey;
typedef void * BFMapValue;

/**
 * Caller owns memory
 */
BFMap BFMapCreate();

/**
 * sets a compare callback that compares keys during our operations
 */
void BFMapSetCompare(BFMap map, int (*compare)(BFMapKey a, BFMapKey b));

/**
 * defines how key and value are released when BFMapRemove is called
 */
void BFMapSetRelease(BFMap map, void (*release)(BFMapKey key, BFMapValue value));

/**
 * releases map
 */
void BFMapRelease(BFMap map);

/**
 * returns size of map (number of key/value pairs)
 */
size_t BFMapGetSize(BFMap map);

/**
 * Adds entry for key and value
 *
 * key: can be a pointer or integer, 0 is allowed
 * value: can be a pointer or integer, 0 is allowed
 */
int BFMapInsert(BFMap map, BFMapKey key, BFMapValue value);

/**
 * error: nonzero if no pair was found
 *
 * returns value for key. Can be null based on what the caller inserted
 * into map. I.e. the caller could have defined keys=char* & value=int,
 * which permits value to be a zero
 */
BFMapValue BFMapGetValue(BFMap map, BFMapKey key, int * error);

/**
 * removes entry with key
 */
int BFMapRemove(BFMap map, BFMapKey key);

/**
 * true if map contains key
 */
bool BFMapContains(BFMap map, BFMapKey key);

#endif // MAP_H

