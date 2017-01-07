#include "cmd.h"

// Couple of private functions
static void get_input(editor_t* e, char* input, const char* cmd) {
	// This function puts a string in a ncurses window, add moves the
	// cursor as well
	// Here we are creating a vim like "command prompt"
	mvwaddstr(e->cmd_win.win, 0,0, ":");
	// Refresh the window
	wrefresh(e->cmd_win.win);

	// Display the user's input
	echo();
	// Get a string from a ncurses window
	wgetstr(e->cmd_win.win, input);
	// The whole "echo" and "noecho" are used to tell if ncurses should
	// "echo" or display what the user typed in. Since we usually handle
	// all the input manually, we turn it off here
	noecho();

	// Clear the command window
	wclear(e->cmd_win.win);
	wrefresh(e->cmd_win.win);
}

static int run_cmd(editor_t* e, const char* cmd, const char* arg) {
	// Save the file
	if(strcmp(cmd, "w") == 0) {
		return buffer_write(e->buf, arg);
	}
	// Edit or open a new file
	if(strcmp(cmd, "edit") == 0) {
		if (arg) {
			free(e->buf->name);
			e->buf->name = strdup(arg);
		}
		return buffer_read(e->buf, arg);
	}
	// bad argument
	return -1;
}

int command(editor_t* e, int key) {
	char input[BUFSIZ] = {};
	get_input(e, input, NULL);
	if (strlen(input) == 0) {
		return -1;
	}

	// Parse the command and then run it
	char* cmd = strtrim(input);
	char* arg = strchr(cmd, ' ');
	if (arg) {
		*arg = '\0';
		arg = arg + 1;
	}
	return run_cmd(e, cmd, arg);
}

int read_file(editor_t* e, int key) {
	return buffer_read(e->buf, NULL);
}

int write_file(editor_t* e, int key) {
	if(!e->buf->name) {
		char input[BUFSIZ] = {};
		get_input(e, input, NULL);
		// User didn't put anything in
		if (strlen(input) == 0) {
			return -1;
		} else {
			// Set the name
			e->buf->name = input;
		}
	}
	return buffer_write(e->buf, NULL);
}

int beg_of_line(editor_t* e, int key) {
	e->buf->col = 0;
	return 1;
}

int end_of_line(editor_t* e, int key) {
	e->buf->col = strlen(e->buf->line->str);
	return 1;
}

int next_char(editor_t* e, int key) {
	// If the next character is on the same line
	if(e->buf->col + 1 <= strlen(e->buf->line->str)) {
		e->buf->col++;
		return 1;
	} else if (e->buf->line->next) {
		// Otherwise move to next line
		next_line(e, key);
		beg_of_line(e, key);
		return 1;
	}
	return 0;
}

int prev_char(editor_t* e, int key) {
	if(e->buf->col + 1 > 0) {
		e->buf->col--;
		return 1;
	} else if (e->buf->line->prev) {
		prev_line(e, key);
		end_of_line(e, key);
		return 1;
	}
	return 0;
}

int next_word(editor_t* e, int key) {
	// Basically, find the next space and move there
	while(next_char(e, key)) {
		char ch = e->buf->line->str[e->buf->col];
		if(!isblank(ch) && ch != '\0') {
			if (e->buf->col == 0) {
				return 1;
			} else {
				char pr_char = e->buf->line->str[e->buf->col - 1];
				if (isblank(pr_char)) {
					return 1;
				}
			}
		}
	}
	return 0;
}

int prev_word(editor_t* e, int key) {
	while(prev_char(e, key)) {
		char ch = e->buf->line->str[e->buf->col];
		if(!isblank(ch) && ch != '\0') {
			if (e->buf->col == 0) {
				return 1;
			} else {
				char pr_char = e->buf->line->str[e->buf->col - 1];
				if (isblank(pr_char)) {
					return 1;
				}
			}
		}
	}
	return 0;
}

int next_line(editor_t* e, int key) {
	line_t* next_l = e->buf->line->next;
	int ret_val = 0;
	if(next_l) {
		e->buf->line = next_l;
		e->buf->row++;
		ret_val = 1;
	}
	if (e->buf->col > strlen(e->buf->line->str)) {
		end_of_line(e, key);
	}
	return ret_val;
}

int prev_line(editor_t* e, int key) {
	line_t* prev_l = e->buf->line->prev;
	int ret_val = 0;
	if(prev_l) {
		e->buf->line = prev_l;
		e->buf->row--;
		ret_val = 1;
	}
	if (e->buf->col > strlen(e->buf->line->str)) {
		end_of_line(e, key);
	}
	return ret_val;
}

int new_line(editor_t* e, int key) {
	char* split = e->buf->line->str + e->buf->col;
	size_t len = strlen(split);
	line_t* l = line_create(split, len);
	memset(split, '\0', len);
	if(e->buf->line->next) {
		buffer_insert(e->buf, e->buf->line->next, l);
	} else {
		buffer_append(e->buf, l);
	}
	next_line(e, key);
	beg_of_line(e, key);
	return 1;
}

int back_char(editor_t* e, int key) {
	line_t* l = e->buf->line;
	prev_char(e, key);

	if (e->buf->line == l->prev) {
		line_concat(e->buf->line, l->str, strlen(l->str));
		buffer_erase(e->buf, l);
	} else {
		line_delete(e->buf->line, e->buf->col);
	}

	return 1;
}

int del_char(editor_t* e, int key) {
	line_delete(e->buf->line, e->buf->col);
	return 1;
}

int add_char(editor_t* e, int key) {
	line_insert(e->buf->line, key, e->buf->col);
	next_char(e, key);
	return 1;
}

int add_tab(editor_t* e, int key) {
	int i;
	for(i = 0; i < 4; i++) {
		add_char(e, ' ');
	}
	return 1;
}

int new_buffer(editor_t* e, int key) {
	buffer_t* buf = buffer_create(NULL);
	buf->next = e->buf;
	buf->prev = e->buf->prev;
	if (buf->next) {
		buf->next->prev = buf;
	}
	if (buf->prev) {
		buf->prev->next = buf;
	}
	if (e->beg == e->buf) {
		e->beg = buf;
	}

	e->buf = buf;
	return 1;
}

int del_buffer(editor_t* e, int key) {
	buffer_t* buf = e->buf;
	if (buf == e->beg) {
		e->beg = buf->next;
	} else {
		buf->prev->next = buf->next;
	}

	if(buf == e->end) {
		e->end = buf->prev;
	} else {
		buf->next->prev = buf->prev;
	}

	e->buf = buf->next ?: e->beg;
	buffer_free(buf);
	return 1;
}

int next_buffer(editor_t* e, int key) {
	if (e->buf->next) {
		e->buf = e->buf->next;
	} else {
		e->buf = e->beg;
	}
	return 1;
}

int prev_buffer(editor_t* e, int key){
	if (e->buf->prev) {
		e->buf = e->buf->prev;
	} else {
		e->buf = e->end;
	}
	return 1;
}

int beg_of_buffer(editor_t* e, int key){
	e->buf->line = e->buf->beg;
	e->buf->row = 0;
	beg_of_line(e, key);
	return 1;
}

int end_of_buffer(editor_t* e, int key){
	e->buf->line = e->buf->end;
	e->buf->row = e->buf->size - 1;
	end_of_line(e, key);
	return 1;
}
