#ifndef TERMINAL_H
#define TERMINAL_H

#include <termios.h>

// Terminal state
typedef struct {
  struct termios orig_termios;
  int width;
  int height;
} Terminal;

// Initialize terminal in raw mode
int terminal_init(Terminal *term);

// Restore terminal to original state
void terminal_restore(Terminal *term);

// Get terminal size
void terminal_get_size(Terminal *term);

// ANSI escape code functions
void term_clear_screen(void);
void term_move_cursor(int x, int y);
void term_hide_cursor(void);
void term_show_cursor(void);
void term_set_color(int fg, int bg);
void term_reset_color(void);

#endif
