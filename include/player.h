#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

typedef struct {
    float x;
    float y;
    float vel_x;
    float vel_y;
    bool on_ground;
    bool facing_right;
} Player;

// Initialize player
void player_init(Player *player, float x, float y);

// Update player physics
void player_update(Player *player, float delta_time);

// Apply movement input
void player_move_left(Player *player);
void player_move_right(Player *player);
void player_jump(Player *player);
void player_stop_horizontal(Player *player);

#endif
