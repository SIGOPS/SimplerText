#include "buffer.h"

buffer_t* buffer_create(const char *name) {
	buffer_t* buf = calloc(1, sizeof(buffer_t));
	if (name) {
		// Duplicates a string
		buf->name = strdup(name);
	}
	// It's fine if name is NULL, our read function will handle it
	buffer_read(buf, name);
	return buf;
}

void buffer_free(buffer_t* buf) {
	if(!buf) {
		return;
	}
	// To free a buffer, first clear it
	buffer_clear(buf);
	// Then just free the other parts of it
	buf->next = buf->prev = NULL;
	free(buf->name);
	free(buf);
}

int buffer_read(buffer_t* buf, const char* name) {
	// Since we are reading a new file, clear the current info
	// in the buffer we are viewing
	buffer_clear(buf);
	FILE *fp;

	// If a name was passed in, use that, otherwise, use what is 
	// provided in the buffer (useful for reloading a file)
	if(name) {
		fp = fopen(name, "r");
	} else {
		fp = fopen(buf->name, "r");
	}

	// How are we handling the NULL case? Simple, try to open a NULL
	// file: you can't! That mean fp would be NULL, so we do a simple
	// check
	if(fp) {
		char in[BUFSIZ];
		// fgets() will help us read the file line by line
		while(fgets(in, BUFSIZ, fp) != NULL) {
			line_t* l = line_create(in, strlen(in) - 1);
			buffer_append(buf, l);
		}
		// We can do this becuase append will also set the beginning of 
		// the buffer for us
		buf->line = buf->beg;
		fclose(fp);

		// We read everything successfully
		return 1;
	} else {
		// If we couldn't open the file, just create a empty buffer
		// We also we append the line and set it as the current line.
		line_t* l = line_create(NULL, 0);
		buffer_append(buf, l);
		buf->line = l;
	}

	// 2 corresponds to "ENOENT" or Error NO ENTity (basically, file
	// does not exist)
	return 0;
}

int buffer_write(buffer_t* buf, const char* name) {
	FILE *fp;
	if (name) {
		fp = fopen(name, "w");
	} else {
		fp = fopen(buf->name, "w");
	}

	if(fp) {
		for(line_t* l = buf->beg; l != NULL; l = l->next) {
			// fprintf() allows us to "print" directly to a file
			fprintf(fp, "%s\n", l->str);
		}
		fclose(fp);
		return 1;
	}
	return 0;
}

void buffer_append(buffer_t* buf, line_t* line) {
	// Does a buffer end exist? It's a simple check to see if
	// a buffer has been initalized
	if(buf->end) {
		// Basic linked list operations
		line->prev = buf->end;
		line->prev->next = line;
		buf->end = line;
	} else {
		// Otherwise init buffer
		buf->beg = buf->end = line;
	}
	buf->size++;
}

void buffer_prepend(buffer_t* buf, line_t* line) {
	// Basically the same as appending, but at the front of the 
	// linked list
	if(buf->beg) {
		line->next = buf->beg;
		line->next->prev = line;
		buf->beg = line;
	} else {
		buf->beg = buf->end = line;
	}
	buf->size++;
}

void buffer_insert(buffer_t* buf, line_t* dest, line_t* line) {
	// Again this is just a regular linked list operation
	line->next = dest;
	line->prev = dest->prev;
	if (line->next) {
		line->next->prev = line;
	}
	if (line->prev) {
		line->prev->next = line;
	}
	buf->size++;
}

void buffer_erase(buffer_t* buf, line_t* line) {
	// Check for edge cases
	if(line == buf->beg) {
		buf->beg = line->next;
	} else {
		line->prev->next = line->next;
	}

	//... and the end edge case
	if(line == buf->end) {
		buf->end = line->prev;
	} else {
		line->next->prev = line->prev;
	}
	buf->size--;
	line_free(line);
}

void buffer_clear(buffer_t* buf) {
	// Just go through every line and free it.
	if(!buf) {
		return;
	}
	line_t* l = buf->beg;
	while(l) {
		line_t* next_l = l->next;
		line_free(l);
		l = next_l;
	}

	// Finally clear struct values
	buf->size = 0;
	buf->row = buf->col = 0;
	buf->line = buf->beg = buf->end = NULL;
}