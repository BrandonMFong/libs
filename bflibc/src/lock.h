/**
 * author: brando
 * date: 10/25/23
 */

#ifndef LOCK_H
#define LOCK_H

#include <stdbool.h>

/**
 * Lock object
 *
 * Capabilities: mutex, conditional wait
 */
typedef void * BFLock;

/**
 * Validates BFLock object
 */
bool BFLockIsValid(BFLock * _lock);

/**
 * Caller owns
 */
int BFLockCreate(BFLock * lock);

/**
 * Releases memory
 */
int BFLockDestroy(BFLock * lock);

/**
 * Waits until BFLockRelease is called on object
 */
int BFLockWait(BFLock * lock);

/**
 * Releases BFLock waiting on BFLockWait
 */
int BFLockRelease(BFLock * lock);

/**
 * Activates a mutex lock
 */
int BFLockLock(BFLock * lock);

/**
 * Unlocks a mutex lock
 */
int BFLockUnlock(BFLock * lock);

#endif // LOCK_H

