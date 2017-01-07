#include "line.h"

line_t* line_create(const char* str, size_t length) {
    line_t* l = calloc(1, sizeof(line_t));
    if(str) {
        strncpy(l->str, str, length);
    }
    return l;
}

void line_free(line_t* line) {
    line->str[0] = '\0';
    line->next = line->prev = NULL;
    free(line);
}

void line_concat(line_t* line, const char* str, size_t length) {
    strncat(line->str, str, length);
}

void line_insert(line_t* line, char chr, size_t idx) {
    // Inserting into the middle of a string
    if(idx < strlen(line->str)) {
        char* split = line->str + idx;
        // memmove moves memory from a source to a destination.
        // Here we are using it to make space to insert a character
        // by moving everything past the split one over
        memmove(split + 1, split, strlen(split));
    }
    // Unconditionlly insert character
    line->str[idx] = chr;
}

void line_delete(line_t* line, size_t idx) {
    // Some error checking, but otherwise inverse logic of insert
    if(idx < strlen(line->str)) {
        char* split = line->str + idx;
        memmove(split, split + 1, strlen(split));
    }
}
