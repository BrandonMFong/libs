/**
 * author: Brando
 * date: 6/2/22
 */

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdbool.h>

// MACROS

#define PATH_MAX 4096

/**
 * Tells the CalculateSize methods to print out each path it finds and byte size
 */
static const unsigned char kCalculateSizeOptionsVerbose = (0x01) << 0;

/**
 * Calculates the directory size recursively
 *
 * options: using anything from the kCalculationSizeOptions... set
 */
unsigned long long CalculateSizeDirectory(const char * path, unsigned char options, int * error);

/**
 * Calculates the file size of the path  
 */
unsigned long long CalculateSizeFile(const char * path, unsigned char options, int * error);

/**
 * Returns the available space in bytes for the path
 */
unsigned long long CalculateSizeForAvailability(const char * path, int * error);

/**
 * Tests if path exists
 */
bool PathExists(const char * path);

/**
 * Tests if path is a file
 */
bool IsFile(const char * path);

/**
 * Tests if path is a directory
 */
bool IsDirectory(const char * path);

/**
 * Tests if the path is a symbolic link 
 */
bool IsSymbolicLink(const char * path);

/**
 * Returns a copy of the home path for the user in scope
 *
 * Caller must free
 */
char * CopyHomePath(int * err);

/**
 * Upon success, buf will have the file extension for the 
 * input path. If no input path was found, nothing will
 * change in buf and no error code will be returned.  
 *
 * An error code will be returned for a fundamental issue
 * like the parameters being null
 *
 * If buf does not have enough space for the file extension
 * string, undefined behavior will occur
 */
int GetFileExtensionForPath(const char * path, char * buf);

#endif // FILESYSTEM_H

