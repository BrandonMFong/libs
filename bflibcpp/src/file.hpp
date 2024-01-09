/**
 * author: Brando
 * date: 7/28/22
 */

#ifndef FILE_HPP
#define FILE_HPP

#include <stdio.h>
#include "path.hpp"

#define kReservationSize 128

namespace BF {

class File : public Path {
public:
	/**
	 * If path is null, an error will be returned
	 */
	File(const char * path, int * err);
	virtual ~File();


	/** 
	 * Returns base name of the file
	 *
	 * If path is null, an empty string will always be returned
	 * 
	 * Recommended to copy the result once received
	 */
	const char * name();

	/**
	 * returns name with base name and extension
	 *
	 * Recommended to copy the result once received
	 */
	const char * fullname();

	/** 
	 * Returns the file extension of the path
	 *
	 * an empty string will be returned if path is null
	 *
	 * Recommended to copy the result once received
	 */
	const char * extension();

	/**
	 * Returns our parent directory
	 *
	 * Recommended to copy the result once received
	 */
	const char * directory();

protected:

	/**
	 * Serves as a dedicated FILE object for us.
	 *
	 * Whenever a file is open, it is the 
	 * responsibility of the creator to close the file
	 */
	FILE * _fileHandler;

private:

	/**
	 * Reserved memory for path string examination
	 */
	char _fileReserved[kReservationSize];
};

} // namespace BF

#endif

