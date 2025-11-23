#include "player.h"
#include <math.h>

#define GRAVITY 25.0f
#define MOVE_SPEED 15.0f
#define JUMP_FORCE 15.0f
#define JUMP_CUT_MULTIPLIER                                                    \
  0.5f // Multiply velocity by this when jump released early
#define MAX_FALL_SPEED 20.0f
#define FRICTION 0.85f
#define COYOTE_TIME 0.15f     // Can jump this long after leaving ground
#define JUMP_BUFFER_TIME 0.1f // Can press jump this early before landing

void player_init(Player *player, float x, float y) {
  player->x = x;
  player->y = y;
  player->vel_x = 0.0f;
  player->vel_y = 0.0f;
  player->on_ground = false;
  player->facing_right = true;
  player->jump_held = false;
  player->coyote_timer = 0.0f;
  player->jump_buffer_timer = 0.0f;
  player->anim_state = ANIM_IDLE;
  player->anim_timer = 0.0f;
  player->lives = 3;
  player->is_dead = false;
  player->respawn_timer = 0.0f;
  player->coins_collected = 0;
}

void player_update(Player *player, float delta_time) {
  // Handle death state
  if (player->is_dead) {
    player->respawn_timer += delta_time;
    player->vel_x = 0.0f;
    player->vel_y += GRAVITY * delta_time * 0.5f; // Slower fall when dead
    player->y += player->vel_y * delta_time;
    return;
  }

  // Update timers
  player->coyote_timer += delta_time;
  if (player->jump_buffer_timer > 0.0f) {
    player->jump_buffer_timer -= delta_time;
  }
  player->anim_timer += delta_time;

  // Apply gravity
  player->vel_y += GRAVITY * delta_time;

  // Cap fall speed
  if (player->vel_y > MAX_FALL_SPEED) {
    player->vel_y = MAX_FALL_SPEED;
  }

  // Apply friction when on ground
  if (player->on_ground) {
    player->vel_x *= FRICTION;
    if (fabsf(player->vel_x) < 0.1f) {
      player->vel_x = 0.0f;
    }
  }

  // Update position
  player->x += player->vel_x * delta_time;
  player->y += player->vel_y * delta_time;

  // Update animation state
  if (player->on_ground) {
    if (fabsf(player->vel_x) > 0.5f) {
      player->anim_state = ANIM_WALK;
    } else {
      player->anim_state = ANIM_IDLE;
    }
  } else {
    if (player->vel_y < 0.0f) {
      player->anim_state = ANIM_JUMP;
    } else {
      player->anim_state = ANIM_FALL;
    }
  }

  // Keep player in horizontal bounds (will be removed when camera scrolling is
  // added)
  if (player->x < 0)
    player->x = 0;
}

void player_move_left(Player *player) {
  if (!player->is_dead) {
    player->vel_x = -MOVE_SPEED;
    player->facing_right = false;
  }
}

void player_move_right(Player *player) {
  if (!player->is_dead) {
    player->vel_x = MOVE_SPEED;
    player->facing_right = true;
  }
}

void player_jump_press(Player *player) {
  if (player->is_dead)
    return;

  player->jump_held = true;
  player->jump_buffer_timer = JUMP_BUFFER_TIME;

  // Can jump if:
  // 1. On ground
  // 2. Within coyote time (just left ground)
  // This check will also be called from game logic when landing (for jump
  // buffering)
  if (player->on_ground || player->coyote_timer < COYOTE_TIME) {
    player->vel_y = -JUMP_FORCE;
    player->on_ground = false;
    player->coyote_timer = COYOTE_TIME; // Prevent double jump
  }
}

void player_jump_release(Player *player) {
  player->jump_held = false;

  // Variable jump height: cut jump short if released early
  if (player->vel_y < 0.0f) {
    player->vel_y *= JUMP_CUT_MULTIPLIER;
  }
}

void player_stop_horizontal(Player *player) { player->vel_x = 0.0f; }

char player_get_sprite(Player *player) {
  if (player->is_dead) {
    // Death animation cycles through characters
    int frame = (int)(player->anim_timer * 10.0f) % 4;
    const char death_anim[] = "xX*+";
    return death_anim[frame];
  }

  switch (player->anim_state) {
  case ANIM_WALK:
    // Walking animation
    {
      int frame = (int)(player->anim_timer * 8.0f) % 2;
      if (player->facing_right) {
        return frame ? '>' : ')';
      } else {
        return frame ? '<' : '(';
      }
    }
  case ANIM_JUMP:
    return player->facing_right ? '^' : '^';
  case ANIM_FALL:
    return player->facing_right ? 'v' : 'v';
  case ANIM_IDLE:
  default:
    return player->facing_right ? '>' : '<';
  }
}

void player_kill(Player *player) {
  if (!player->is_dead) {
    player->is_dead = true;
    player->lives--;
    player->respawn_timer = 0.0f;
    player->anim_state = ANIM_DEATH;
    player->anim_timer = 0.0f;
    player->vel_y = -8.0f; // Small bounce on death
  }
}

bool player_can_respawn(Player *player) {
  return player->is_dead && player->respawn_timer >= 2.0f;
}
