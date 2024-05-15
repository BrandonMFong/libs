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

int BFByteGetString(unsigned long long byteSize, unsigned char options, char * outStr) {
	double value = 0.0;
	char unit[10];
	const bool usekibi = options & 0x01;
	const char * b_str = "B";
	const char * kb_str = usekibi ? "KiB" : "KB";
	const char * mb_str = usekibi ? "MiB" : "MB";
	const char * gb_str = usekibi ? "GiB" : "GB";
	const char * tb_str = usekibi ? "TiB" : "TB";
	const size_t kb_size = usekibi ? kBFByteKibiByte : kBFByteKiloByte;
	const size_t mb_size = usekibi ? kBFByteMebiByte : kBFByteMegaByte;
	const size_t gb_size = usekibi ? kBFByteGibiByte : kBFByteGigaByte;
	const size_t tb_size = usekibi ? kBFByteTebiByte : kBFByteTeraByte;

	if (outStr == 0) {
		return 1;
	} else {
		strcpy(outStr, ""); // init 

		// Byte
		// 
		// Default
		value = byteSize;
		strcpy(unit, b_str);

		// TeraByte
		if (byteSize >= tb_size) {
			value = ConvertValueToScale(byteSize, tb_size);
			strcpy(unit, tb_str);
		// GigaByte
		} else if (byteSize >= gb_size) {
			value = ConvertValueToScale(byteSize, gb_size);
			strcpy(unit, gb_str);
		// MegaByte
		} else if (byteSize >= mb_size) {
			value = ConvertValueToScale(byteSize, mb_size);
			strcpy(unit, mb_str);
		// KiloByte
		} else if (byteSize >= kb_size) {
			value = ConvertValueToScale(byteSize, kb_size);
			strcpy(unit, kb_str);
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

