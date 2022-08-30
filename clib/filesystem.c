/**
 * author: Brando 
 * date: 6/2/22
 */

#include "filesystem.h"
#include "coreutils.h"
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>

unsigned long long CalculateSizeDirectory(const char * path, unsigned char options, int * err) {
	unsigned long long result = 0;
	int error = 0;
	DIR * dir = 0;
	struct dirent * sdirent = 0;
	char tempPath[PATH_MAX];
	char fullPath[PATH_MAX];
	unsigned long long size = 0;

	dir = opendir(path);
	
	if (dir == 0) {
		error = 1;
		Error("Could not open directory: '%s'", path);
	} else {
		// Go through each item in this directory
		while ((sdirent = readdir(dir)) && !error) {
			// We do not want to calculate the size of parent or current dir
			if (strcmp(sdirent->d_name, "..") && strcmp(sdirent->d_name, ".")) {
				// Construct the path
				sprintf(tempPath, "%s/%s", path, sdirent->d_name);
				
				// Get full path
				if (!realpath(tempPath, fullPath)) {
					error = 1;
				}

				if (!error) {
					size = 0;

					// If we are working with a directory, then we need 
					// too recursively call this function again 
					//
					// If path is not a diretory or a path then we will 
					// do nothing 
					//
					// We will also be ignoring symbolic links 
					if (IsSymbolicLink(fullPath)) {
						size = 0;
					} else if (IsDirectory(fullPath)) {
						size = CalculateSizeDirectory(fullPath, options, &error);
					
					// Otherwise, we will just get the size of this path 
					} else if (IsFile(fullPath)) {
						size = CalculateSizeFile(fullPath, options, &error);
					}

					if (!error) {
						result += size;
					}
				}
			}
               }

               if (closedir(dir)) {
                       error = !error ? 1 : error;
                       Error("Could not close directory: '%s'", path);
               }
       }

       return result;
}

unsigned long long CalculateSizeFile(const char * path, unsigned char options, int * error) {
       unsigned long long result = 0;
       struct stat buf;
       
       if (!stat(path, &buf)) {
               result = buf.st_size;

	       if (options & kCalculateSizeOptionsVerbose) {
			printf("(Bytes: %llu) %s\n", result, path);
	       }
       } else {
               if (error != 0) {
                       *error = 1;
                       Error("Error attempting to calculate file size for: %s", path);
               }
       }

       return result;
}

unsigned long long CalculateSizeForAvailability(const char * path, int * error) {
	unsigned long long result = 0;
	struct statvfs stat;

	if (!statvfs(path, &stat)) {
		result = stat.f_frsize * stat.f_bavail;
	} else {
		if (error != 0) {
			*error = 1;
		}	
	}

	return result;
}

bool PathExists(const char * path) {
	struct stat buffer;
	return (stat(path, &buffer) == 0);
}

bool IsFile(const char * path) {
	struct stat buf;

	if (stat(path, &buf)) {
		return false;
	} else {
		return S_ISREG(buf.st_mode);
	}
}

bool IsDirectory(const char * path) {
	struct stat buf;

	if (stat(path, &buf)) {
		return false;
	} else {
		return S_ISDIR(buf.st_mode);
	}
}

bool IsSymbolicLink(const char * path) {
	struct stat statbuf;

	if (lstat(path, &statbuf)) {
		return false;
	} else {
		return S_ISLNK(statbuf.st_mode);
	}
}

char * CopyHomePath(int * err) {
	char * 	result 	= 0;
	int 	error 	= 0;
	char 	tempPath[PATH_MAX];

	if (!error) {
#if defined(WINDOWS)
		sprintf(tempPath, "%s%s", getenv("HOMEDRIVE"), getenv("HOMEPATH"));
#elif defined(MACOS)
		sprintf(tempPath, "%s", getenv("HOME"));
#elif defined(LINUX)
		sprintf(tempPath, "%s", getenv("HOME"));
#else
//		DLog("No architecture defined for build\n");
		error = 2;
#endif

		if (strlen(tempPath) == 0) {
			error = 1;
		}
	}

	if (!error) {
		result = CopyString(tempPath, &error);
	}

	if (err != 0) {
		*err = error;
	}

	return result;
}

int GetFileExtensionForPath(const char * path, char * buf) {
	int result = 0;
	char * fe = 0;

	if (path == 0) {
		result = 1;
	} else if (buf == 0) {
		result = 2;
	} else {
		fe = strrchr(path, '.');
	}

	if (result == 0) {
		if (fe) {
			if (strlen(fe) > 0) {
				fe++;
				strcpy(buf, fe);
			}
		}
	}

	return result;
}

