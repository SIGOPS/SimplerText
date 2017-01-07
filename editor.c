#include "editor.h"

// Function pointer array of normal mode commands
static cmd_t normal_mode_cmds[] = {
    // Editor Operations
    {CTRL('s'), write_file, NORMAL},
    {CTRL('o'), read_file, NORMAL},

    // Opening and closing buffers
    {CTRL('t'), new_buffer, NORMAL},
    {CTRL('w'), del_buffer, NORMAL},

    // Buffer Switching
    {CTRL('n'), next_buffer, NORMAL},
    {CTRL('p'), prev_buffer, NORMAL},

    // Change Mode
    {'i', NULL, INSERT},

    // File Movement
    {'g', beg_of_buffer, NORMAL},
    {'G', end_of_buffer, NORMAL},
    {'a', next_char, INSERT},

    // Movement
    {KEY_LEFT, prev_char, NORMAL},
    {KEY_DOWN, next_line, NORMAL},
    {KEY_UP, prev_line, NORMAL},
    {KEY_RIGHT, next_char, NORMAL},

    // Vim style movement
    {'w', next_word, NORMAL},
    {'b', prev_word, NORMAL},
    {'x', del_char, NORMAL},

    // Command Mode
    {':', command, NORMAL},

    // Anything else
    {'\0', NULL, NORMAL}
};

static const mode_tt normal_mode = {
    .exit_k = CTRL('q'),
    .cmd = normal_mode_cmds,
    .count = COUNT(normal_mode_cmds),
    .name = "NORMAL"
};

static cmd_t insert_mode_cmds[] = {
    // Save file
    {CTRL('s'), write_file, INSERT},

    // Movement
    {KEY_HOME, beg_of_line, INSERT},
    {KEY_END, end_of_line, INSERT},

    // Arrow Keys
    {KEY_UP, prev_line, INSERT},
    {KEY_DOWN, next_line, INSERT},
    {KEY_RIGHT, next_char, INSERT},
    {KEY_LEFT, prev_char, INSERT},

    // Enter Keys
    {'\n', new_line, INSERT},
    {'\r', new_line, INSERT},

    // Backspace
    {127, back_char, INSERT},
    {KEY_BACKSPACE, back_char, INSERT},

    // Delete
    {KEY_DC, del_char, INSERT},

    // ESC
    {27, NULL, NORMAL},

    // Tab needs a special key
    {9, add_tab, INSERT},
    {KEY_BTAB, add_tab, INSERT},
    {KEY_CTAB, add_tab, INSERT},
    {KEY_CATAB, add_tab, INSERT},

    // Anything else
    {'\0', add_char, INSERT}
};

static const mode_tt insert_mode = {
    .exit_k = CTRL('q'),
    .cmd = insert_mode_cmds,
    .count = COUNT(insert_mode_cmds),
    .name = "- INSERT -"
};

void set_mode(editor_t* e, enum MODE m) {
    switch (m) {
        // Change mode based on passed enum
        case INSERT:
            e->mode = insert_mode;
            break;
        case NORMAL:
            e->mode = normal_mode;
            break;
    }
}

void editor_init(editor_t* e, int ac, const char* av[]) {
    const size_t cols = COLS;
    const size_t lines = LINES;

    // Create the window for the command area
    e->cmd_win.win = newwin(1, cols, lines - 1, 0);
    e->cmd_win.cols = cols;
    e->cmd_win.rows = 1;

    // Create the window for the title/info bar
    e->title_win.win = newwin(1, cols, lines - 2, 0);
    e->title_win.cols = cols;
    e->title_win.rows = 1;
    // This just changes the highlighting so the foreground and
    // background colors are switched
    wattron(e->title_win.win, A_REVERSE);

    // Create the window for the text area
    e->text_win.win = newwin(lines - 2, cols, 0, 0);
    e->text_win.cols = cols;
    e->text_win.rows = lines - 2;
    e->text_win.bottom = e->text_win.rows - 1;
    // Enable the keypad
    keypad(e->text_win.win, 1);

    int i;
    for (i = ac - 1; i > 0; i--) {
        // Create a buffer for each file passed in
        buffer_t* buf = buffer_create(av[i]);
        if (!e->end) {
            e->buf = e->end = buf;
        } else {
            e->buf->prev = buf;
            buf->next = e->buf;
            e->buf = buf;
        }
    }
    e->beg = e->buf;

    set_mode(e, NORMAL);
}

void editor_term(editor_t* e) {
    buffer_t* buf = e->beg;
    // Go through and de-initalize all open buffers
    while (buf) {
        buffer_t* n_buf = buf->next;
        buffer_free(n_buf);
        buf = n_buf;
    }
    // Delete the open windows
    delwin(e->cmd_win.win);
    delwin(e->title_win.win);
    delwin(e->text_win.win);
}

void editor_display(editor_t* e) {
    // Make a buffer the width of the screen
    buffer_t* buf = e->buf;

    // Clear the current title bar
    wclear(e->title_win.win);

    // Print out the mode
    mvwprintw(e->title_win.win, 0,0, " %s > ", e->mode.name);

    // We add 3 due to the spacing characters and 1 for additional padding
    size_t curr_x_pos = strlen(e->mode.name) + 4;

    // Print out all the open buffers
    do {
        if(buf != e->buf){
            wattron(e->title_win.win, A_BOLD);
        }
        if(buf->name) {
            mvwprintw(e->title_win.win, 0, curr_x_pos, "%s ", buf->name);
            curr_x_pos += strlen(buf->name) + 1;
        } else {
            mvwprintw(e->title_win.win, 0, curr_x_pos, "%s ", "[No Name]");
            curr_x_pos += strlen("[No Name]") + 1;
        }
        if(buf != e->buf){
            wattroff(e->title_win.win, A_BOLD);
        }
        buf = buf->next ?: e->beg;
    } while (buf != e->buf);

    // Print out the cursor position
    char cursor_info[BUFSIZ] = {};
    sprintf(cursor_info, "%lu:%lu", e->buf->row, e->buf->col);
    mvwprintw(e->title_win.win, 0, COLS - strlen(cursor_info), "%s", cursor_info);

    // Finally, fill the middle in with spaces
    size_t i;
    for(i = 0; i < COLS - strlen(cursor_info) - curr_x_pos; i++) {
        mvwprintw(e->title_win.win, 0, curr_x_pos + i, " ");
    }

    // Setup the text window
    if(e->buf->row > e->text_win.bottom) {
        e->text_win.top += e->buf->row - e->text_win.bottom;
        e->text_win.bottom = e->buf->row;
    } else if (e->buf->row < e->text_win.top) {
        e->text_win.bottom -= e->text_win.top - e->buf->row;
        e->text_win.top = e->buf->row;
    }

    // Fill the text window with text
    size_t win_row;
    line_t* w_line = e->buf->beg;

    for(win_row = 0; win_row < e->text_win.top && w_line; win_row++) {
        w_line = w_line->next;
    }

    for(win_row = 0; win_row <e->text_win.rows; win_row++) {
        wmove(e->text_win.win, win_row, 0);
        size_t col;
        for(col = 0; col < e->text_win.cols; col++) {
            if (w_line && col < strlen(w_line->str)) {
                waddch(e->text_win.win, w_line->str[col]);
            } else {
                waddch(e->text_win.win, ' ');
            }
        }
        if (w_line) {
            w_line = w_line->next;
        }
    }

    int curs_y = e->buf->row - e->text_win.top;
    int curs_x = e->buf->col;
    wmove(e->text_win.win, curs_y, curs_x);

    // Refesh the text windows and display them
    wnoutrefresh(e->title_win.win);
    wnoutrefresh(e->text_win.win);
    doupdate();
}

void editor_edit(editor_t* e) {
    while(true) {
        // Just in case there wasn't a buffer created, do so now
        if (!e->buf) {
            e->buf = buffer_create(NULL);
            e->beg = e->end = e->buf;
        }
        // Render everything
        editor_display(e);

        // Wait for a character
        int key = wgetch(e->text_win.win);

        // If the character is the "quit" key, then return (thus going
        // to the terminate function)
        if (key == e->mode.exit_k) {
            return;
        }

        int idx;
        cmd_t cmd = {};
        // Otherwise, iterate through all possible command keys, check them
        // if they match, execute it
        for(idx = 0; idx < e->mode.count; idx++) {
            cmd = e->mode.cmd[idx];
            if(cmd.key == key) {
                if (cmd.func) {
                    cmd.func(e, key);
                }
                goto done;
            }
        }

        // If nothing matched, then execute the default handler
        if(cmd.func) {
            cmd.func(e, key);
        }
        // The goto is pretty powerful here, it lets us skip over code
        // This line just makes sure the correct mode is set
done:
        set_mode(e, cmd.next_mode);
    }
}
