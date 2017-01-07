#ifndef WINDOW_H
#define WINDOW_H

#include <ncurses.h>

typedef struct window_t {
    WINDOW* win;
    size_t cols, rows;
    size_t top, bottom;
} window_t;

#endif /* WINDOW_H */
