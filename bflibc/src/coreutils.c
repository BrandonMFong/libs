/**
 * author: Brando 
 * date: 6/2/22
 */

#include "coreutils.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <libgen.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>

void BFErrorPrint(const char * format, ...) {
	const int logSize = 1024;
	char logString[logSize];
	va_list args;
	
	va_start(args, format);
	vsnprintf(
		logString,
		logSize,
		format,
		args);

	va_end(args);

	printf("Error: %s\n", logString);
}

/**
 * Converts the value to the level specified by scale
 */
double ConvertValueToScale(unsigned long long value, long long scale) {
	double result = 0, d1 = 0;

	result = value / scale;
	d1 = (value % scale);

	result += (d1 / scale);

	return result;
}

int BFByteGetString(unsigned long long byteSize, char * outStr) {
	double value = 0.0;
	char unit[10];

	if (outStr == 0) {
		return 1;
	} else {
		strcpy(outStr, ""); // init 

		// Byte
		// 
		// Default
		value = byteSize;
		strcpy(unit, "b");

		// TeraByte
		if (byteSize >= kBFByteTeraByte) {
			value = ConvertValueToScale(byteSize, kBFByteTeraByte);
			strcpy(unit, "tb");
		// GigaByte
		} else if (byteSize >= kBFByteGigaByte) {
			value = ConvertValueToScale(byteSize, kBFByteGigaByte);
			strcpy(unit, "gb");
		// MegaByte
		} else if (byteSize >= kBFByteMegaByte) {
			value = ConvertValueToScale(byteSize, kBFByteMegaByte);
			strcpy(unit, "mb");
		// KiloByte
		} else if (byteSize >= kBFByteKiloByte) {
			value = ConvertValueToScale(byteSize, kBFByteKiloByte);
			strcpy(unit, "kb");
		}

		sprintf(outStr, "%.2f %s", value, unit);
	}

	return 0;
}

bool BFArrayStringContainsString(char ** strArr, int arrSize, const char * element) {
	for (int i = 0; i < arrSize; i++) {
		if (!strcmp(element, strArr[i])) return true;
	}

	return false;
}

int BFArrayStringGetIndexForString(char ** strArr, int arrSize, const char * element) {
	for (int i = 0; i < arrSize; i++) {
		if (!strcmp(element, strArr[i])) return i;
	}

	return -1;
}

int BFNetGetIPForHostname(const char * hostname, char * ip) {
	int result = 0;
	struct hostent * he;
	struct in_addr ** addrList;

	if (!(he = gethostbyname(hostname))) {
		result = 1;
	}

	if (!result) {
		addrList = (struct in_addr **) he->h_addr_list;

		if (addrList == 0) {
			result = 2;
		}
	}
	
	if (!result) {
		if (addrList[0] == 0) {
			result = 3;
		} else{
			strcpy(ip, inet_ntoa(*addrList[0]));
		}
	}
	
	return result;
}

char * BFStringCreateBinaryStringForNumber(long long num, int byteSize, int * err) {
	int bitSize = byteSize * 8;
	int resultSize = bitSize + (bitSize % 4);
	int error = 0;
	char * result = (char *) malloc(resultSize + 1);

	if (result == 0) {
		error = 1;
	} else {
		for (int i = resultSize - 1, j = 0; (i >= 0); i--, j++) {
			if (j < bitSize) {
				result[i] = (num >> j) & 0x01 ? '1' : '0';
			} else {
				result[i] = '0';
			}
		}

		result[resultSize] = '\0';
	}

	if (err) *err = error;

	return result;
}

