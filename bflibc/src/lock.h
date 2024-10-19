/**
 * @file
 * author: brando
 * date: 10/25/23
 */

#ifndef LOCK_H
#define LOCK_H

#include <stdbool.h>
#include "typebftime.h"

/**
 * Lock object
 *
 * Capabilities: mutex, conditional wait
 */
typedef void * BFLock;

/**
 * Validates BFLock object
 */
bool BFLockIsValid(const BFLock * _lock);

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
int BFLockWait(const BFLock * lock);

/**
 * true if lock is being blocked by BFLockWait
 */
bool BFLockIsWaiting(const BFLock * l);

#define kBFLockTimedWaitCodeTimedOut 0xff

/**
 * Waits for a specific amount of time before continuing
 *
 * if elapsed time reaches t, kBFLockTimedWaitCodeTimedOut
 * returns (not necessarily an error)
 *
 * t : seconds with nanosecond resolution. e.g. `1.0` is one second
 */
int BFLockTimedWait(const BFLock * l, BFTime t);

/**
 * Releases BFLock waiting on BFLockWait
 */
int BFLockRelease(const BFLock * lock);

/**
 * Activates a mutex lock
 */
int BFLockLock(const BFLock * lock);

/**
 * Unlocks a mutex lock
 */
int BFLockUnlock(const BFLock * lock);

#endif // LOCK_H

