#include "terminal.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

int terminal_init(Terminal *term) {
    // Get original terminal settings
    if (tcgetattr(STDIN_FILENO, &term->orig_termios) == -1) {
        perror("tcgetattr");
        return -1;
    }

    // Set up raw mode
    struct termios raw = term->orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        perror("tcsetattr");
        return -1;
    }

    // Get terminal size
    terminal_get_size(term);

    // Hide cursor and clear screen
    term_hide_cursor();
    term_clear_screen();

    return 0;
}

void terminal_restore(Terminal *term) {
    term_show_cursor();
    term_clear_screen();
    term_move_cursor(0, 0);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &term->orig_termios);
}

void terminal_get_size(Terminal *term) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        term->width = 80;
        term->height = 24;
    } else {
        term->width = ws.ws_col;
        term->height = ws.ws_row;
    }
}

void term_clear_screen(void) {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
}

void term_move_cursor(int x, int y) {
    char buf[32];
    int len = snprintf(buf, sizeof(buf), "\x1b[%d;%dH", y + 1, x + 1);
    write(STDOUT_FILENO, buf, len);
}

void term_hide_cursor(void) {
    write(STDOUT_FILENO, "\x1b[?25l", 6);
}

void term_show_cursor(void) {
    write(STDOUT_FILENO, "\x1b[?25h", 6);
}

void term_set_color(int fg, int bg) {
    char buf[32];
    int len = snprintf(buf, sizeof(buf), "\x1b[%d;%dm", fg + 30, bg + 40);
    write(STDOUT_FILENO, buf, len);
}

void term_reset_color(void) {
    write(STDOUT_FILENO, "\x1b[0m", 4);
}
