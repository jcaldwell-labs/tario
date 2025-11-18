#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

static double get_time(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1000000000.0;
}

static void update_camera(Game *game) {
    // Camera follows player horizontally
    int viewport_width = game->screen->width;
    int viewport_height = game->screen->height - 2; // Leave room for HUD

    // Center camera on player, but clamp to level bounds
    game->camera_x = game->player.x - viewport_width / 2;
    game->camera_y = game->player.y - viewport_height / 2;

    // Clamp camera to level bounds
    if (game->camera_x < 0) game->camera_x = 0;
    if (game->camera_y < 0) game->camera_y = 0;
    if (game->camera_x > LEVEL_WIDTH - viewport_width) {
        game->camera_x = LEVEL_WIDTH - viewport_width;
    }
    if (game->camera_y > LEVEL_HEIGHT - viewport_height) {
        game->camera_y = LEVEL_HEIGHT - viewport_height;
    }
}

static void check_collisions(Game *game) {
    Player *p = &game->player;
    Level *level = &game->level;

    if (p->is_dead) return;

    // Store previous ground state for coyote time
    bool was_on_ground = p->on_ground;
    p->on_ground = false;

    // Check ground collision (including one-way platforms)
    int player_bottom = (int)(p->y + 1);
    int player_x = (int)p->x;

    // Check solid tiles
    if (player_bottom >= 0 && player_bottom < LEVEL_HEIGHT) {
        if (level_is_solid(level, player_x, player_bottom) && p->vel_y >= 0) {
            p->y = player_bottom - 1;
            p->vel_y = 0;
            p->on_ground = true;
        }
    }

    // Check one-way platforms (can only land on from above)
    if (!p->on_ground && p->vel_y >= 0) {
        int check_y = (int)(p->y + 0.5f);
        if (level_is_platform(level, player_x, check_y + 1) && p->y < check_y + 1) {
            p->y = check_y;
            p->vel_y = 0;
            p->on_ground = true;
        }
    }

    // Reset coyote timer when landing
    if (p->on_ground && !was_on_ground) {
        p->coyote_timer = 0.0f;

        // Check for jump buffering (player pressed jump before landing)
        if (p->jump_buffer_timer > 0.0f) {
            player_jump_press(p);
        }
    }

    // If player just left ground, reset coyote timer
    if (was_on_ground && !p->on_ground) {
        p->coyote_timer = 0.0f;
    }

    // Fall off bottom of level = death
    if (player_bottom >= LEVEL_HEIGHT) {
        player_kill(p);
        return;
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
        p->x = player_left + 1.5f;
    }
    if (level_is_solid(level, player_right, player_y) && p->vel_x > 0) {
        p->vel_x = 0;
        p->x = player_right - 0.5f;
    }

    // Check for spikes
    if (level_is_deadly(level, player_x, player_y) ||
        level_is_deadly(level, player_x, player_bottom)) {
        player_kill(p);
        return;
    }

    // Check for goal
    if (level_is_goal(level, player_x, player_y) ||
        level_is_goal(level, player_x, player_top)) {
        game->victory = true;
    }

    // Collect coins
    if (level_is_coin(level, player_x, player_y)) {
        level_collect_coin(level, player_x, player_y);
        p->coins_collected++;
    }
    if (level_is_coin(level, player_x, player_top)) {
        level_collect_coin(level, player_x, player_top);
        p->coins_collected++;
    }
}

static void respawn_player(Game *game) {
    int saved_lives = game->player.lives;
    int saved_coins = game->player.coins_collected;
    player_init(&game->player, game->spawn_x, game->spawn_y);
    game->player.lives = saved_lives;
    game->player.coins_collected = saved_coins;
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

    // Spawn player at start
    game->spawn_x = 5.0f;
    game->spawn_y = LEVEL_HEIGHT - 10.0f;
    player_init(&game->player, game->spawn_x, game->spawn_y);

    game->running = true;
    game->last_time = get_time();
    game->camera_x = 0;
    game->camera_y = 0;
    game->paused = false;
    game->victory = false;

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
        if (!game->paused) {
            game_update(game, delta_time);
        }
        game_render(game);

        // Target 60 FPS
        struct timespec sleep_time = {0, 16666667}; // ~16.67ms in nanoseconds
        nanosleep(&sleep_time, NULL);
    }
}

void game_update(Game *game, float delta_time) {
    // Check for respawn
    if (player_can_respawn(&game->player)) {
        if (game->player.lives > 0) {
            respawn_player(game);
        } else {
            // Game over - could add game over screen here
            game->running = false;
        }
    }

    player_update(&game->player, delta_time);
    check_collisions(game);
    update_camera(game);
}

void game_render(Game *game) {
    screen_buffer_clear(game->screen);

    int cam_x = (int)game->camera_x;
    int cam_y = (int)game->camera_y;
    int viewport_height = game->screen->height - 2; // Reserve bottom for HUD

    // Render level
    for (int y = 0; y < viewport_height && y + cam_y < LEVEL_HEIGHT; y++) {
        for (int x = 0; x < game->screen->width && x + cam_x < LEVEL_WIDTH; x++) {
            int world_x = x + cam_x;
            int world_y = y + cam_y;
            TileType tile = level_get_tile(&game->level, world_x, world_y);
            if (tile != TILE_EMPTY) {
                screen_buffer_draw_char(game->screen, x, y, tile);
            }
        }
    }

    // Render player
    int px = (int)game->player.x - cam_x;
    int py = (int)game->player.y - cam_y;
    if (px >= 0 && px < game->screen->width && py >= 0 && py < viewport_height) {
        char player_char = player_get_sprite(&game->player);
        screen_buffer_draw_char(game->screen, px, py, player_char);
    }

    // Render HUD at bottom
    int hud_y = game->screen->height - 2;
    char hud[128];
    snprintf(hud, sizeof(hud), "Lives: %d  Coins: %d  Pos: %.0f,%.0f",
             game->player.lives, game->player.coins_collected,
             game->player.x, game->player.y);
    screen_buffer_draw_string(game->screen, 0, hud_y, hud);

    char controls[] = "WASD/Arrows=Move SPACE=Jump P=Pause Q=Quit";
    screen_buffer_draw_string(game->screen, 0, hud_y + 1, controls);

    // Show victory message
    if (game->victory) {
        int msg_y = game->screen->height / 2;
        char victory_msg[] = "*** VICTORY! Press Q to quit ***";
        int msg_x = (game->screen->width - strlen(victory_msg)) / 2;
        screen_buffer_draw_string(game->screen, msg_x, msg_y, victory_msg);
    }

    // Show pause message
    if (game->paused) {
        int msg_y = game->screen->height / 2;
        char pause_msg[] = "*** PAUSED - Press P to resume ***";
        int msg_x = (game->screen->width - strlen(pause_msg)) / 2;
        screen_buffer_draw_string(game->screen, msg_x, msg_y, pause_msg);
    }

    // Show death message
    if (game->player.is_dead) {
        int msg_y = game->screen->height / 2 - 1;
        char death_msg[] = "*** OUCH! ***";
        int msg_x = (game->screen->width - strlen(death_msg)) / 2;
        screen_buffer_draw_string(game->screen, msg_x, msg_y, death_msg);
    }

    screen_buffer_render(game->screen);
}

void game_handle_input(Game *game) {
    char c;
    static bool space_held = false;
    static bool up_held = false;

    while (read(STDIN_FILENO, &c, 1) == 1) {
        // Quit
        if (c == 'q' || c == 'Q' || c == 27) { // ESC
            game->running = false;
            return;
        }

        // Pause
        if (c == 'p' || c == 'P') {
            game->paused = !game->paused;
        }

        if (game->paused) continue;

        // Handle arrow keys (they come as escape sequences)
        if (c == '\x1b') {
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) != 1) continue;
            if (read(STDIN_FILENO, &seq[1], 1) != 1) continue;

            if (seq[0] == '[') {
                switch (seq[1]) {
                    case 'A': // Up arrow
                        if (!up_held) {
                            player_jump_press(&game->player);
                            up_held = true;
                        }
                        break;
                    case 'B': // Down arrow
                        break;
                    case 'C': // Right arrow
                        player_move_right(&game->player);
                        break;
                    case 'D': // Left arrow
                        player_move_left(&game->player);
                        break;
                }
            }
        } else {
            // Regular keys
            switch (c) {
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
                    if (!up_held) {
                        player_jump_press(&game->player);
                        up_held = true;
                    }
                    break;
                case ' ':
                    if (!space_held) {
                        player_jump_press(&game->player);
                        space_held = true;
                    }
                    break;
            }
        }
    }

    // Note: We can't detect key release in this simple input system,
    // so variable jump height works by cutting the jump when space is no longer held
    // In this implementation, we'll rely on the JUMP_CUT_MULTIPLIER applied immediately
    // This is a limitation of non-blocking terminal input without a proper input library
}
