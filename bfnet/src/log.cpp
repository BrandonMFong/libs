/**
 * author: brando
 * date: 10/27/24
 */

#include "log.hpp"
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>

extern "C" {
#include <bflibc/bflibc.h>
}

using namespace BF::Net;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
void (*cb)(const char * str) = NULL;

void Log::SetCallback(void (*_cb)(const char * str)) {
	pthread_mutex_lock(&m);
	cb = _cb;
	pthread_mutex_unlock(&m);
}

void Log::Write(const char * format, ...) {
	pthread_mutex_lock(&m);
	if (cb) {
		va_list valist; va_start(valist, format);
		char * line = BFStringCreateFormatArgListString(format, valist);
		cb(line);
		free(line);
		va_end(valist);
	}
	pthread_mutex_unlock(&m);
}

