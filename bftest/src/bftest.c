/**
 * author: brando
 * date: 11/13/24
 */

#include "bftest.h"
#include <stdarg.h>

void BFTestLogPush(const char * format, ...) {
	va_list valist;
	va_start(valist, format);
	vprintf(format, valist);
	va_end(valist);
}

