#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

static double get_time(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1000000000.0;
}

static void check_collisions(Game *game) {
    Player *p = &game->player;
    Level *level = &game->level;

    // Check ground collision
    int player_bottom = (int)(p->y + 1);
    int player_x = (int)p->x;

    if (player_bottom >= 0 && player_bottom < LEVEL_HEIGHT) {
        if (level_is_solid(level, player_x, player_bottom)) {
            p->y = player_bottom - 1;
            p->vel_y = 0;
            p->on_ground = true;
        } else {
            p->on_ground = false;
        }
    } else if (player_bottom >= LEVEL_HEIGHT) {
        // Fell off the bottom
        p->y = LEVEL_HEIGHT - 3;
        p->vel_y = 0;
        p->on_ground = true;
    }

    // Check ceiling collision
    int player_top = (int)p->y;
    if (player_top >= 0 && level_is_solid(level, player_x, player_top) && p->vel_y < 0) {
        p->vel_y = 0;
        p->y = player_top + 1;
    }

    // Check left/right collisions
    int player_y = (int)p->y;
    int player_left = (int)(p->x - 0.5f);
    int player_right = (int)(p->x + 0.5f);

    if (level_is_solid(level, player_left, player_y) && p->vel_x < 0) {
        p->vel_x = 0;
        p->x = player_left + 1;
    }
    if (level_is_solid(level, player_right, player_y) && p->vel_x > 0) {
        p->vel_x = 0;
        p->x = player_right - 1;
    }
}

int game_init(Game *game) {
    if (terminal_init(&game->terminal) != 0) {
        return -1;
    }

    game->screen = screen_buffer_create(game->terminal.width, game->terminal.height);
    if (!game->screen) {
        terminal_restore(&game->terminal);
        return -1;
    }

    level_init(&game->level);
    player_init(&game->player, 5.0f, 10.0f);

    game->running = true;
    game->last_time = get_time();

    return 0;
}

void game_cleanup(Game *game) {
    screen_buffer_free(game->screen);
    terminal_restore(&game->terminal);
}

void game_run(Game *game) {
    while (game->running) {
        double current_time = get_time();
        float delta_time = (float)(current_time - game->last_time);
        game->last_time = current_time;

        // Cap delta time to prevent huge jumps
        if (delta_time > 0.1f) delta_time = 0.1f;

        game_handle_input(game);
        game_update(game, delta_time);
        game_render(game);

        // Target 30 FPS
        struct timespec sleep_time = {0, 33000000}; // 33ms in nanoseconds
        nanosleep(&sleep_time, NULL);
    }
}

void game_update(Game *game, float delta_time) {
    player_update(&game->player, delta_time);
    check_collisions(game);
}

void game_render(Game *game) {
    screen_buffer_clear(game->screen);

    // Render level
    for (int y = 0; y < LEVEL_HEIGHT && y < game->screen->height; y++) {
        for (int x = 0; x < LEVEL_WIDTH && x < game->screen->width; x++) {
            TileType tile = level_get_tile(&game->level, x, y);
            if (tile != TILE_EMPTY) {
                screen_buffer_draw_char(game->screen, x, y, tile);
            }
        }
    }

    // Render player
    int px = (int)game->player.x;
    int py = (int)game->player.y;
    char player_char = game->player.facing_right ? '>' : '<';
    screen_buffer_draw_char(game->screen, px, py, player_char);

    // Render UI
    char info[64];
    snprintf(info, sizeof(info), "X:%.1f Y:%.1f VX:%.1f VY:%.1f %s",
             game->player.x, game->player.y,
             game->player.vel_x, game->player.vel_y,
             game->player.on_ground ? "GND" : "AIR");
    screen_buffer_draw_string(game->screen, 0, 0, info);
    screen_buffer_draw_string(game->screen, 0, 1, "WASD=Move SPACE=Jump Q=Quit");

    screen_buffer_render(game->screen);
}

void game_handle_input(Game *game) {
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1) {
        switch (c) {
            case 'q':
            case 'Q':
            case 27: // ESC
                game->running = false;
                break;
            case 'a':
            case 'A':
                player_move_left(&game->player);
                break;
            case 'd':
            case 'D':
                player_move_right(&game->player);
                break;
            case 'w':
            case 'W':
            case ' ':
                player_jump(&game->player);
                break;
        }
    }
}
