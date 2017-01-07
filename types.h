#ifndef TYPES_H
#define TYPES_H

#include "window.h"
#include "buffer.h"

enum MODE {
	NORMAL,
	INSERT,
};

// tt because a mode_t already exists
typedef struct mode_tt {
	int exit_k;
	struct cmd_t *cmd;
	size_t count;
	char* name;
} mode_tt;

typedef struct editor_t {
	window_t cmd_win;
	window_t title_win;
	window_t text_win;

	buffer_t *buf;
	buffer_t *beg;
	buffer_t *end;

	mode_tt mode;
} editor_t;

typedef struct cmd_t {
	int key;
	int (*func)(editor_t*, int);
	enum MODE next_mode;
} cmd_t;

#endif