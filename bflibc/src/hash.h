/**
 * author: brando
 * date: 11/20/24
 *
 * https://www.geeksforgeeks.org/hash-functions-and-list-types-of-hash-functions/
 */

#ifndef HASH_H
#define HASH_H

/**
 * h(k)=k mod m
 * k = key
 * m = prime number
 */
int BFHashDivision(long long key, long long prime);

/**
 * h(k)=⌊m(kAmod1)⌋
 * k = key
 * m = prime num
 */
int BFHashMultiplication();
int BFHashMidSquare();
int BFHashFolding();

#endif // HASH_H

