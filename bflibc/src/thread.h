/**
 * author: brando
 * date: 10/20/23
 */

#ifndef THREAD_H
#define THREAD_H

#include <stdbool.h>

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
int BFThreadAsyncDetach(void (* callback)(void *), void * args);

/**
 * Object that describes an async thread 
 */
typedef void * BFThreadAsyncID;

/**
 * Launches and detaches `callback` on a separate thread
 *
 * will return while thread is running
 *
 * us async id to query the async thread
 *
 * Caller owns return object, destroy using BFThreadAsyncIDDestroy
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
 */
bool BFThreadAsyncIDIsRunning(BFThreadAsyncID);

/**
 * Cancels async thread
 */
int BFThreadAsyncCancel(BFThreadAsyncID);

#endif // THREAD_H

