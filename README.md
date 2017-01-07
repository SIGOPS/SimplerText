# SimplerText

A simple text editor designed to show the working of the C programming language

![Pretty!](http://i.imgur.com/yEjJY1h.png)

Designed to emulate Vim, the text editor uses a linked-list based design to operate.

Highly extensible. Press `i` to start editing!

##Usage:

`./simplertext file1 file2 ...` to run.

##Keybindings
###Shortcuts
- <kbd>CTRL</kbd> + <kbd>s</kbd> to save
- <kbd>CTRL</kbd> + <kbd>o</kbd> to open in a new buffer
- <kbd>CTRL</kbd> + <kbd>n</kbd> to create a new blank buffer
- <kbd>CTRL</kbd> + <kbd>w</kbd> to close a buffer
- <kbd>CTRL</kbd> + <kbd>t</kbd> to move to next buffer
- <kbd>CTRL</kbd> + <kbd>T</kbd> to move to previous buffer

###Normal Mode keybinds
- <kbd>i</kbd> to enter insert mode
- <kbd>g</kbd>/<kbd>G</kbd> to move to beginning/end of buffer
- Arrow keys to move around
- <kbd>w</kbd> to move to the next word
- <kbd>b</kbd> to move to the previous word
- <kbd>x</kbd> to delete a character
- <kbd>:</kbd> to insert a commnad

###Insert Mode keybinds
- <kbd>CTRL</kbd> + <kbd>s</kbd> to save

###Command bar commands
- `w` to write file
- `q` to close buffer (will quit if last buffer)
- `qa` to close all buffers and quit
- `edit filename` to open a file in the same buffer
- `open filename` to open a file in a new buffer