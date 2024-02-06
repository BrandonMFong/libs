/**
 * author: brando
 * date: 2/5/24
 */

#include "filewriter.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	FILE * file;
} _FileWriter;

int FileWriterCreate(FileWriter * filewriter, const char * filepath) {
	if (!filewriter || !filepath) return -1;

	_FileWriter * fw = malloc(sizeof(_FileWriter));
	if (!fw) return -1;

	fw->file = fopen(filepath, "w");
	if (!fw->file) return -1;

	*filewriter = (FileWriter *) fw;

	return 0;
}

int FileWriterQueueLine(FileWriter * filewriter, const char * line) {
	return 0;
}

int FileWriterClose(FileWriter * filewriter) {
	if (!filewriter) return -3;
	_FileWriter * fw = *filewriter;

	fclose(fw->file);

	free(fw);

	return 0;
}

