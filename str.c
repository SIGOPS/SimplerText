#include "str.h"

char* strtrim(char* str) {
    // isspace() can check if the character we have been given
    //  white space or not, saves us some work!

    // Get rid of leading whitepsace
    while(isspace(*str)) {
        str++;
    }

    // If the entire string was a space, then just
    // return
    if(*str == '\0') {
        return str;
    }

    // This may look complicated, but remember that strings are
    // just arrays of characters, so we can use pointers to manipulate
    // them.
    //
    // All we are doing here is starting from the end of the string, and simply
    // decrementing the end pointer until we reach some actual text. After that
    // we just add the null terminator, and C will never know that there might
    // exist text after what we called "the end" of the string
    char* end = str + strlen(str) -1;
    while(end > str && isspace(*end)) {
        end--;
    }
    *(end + 1) = '\0';
    return str;
}
