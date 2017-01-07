#ifndef BUFFER_H
#define BUFFER_H

#include "line.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct buffer_t {
	char* name;
	struct buffer_t *prev;
	struct buffer_t *next;
	line_t* line;
	line_t* beg;
	line_t* end;
	size_t size;
	size_t col, row;
} buffer_t;

/**
 * Initalizes a buffer
 * @param  name file name to initalize the buffer with (can be null)
 * @return      pointer to the initalized buffer
 */
buffer_t* buffer_create(const char *name);

/**
 * Deinitalizes a buffer
 * @param buf buffer to deinit
 */
void buffer_free(buffer_t *buf);

/**
 * Reads the contents of either a file or the buffer name into a buffer
 * @param  buf  buffer to read into
 * @param  name name of the file (can be null)
 * @return      0 if success, 2 if file did not exist
 */
int buffer_read(buffer_t *buf, const char *name);

/**
 * Writes the contents of a buffer into a file
 * @param  buf  buffer to write to a file
 * @param  name name of the file to write (can be null)
 * @return      0 if success, 2 if could not write
 */
int buffer_write(buffer_t *buf, const char *name);

/**
 * Appends a line to a buffer
 * @param buf  Buffer to append to
 * @param line Line to append
 */
void buffer_append(buffer_t* buf, line_t* line);

/**
 * Prepend a line to a buffer
 * @param buf  Buffer to prepend to
 * @param line Line to prepend
 */
void buffer_prepend(buffer_t* buf, line_t* line);

/**
 * Inster a line at a position in the buffer
 * @param buf  Buffer to insert into
 * @param dest Line to insert
 * @param line Line to append to
 */
void buffer_insert(buffer_t* buf, line_t* dest, line_t* line);

/**
 * Erases a line in a buffer
 * @param buf  buffer to erase line from
 * @param line line to erase
 */
void buffer_erase(buffer_t* buf, line_t* line);

/**
 * Clears all lines in the buffer
 * @param buf buffer to clear lines from
 */
void buffer_clear(buffer_t* buf);

#endif /* BUFFER_H */