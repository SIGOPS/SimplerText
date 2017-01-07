#include <ncurses.h>
#include "editor.h"

int main(int argc, const char* argv[]) {
	initscr();
	raw();
	noecho();
	start_color();

	editor_t e = {};
	editor_init(&e, argc, argv);
	editor_edit(&e);
	editor_term(&e);
	
	endwin();
	return 0;
}
