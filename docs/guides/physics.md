# Physics Guide

This guide explains Tario's physics simulation system.

## Overview

Tario uses a simple but effective 2D physics system with:
- Gravity-based vertical movement
- Velocity and friction for horizontal movement
- AABB collision detection
- One-way platform support
- Advanced jump mechanics

## Physics Constants

```c
#define GRAVITY         25.0f   // units/s² - downward acceleration
#define JUMP_FORCE      15.0f   // units/s - upward velocity on jump
#define MAX_FALL_SPEED  20.0f   // units/s - terminal velocity
#define MOVE_SPEED      15.0f   // units/s - horizontal movement
#define FRICTION        0.85f   // multiplier when grounded
#define COYOTE_TIME     0.15f   // seconds after leaving platform
#define JUMP_BUFFER     0.10f   // seconds before landing
```

## Physics Update Loop

Each frame, physics updates in this order:

```c
void player_update(Player *p, Level *level, float dt) {
    // 1. Apply gravity
    p->vel_y += GRAVITY * dt;
    if (p->vel_y > MAX_FALL_SPEED) {
        p->vel_y = MAX_FALL_SPEED;
    }

    // 2. Apply friction (when grounded)
    if (p->grounded) {
        p->vel_x *= FRICTION;
    }

    // 3. Update timers
    if (p->coyote_timer > 0) p->coyote_timer -= dt;
    if (p->jump_buffer > 0) p->jump_buffer -= dt;

    // 4. Apply velocity to position
    p->x += p->vel_x * dt;
    p->y += p->vel_y * dt;

    // 5. Collision detection and response
    resolve_collisions(p, level);

    // 6. Update grounded state
    update_grounded_state(p, level);
}
```

## Collision Detection

### AABB (Axis-Aligned Bounding Box)

Tario uses AABB collision with tile-based levels:

```c
bool check_collision(float px, float py, float pw, float ph,
                     float tx, float ty, float tw, float th) {
    return px < tx + tw &&
           px + pw > tx &&
           py < ty + th &&
           py + ph > ty;
}
```

### Multi-point Sampling

For accurate collision, we check multiple points:

```
┌─────┐
│1   2│  1,2 = Top corners
│     │
│3   4│  3,4 = Bottom corners
└─────┘
```

### Collision Resolution

When collision detected:

1. **Horizontal collision**: Push out of tile horizontally, zero x velocity
2. **Vertical collision**: Push out of tile vertically, zero y velocity
3. **Landing on ground**: Set `grounded = true`, reset coyote time

```c
void resolve_vertical(Player *p, Level *level) {
    // Check tiles at player's feet
    if (is_solid_at(level, p->x, p->y + p->height)) {
        // Push up out of ground
        p->y = floor(p->y + p->height) - p->height;
        p->vel_y = 0;
        p->grounded = true;
        p->coyote_timer = COYOTE_TIME;
    }
}
```

## Jump Mechanics

### Variable Jump Height

Jump height depends on how long the jump button is held:

```c
void player_jump_press(Player *p) {
    if (p->grounded || p->coyote_timer > 0) {
        p->vel_y = -JUMP_FORCE;  // Negative = upward
        p->grounded = false;
        p->coyote_timer = 0;
    } else {
        // Buffer the jump for when we land
        p->jump_buffer = JUMP_BUFFER;
    }
}

void player_jump_release(Player *p) {
    // Cut jump short when button released
    if (p->vel_y < 0) {
        p->vel_y *= 0.5;  // Reduce upward velocity
    }
}
```

### Coyote Time

Allows jumping shortly after walking off a ledge:

```c
// When leaving ground
if (was_grounded && !p->grounded) {
    p->coyote_timer = COYOTE_TIME;  // Start timer
}

// When trying to jump
if (p->coyote_timer > 0) {
    // Can still jump!
    p->vel_y = -JUMP_FORCE;
    p->coyote_timer = 0;
}
```

### Jump Buffering

Stores jump input before landing:

```c
// When jump pressed in air
if (!p->grounded) {
    p->jump_buffer = JUMP_BUFFER;  // Remember intent
}

// When landing
if (becomes_grounded && p->jump_buffer > 0) {
    // Execute buffered jump
    p->vel_y = -JUMP_FORCE;
    p->jump_buffer = 0;
}
```

## One-Way Platforms

Platforms you can jump through from below:

```c
bool should_collide_platform(Player *p, int tile_y) {
    // Only collide when:
    // 1. Moving downward (falling)
    // 2. Player's feet are above or at platform level
    return p->vel_y > 0 &&
           p->y + p->height <= tile_y;
}
```

## Frame Independence

All physics use delta time for consistent behavior:

```c
// Position = velocity * time
p->x += p->vel_x * dt;
p->y += p->vel_y * dt;

// Velocity = acceleration * time
p->vel_y += GRAVITY * dt;
```

### Delta Time Capping

Prevent physics explosions from large time steps:

```c
if (dt > 0.1f) {
    dt = 0.1f;  // Cap at 100ms
}
```

## Debugging Physics

### Common Issues

**Player falls through floor:**
- Check collision sampling points
- Verify delta time isn't too large
- Ensure collision resolution order is correct

**Jump feels wrong:**
- Adjust GRAVITY and JUMP_FORCE ratio
- Check coyote time and jump buffer values
- Verify variable jump height is working

**Sliding on slopes:**
- Tario doesn't have slopes, but horizontal friction helps

### Debug Output

Enable DEBUG mode to print physics state:

```c
#ifdef DEBUG
printf("pos: (%.2f, %.2f) vel: (%.2f, %.2f) grounded: %d\n",
       p->x, p->y, p->vel_x, p->vel_y, p->grounded);
#endif
```

## Tuning Guide

| Feel | Adjust |
|------|--------|
| Floaty jumps | Increase GRAVITY |
| Heavy feel | Decrease JUMP_FORCE |
| Slippery | Decrease FRICTION (closer to 1.0) |
| More control | Increase FRICTION (closer to 0.5) |
| Forgiving jumps | Increase COYOTE_TIME |
| Responsive jumps | Increase JUMP_BUFFER |
