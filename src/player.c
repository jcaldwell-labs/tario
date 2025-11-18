#include "player.h"

#define GRAVITY 25.0f
#define MOVE_SPEED 15.0f
#define JUMP_FORCE 12.0f
#define MAX_FALL_SPEED 20.0f
#define FRICTION 0.8f

void player_init(Player *player, float x, float y) {
    player->x = x;
    player->y = y;
    player->vel_x = 0.0f;
    player->vel_y = 0.0f;
    player->on_ground = false;
    player->facing_right = true;
}

void player_update(Player *player, float delta_time) {
    // Apply gravity
    player->vel_y += GRAVITY * delta_time;

    // Cap fall speed
    if (player->vel_y > MAX_FALL_SPEED) {
        player->vel_y = MAX_FALL_SPEED;
    }

    // Apply friction when on ground
    if (player->on_ground) {
        player->vel_x *= FRICTION;
        if (player->vel_x > -0.1f && player->vel_x < 0.1f) {
            player->vel_x = 0.0f;
        }
    }

    // Update position
    player->x += player->vel_x * delta_time;
    player->y += player->vel_y * delta_time;

    // Keep player in bounds
    if (player->x < 0) player->x = 0;
    if (player->x >= 79) player->x = 79;
}

void player_move_left(Player *player) {
    player->vel_x = -MOVE_SPEED;
    player->facing_right = false;
}

void player_move_right(Player *player) {
    player->vel_x = MOVE_SPEED;
    player->facing_right = true;
}

void player_jump(Player *player) {
    if (player->on_ground) {
        player->vel_y = -JUMP_FORCE;
        player->on_ground = false;
    }
}

void player_stop_horizontal(Player *player) {
    player->vel_x = 0.0f;
}
