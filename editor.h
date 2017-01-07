#ifndef EDITOR_H
#define EDITOR_H

#include "types.h"
#include "cmd.h"
#include "window.h"
#include "buffer.h"

#define COUNT(x)	(sizeof(x) / sizeof *(x))
#define CTRL(chr)	(chr & 037)

void set_mode(editor_t* e, enum MODE m);

int q_flag;

void editor_init(editor_t* e, int ac, const char* av[]);
void editor_edit(editor_t* e);
void editor_term(editor_t* e);

#endif /* EDITOR_H */