/**
 * author: Brando
 * date: 2/5/24
 */

#ifndef FILE_WRITER_H
#define FILE_WRITER_H

typedef void * BFFileWriter;

/**
 * creates a thread safe writer to filepath
 */
int BFFileWriterCreate(BFFileWriter * filewriter, const char * filepath);

/**
 * clears file of contents
 */
int BFFileWriterTruncate(BFFileWriter * filewriter);

/**
 * queues up `line` to be written
 */
int BFFileWriterQueueLine(BFFileWriter * filewriter, const char * line);

/**
 * allows line to be formatted
 */
int BFFileWriterQueueFormatLine(BFFileWriter * filewriter, const char * format, ...);

/**
 * Flushes buffer to file
 */
int BFFileWriterFlush(BFFileWriter * filewriter);

/**
 * closes file writing access
 */
int BFFileWriterClose(BFFileWriter * filewriter);

#endif // FILE_WRITER_H

