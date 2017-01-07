#ifndef LINE_H
#define LINE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct line_t {
    // BUFSIZ is a macro that gives the best system optimized
    // size for a buffer (https://www.gnu.org/software/libc/manual/html_node/Controlling-Buffering.html)
    char str[BUFSIZ];

    // We will used a linked list like structure to hold
    // our line information. It allows for easy reordering
    // and even history in the future.
    struct line_t *prev;
    struct line_t *next;
} line_t;

/*
 * line_create(): Initalizes a line_t type
 *
 * return: line_t*   -> Pointer to the initalized line
 *
 * const char* str   -> String to initalize line to
 * size_t length     -> Length of string used to initalize line
 */
line_t* line_create(const char* str, size_t length);

/*
 * line_free(): deinitalizes a line_t type
 *
 * return: void
 *
 * line_t* line      -> Line to deinitalize
 */
void line_free(line_t* line);

/*
 * line_concat(): Concatinates the given line with the provided string
 *
 * return: void
 *
 * line_t* line      -> Line to concatinate to
 * const char* str   -> String to append/concatinate to the line
 * size_t length     -> Length of provided string
 */
void line_concat(line_t* line, const char* str, size_t length);

/*
 * line_insert(): Inserts a character at the provided position in to the
 *  given line
 *
 * return: void
 *
 * line_t* line      -> Line to insert a character in
 * char chr          -> Character to insert into the line
 * size_t idx        -> Index of the line to insert the character into
 */
void line_insert(line_t* line, char chr, size_t idx);

/*
 * line_erase(): Erases a character at the provided position in to the
 *  given line
 *
 * return: void
 *
 * line_t* line      -> Line to erase a character in
 * size_t idx        -> Index of the character to erase
 */
void line_delete(line_t* line, size_t idx);

#endif /* LINE_H */
