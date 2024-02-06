/**
 * author: Brando
 * date: 2/5/24
 */

#ifndef FILE_WRITER_H
#define FILE_WRITER_H

typedef void * FileWriter;

/**
 * creates a thread safe writer to filepath
 */
int FileWriterCreate(FileWriter * filewriter, const char * filepath);

/**
 * queues up `line` to be written
 */
int FileWriterQueueLine(FileWriter * filewriter, const char * line);

int FileWriterFlush(FileWriter * filewriter);

/**
 * closes file writing access
 */
int FileWriterClose(FileWriter * filewriter);

#endif // FILE_WRITER_H

