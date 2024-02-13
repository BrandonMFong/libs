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
 * Releases BFThreadAsyncID
 */
void BFThreadAsyncIDDestroy(BFThreadAsyncID in);

/**
 * returns the error code, if any, for BFThreadAsync
 */
int BFThreadAsyncIDError(BFThreadAsyncID);

/**
 * true if callback from BFThreadAsync is still running
 *
 * thread safe.  you can use this function to poll the async
 * thread if it's still running
 */
bool BFThreadAsyncIDIsRunning(BFThreadAsyncID);

/**
 * Cancels async thread
 */
int BFThreadAsyncCancel(BFThreadAsyncID);

#endif // THREAD_H

