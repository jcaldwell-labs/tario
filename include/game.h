#ifndef GAME_H
#define GAME_H

#include "level.h"
#include "player.h"
#include "render.h"
#include "terminal.h"
#include <stdbool.h>

typedef struct {
  Terminal terminal;
  ScreenBuffer *screen;
  Player player;
  Level level;
  bool running;
  double last_time;
  float camera_x;
  float camera_y;
  float spawn_x;
  float spawn_y;
  bool paused;
  bool victory;
} Game;

// Initialize game
int game_init(Game *game);

// Cleanup game resources
void game_cleanup(Game *game);

// Main game loop
void game_run(Game *game);

// Update game state
void game_update(Game *game, float delta_time);

// Render game
void game_render(Game *game);

// Handle input
void game_handle_input(Game *game);

#endif
