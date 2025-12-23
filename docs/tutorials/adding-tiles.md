# Tutorial: Adding New Tile Types

This tutorial walks you through adding a new tile type to Tario.

## Overview

We'll add a "Bounce Pad" tile that launches the player into the air when touched.

## Step 1: Define the Tile Type

In `include/level.h`, add your new tile to the `TileType` enum:

```c
typedef enum {
    TILE_EMPTY,
    TILE_GROUND,
    TILE_BRICK,
    TILE_PLATFORM,
    TILE_SPIKE,
    TILE_COIN,
    TILE_GOAL,
    TILE_PIPE_LEFT,
    TILE_PIPE_RIGHT,
    TILE_QUESTION,
    TILE_BOUNCE,  // Add this line
} TileType;
```

## Step 2: Define Tile Behavior

In `src/level.c`, update the collision query functions:

```c
// Is the tile solid (blocks movement)?
bool level_is_solid(Level *level, int x, int y) {
    TileType tile = level_get_tile(level, x, y);
    switch (tile) {
        case TILE_GROUND:
        case TILE_BRICK:
        case TILE_PIPE_LEFT:
        case TILE_PIPE_RIGHT:
        case TILE_QUESTION:
        case TILE_BOUNCE:  // Bounce pads are solid
            return true;
        default:
            return false;
    }
}

// Add a new query function for bounce tiles
bool level_is_bounce(Level *level, int x, int y) {
    return level_get_tile(level, x, y) == TILE_BOUNCE;
}
```

Don't forget to add the declaration in `include/level.h`:

```c
bool level_is_bounce(Level *level, int x, int y);
```

## Step 3: Handle Tile Interaction

In `src/player.c`, check for bounce pad collision during physics update:

```c
void player_update(Player *player, Level *level, float dt) {
    // ... existing physics code ...

    // After resolving ground collision, check for bounce
    if (player->grounded) {
        int tile_x = (int)(player->x + player->width / 2);
        int tile_y = (int)(player->y + player->height);

        if (level_is_bounce(level, tile_x, tile_y)) {
            // Super jump!
            player->vel_y = -JUMP_FORCE * 1.5f;
            player->grounded = false;
        }
    }
}
```

## Step 4: Render the Tile

In `src/game.c`, update the render function to draw your tile:

```c
void game_render(Game *game) {
    // ... viewport calculation ...

    for (int y = start_y; y < end_y; y++) {
        for (int x = start_x; x < end_x; x++) {
            TileType tile = level_get_tile(&game->level, x, y);
            char ch = ' ';

            switch (tile) {
                case TILE_GROUND:   ch = '#'; break;
                case TILE_BRICK:    ch = '='; break;
                case TILE_PLATFORM: ch = '-'; break;
                case TILE_SPIKE:    ch = '^'; break;
                case TILE_COIN:     ch = 'o'; break;
                case TILE_GOAL:     ch = 'F'; break;
                case TILE_BOUNCE:   ch = 'B'; break;  // Add this
                // ... other tiles ...
            }

            screen_buffer_set(game->buffer, screen_x, screen_y, ch);
        }
    }
}
```

## Step 5: Place Tiles in the Level

In `src/level.c`, add bounce pads to the level layout:

```c
void level_init(Level *level) {
    // ... existing level code ...

    // Add some bounce pads
    level->tiles[42][50] = TILE_BOUNCE;
    level->tiles[42][51] = TILE_BOUNCE;
}
```

## Step 6: Build and Test

```bash
make clean
make
./tario
```

Navigate to your bounce pads and test:
- Does the player bounce higher than a normal jump?
- Does collision detection work properly?
- Does the tile render correctly?

## Complete Example

Here's the complete diff for adding a bounce pad:

### include/level.h
```diff
 typedef enum {
     TILE_EMPTY,
     TILE_GROUND,
     // ... existing tiles ...
+    TILE_BOUNCE,
 } TileType;

+bool level_is_bounce(Level *level, int x, int y);
```

### src/level.c
```diff
 bool level_is_solid(Level *level, int x, int y) {
     TileType tile = level_get_tile(level, x, y);
     switch (tile) {
         case TILE_GROUND:
         case TILE_BRICK:
+        case TILE_BOUNCE:
             return true;
         // ...
     }
 }

+bool level_is_bounce(Level *level, int x, int y) {
+    return level_get_tile(level, x, y) == TILE_BOUNCE;
+}
```

### src/player.c
```diff
 void player_update(Player *player, Level *level, float dt) {
     // ... after ground collision ...

+    if (player->grounded) {
+        int tx = (int)(player->x + player->width / 2);
+        int ty = (int)(player->y + player->height);
+        if (level_is_bounce(level, tx, ty)) {
+            player->vel_y = -JUMP_FORCE * 1.5f;
+            player->grounded = false;
+        }
+    }
 }
```

### src/game.c
```diff
 case TILE_GOAL:     ch = 'F'; break;
+case TILE_BOUNCE:   ch = 'B'; break;
```

## Tips for New Tiles

### Choosing Properties

| Tile Type | Solid | Deadly | Platform | Collectible |
|-----------|-------|--------|----------|-------------|
| Ground | Yes | No | No | No |
| Spike | No | Yes | No | No |
| Platform | Special | No | Yes | No |
| Coin | No | No | No | Yes |
| Bounce | Yes | No | No | No |

### Visual Design

Choose ASCII characters that convey meaning:
- `B` for bounce (or `^` facing up)
- `~` for water
- `*` for something special
- `!` for danger

### Testing Checklist

- [ ] Tile appears in level
- [ ] Collision works correctly
- [ ] Interaction triggers as expected
- [ ] No memory leaks
- [ ] Compiles without warnings

## Next Steps

- Add animation to your tile
- Create tile combinations (e.g., bounce + spike)
- Implement tile state (e.g., activated/deactivated)
