#ifndef STR_H
#define STR_H

#include <ctype.h>
#include <stdio.h>
#include <string.h>

/*
 * strtrim - Function trims whitespace from the given string
 * 
 * return: char*  -> string that has excess whitepsace removed
 *
 * input:
 *  char* str     -> string to remove whitepsace from
 */
char* strtrim(char* str);

#endif /* STR_H */
