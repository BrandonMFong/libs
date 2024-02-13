/**
 * author: brando
 * date: 10/20/23
 */

#ifndef THREAD_H
#define THREAD_H

#include <stdbool.h>
#include "typethreadid.h"

/**
 * returns the count of how many sync/async threads have been launched
 * in current process
 */
int BFThreadGetStartedCount();

/**
 * resets the thread launch count
 */
void BFThreadResetStartedCount();

/**
 * returns the count of how many sync/async threads have been finished
 * in current process
 */
int BFThreadGetStoppedCount();

/**
 * resets the thread stop count
 */
void BFThreadResetStoppedCount();

/**
 * Launches thread and returns when thread terminates
 */
int BFThreadSync(void (* callback)(void *), void * args);

/**
 * Launches an async thread and detaches thread
 *
 * If you want to manage and query an async thread, please
 * consider using BFThreadAsync
 */
__attribute__((deprecated("detach state is now the default behavior. this function is no longer supported")))
int BFThreadAsyncDetach(void (* callback)(void *), void * args);

/**
 * Launches and detaches `callback` on a separate thread
 *
 * will return while thread is running
 *
 * us async id to query the async thread
 *
 * Caller DOES NOT own return object but has the ability to destroy async id.
 * To destroy use BFThreadAsyncIDDestroy
 */
BFThreadAsyncID BFThreadAsync(void (* callback)(void *), void * args);

/**
 * true if we can safely use id
 */
bool BFThreadAsyncIDIsValid(BFThreadAsyncID);

/**
 * returns current thread id
 *
 * caller is not responsible for the memory
 *
 * returns 0 if there was an error
 */
const BFThreadAsyncID BFThreadAsyncGetID();

/**
 * Releases BFThreadAsyncID
 */
void BFThreadAsyncDestroy(BFThreadAsyncID in);

/**
 * returns the error code, if any, for BFThreadAsync
 */
int BFThreadAsyncError(BFThreadAsyncID);

/**
 * true if callback from BFThreadAsync is still running
 *
 * thread safe.  you can use this function to poll the async
 * thread if it's still running
 */
bool BFThreadAsyncIsRunning(BFThreadAsyncID);

/**
 * Sets a flag that is readable 
 *
 * If called, `BFThreadAsyncIsCanceled` will always return true
 *
 * The result of the function is not reversible
 */
int BFThreadAsyncCancel(BFThreadAsyncID);

/**
 * If thread has been canceled
 *
 * caller can safely call this on running thread
 */
bool BFThreadAsyncIsCanceled(BFThreadAsyncID);

#endif // THREAD_H

