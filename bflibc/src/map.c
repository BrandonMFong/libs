/**
 * author: brando
 * date: 11/22/24
 */

#include "map.h"
#include "free.h"

typedef struct _BFMap {

} _BFMap;

BFMap BFMapCreate() {
	_BFMap * res = (_BFMap *) malloc(sizeof(_BFMap));
	return (BFMap) res;
}

void BFMapRelease(BFMap map) {
	BFFree(map);
}

int BFMapAdd(BFMap * map, void * key, void * value) {
	return 0;
}

void * BFMapGetValue(BFMap * map, void * key) {
	return 0;
}

int BFMapRemove(BFMap * map, void * key) {
	return 0;
}

