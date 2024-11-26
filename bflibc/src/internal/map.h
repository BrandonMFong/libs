/**
 * author: brando
 * date: 11/25/24
 */

#ifndef INTERNAL_MAP_H
#define INTERNAL_MAP_H

/**
 * a pair object, contain key and value of any type
 *
 * use the accessors below to access data
 */
typedef void * BFMapKeyValuePair;
BFMapKey BFMapKeyValuePairGetKey(BFMapKeyValuePair);
BFMapValue BFMapKeyValuePairGetValue(BFMapKeyValuePair);

#endif // INTERNAL_MAP_H

