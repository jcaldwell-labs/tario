#include "render.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

ScreenBuffer *screen_buffer_create(int width, int height) {
  ScreenBuffer *sb = malloc(sizeof(ScreenBuffer));
  if (!sb)
    return NULL;

  sb->width = width;
  sb->height = height;
  sb->buffer = malloc(width * height);

  if (!sb->buffer) {
    free(sb);
    return NULL;
  }

  screen_buffer_clear(sb);
  return sb;
}

void screen_buffer_free(ScreenBuffer *sb) {
  if (sb) {
    free(sb->buffer);
    free(sb);
  }
}

void screen_buffer_clear(ScreenBuffer *sb) {
  memset(sb->buffer, ' ', sb->width * sb->height);
}

void screen_buffer_draw_char(ScreenBuffer *sb, int x, int y, char c) {
  if (x >= 0 && x < sb->width && y >= 0 && y < sb->height) {
    sb->buffer[y * sb->width + x] = c;
  }
}

void screen_buffer_draw_string(ScreenBuffer *sb, int x, int y,
                               const char *str) {
  int i = 0;
  while (str[i] != '\0' && x + i < sb->width) {
    screen_buffer_draw_char(sb, x + i, y, str[i]);
    i++;
  }
}

void screen_buffer_render(ScreenBuffer *sb) {
  // Move cursor to home position
  term_move_cursor(0, 0);

  // Render buffer line by line
  for (int y = 0; y < sb->height; y++) {
    write(STDOUT_FILENO, &sb->buffer[y * sb->width], sb->width);
    if (y < sb->height - 1) {
      write(STDOUT_FILENO, "\r\n", 2);
    }
  }
}
