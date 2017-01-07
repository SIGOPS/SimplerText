#ifndef CMD_H
#define CMD_H

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "str.h"
#include "buffer.h"
#include "line.h"
#include "editor.h"

extern int q_flag;

int command(editor_t* e, int key);
int read_file(editor_t* e, int key);
int write_file(editor_t* e, int key);
int beg_of_line(editor_t* e, int key);
int end_of_line(editor_t* e, int key);
int next_char(editor_t* e, int key);
int prev_char(editor_t* e, int key);
int next_word(editor_t* e, int key);
int prev_word(editor_t* e, int key);
int next_line(editor_t* e, int key);
int prev_line(editor_t* e, int key);
int new_line(editor_t* e, int key);
int back_char(editor_t* e, int key);
int del_char(editor_t* e, int key);
int add_char(editor_t* e, int key);
int add_tab(editor_t* e, int key);
int new_buffer(editor_t* e, int key);
int del_buffer(editor_t* e, int key);
int next_buffer(editor_t* e, int key);
int prev_buffer(editor_t* e, int key);
int beg_of_buffer(editor_t* e, int key);
int end_of_buffer(editor_t* e, int key);

#endif /* CMD_H */