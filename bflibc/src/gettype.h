/**
 * author: brando
 * date: 11/20/24
 *
 * The following helps find the type of a variable.
 *
 * ref:
 * 	https://stackoverflow.com/a/57409808/12135693
 */

#ifndef GETTYPE_H
#define GETTYPE_H

#define kGetTypeInt 1
#define kGetTypeShort 2
#define kGetTypeLong 3
#define kGetTypeChar 4
#define kGetTypeFloat 5
#define kGetTypeDouble 6
#define kGetTypeDefault kGetTypeInt

#define BFGetType(x) \
	_Generic(\
		(x),\
		int: kGetTypeInt,\
		short: kGetTypeShort,\
		long: kGetTypeLong,\
		char: kGetTypeChar,\
		float: kGetTypeFloat,\
		double: kGetTypeDouble,\
		default: kGetTypeDefault \
	)

#endif // GETTYPE_H

