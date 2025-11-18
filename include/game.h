#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include "terminal.h"
#include "render.h"
#include "player.h"
#include "level.h"

typedef struct {
    Terminal terminal;
    ScreenBuffer *screen;
    Player player;
    Level level;
    bool running;
    double last_time;
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
