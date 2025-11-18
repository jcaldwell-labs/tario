#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

typedef enum {
    ANIM_IDLE,
    ANIM_WALK,
    ANIM_JUMP,
    ANIM_FALL,
    ANIM_DEATH
} AnimState;

typedef struct {
    float x;
    float y;
    float vel_x;
    float vel_y;
    bool on_ground;
    bool facing_right;
    bool jump_held;           // Is jump button currently held
    float coyote_timer;       // Time since left ground (for coyote time)
    float jump_buffer_timer;  // Time since jump was pressed (for jump buffering)
    AnimState anim_state;
    float anim_timer;
    int lives;
    bool is_dead;
    float respawn_timer;
    int coins_collected;
} Player;

// Initialize player
void player_init(Player *player, float x, float y);

// Update player physics
void player_update(Player *player, float delta_time);

// Apply movement input
void player_move_left(Player *player);
void player_move_right(Player *player);
void player_jump_press(Player *player);
void player_jump_release(Player *player);
void player_stop_horizontal(Player *player);

// Get current animation character
char player_get_sprite(Player *player);

// Handle player death
void player_kill(Player *player);
bool player_can_respawn(Player *player);

#endif
