#ifndef RENDER_H
#define RENDER_H

#include "terminal.h"

// Screen buffer for double buffering
typedef struct {
  char *buffer;
  int width;
  int height;
} ScreenBuffer;

// Create a new screen buffer
ScreenBuffer *screen_buffer_create(int width, int height);

// Free screen buffer
void screen_buffer_free(ScreenBuffer *sb);

// Clear the buffer
void screen_buffer_clear(ScreenBuffer *sb);

// Draw a character at position
void screen_buffer_draw_char(ScreenBuffer *sb, int x, int y, char c);

// Draw a string at position
void screen_buffer_draw_string(ScreenBuffer *sb, int x, int y, const char *str);

// Render buffer to screen
void screen_buffer_render(ScreenBuffer *sb);

#endif
