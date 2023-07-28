/**
 * author: Brando
 * date: 6/2/22
 */

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdbool.h>

/**
 * Tells the CalculateSize methods to print out each path it finds and byte size
 */
static const unsigned char kCalculateSizeOptionsVerbose = (0x01) << 0;

/**
 * Calculates the directory size recursively
 *
 * options: using anything from the kCalculationSizeOptions... set
 */
unsigned long long BFFileSystemDirectoryGetSizeUsed(const char * path, unsigned char options, int * error);

/**
 * Calculates the file size of the path  
 */
unsigned long long BFFileSystemFileGetSizeUsed(const char * path, unsigned char options, int * error);

/**
 * Returns the available space in bytes for the path
 */
unsigned long long BFFileSystemPathGetSizeAvailable(const char * path, int * error);

/**
 * Tests if path exists
 */
bool BFFileSystemPathExists(const char * path);

/**
 * Tests if path is a file
 */
bool BFFileSystemPathIsFile(const char * path);

/**
 * Tests if path is a directory
 */
bool BFFileSystemPathIsDirectory(const char * path);

/**
 * Tests if the path is a symbolic link 
 */
bool BFFileSystemPathIsSymbolicLink(const char * path);

/**
 * Returns a copy of the home path for the user in scope
 *
 * Caller must free
 */
char * BFFileSystemPathCopyHomePath(int * err);

/**
 * returns operating system's tmp directory
 */
int BFFileSystemGetOSTempDirectory(char * path);

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
int BFFileSystemPathGetExtension(const char * path, char * buf);

/**
 * Recursively removes path
 */
int BFFileSystemRemoveAll(const char * path);

#endif // FILESYSTEM_H

