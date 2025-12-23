# Example: Creating Custom Levels

This example shows how to create custom levels for Tario.

## Basic Level Creation

Levels are currently defined in `src/level.c`. Here's how to create a simple custom level:

```c
void level_init(Level *level) {
    // Clear all tiles
    for (int y = 0; y < LEVEL_HEIGHT; y++) {
        for (int x = 0; x < LEVEL_WIDTH; x++) {
            level->tiles[y][x] = TILE_EMPTY;
        }
    }

    // Create ground floor (row 45)
    for (int x = 0; x < LEVEL_WIDTH; x++) {
        level->tiles[45][x] = TILE_GROUND;
    }

    // Add some platforms
    for (int x = 10; x < 20; x++) {
        level->tiles[40][x] = TILE_BRICK;
    }

    // Add coins
    level->tiles[39][15] = TILE_COIN;

    // Add goal
    level->tiles[44][180] = TILE_GOAL;

    level->coins_collected = 0;
}
```

## Level Design Patterns

### Ground with Gaps

```c
// Create ground with a gap for jumping
for (int x = 0; x < 50; x++) {
    if (x < 20 || x > 25) {  // Gap from 20-25
        level->tiles[45][x] = TILE_GROUND;
    }
}
```

### Staircase

```c
// Ascending staircase
int start_x = 30;
int start_y = 45;
for (int i = 0; i < 5; i++) {
    level->tiles[start_y - i][start_x + i * 2] = TILE_BRICK;
    level->tiles[start_y - i][start_x + i * 2 + 1] = TILE_BRICK;
}
```

### Floating Platform Section

```c
// Floating platforms at varying heights
int platforms[][3] = {
    // {x, y, width}
    {40, 38, 5},
    {50, 35, 4},
    {60, 32, 6},
    {72, 35, 5},
};

for (int i = 0; i < 4; i++) {
    for (int w = 0; w < platforms[i][2]; w++) {
        level->tiles[platforms[i][1]][platforms[i][0] + w] = TILE_PLATFORM;
    }
}
```

### Spike Gauntlet

```c
// Ground with spikes
for (int x = 80; x < 100; x++) {
    level->tiles[45][x] = TILE_GROUND;
    if (x % 3 == 0) {  // Spike every 3 tiles
        level->tiles[44][x] = TILE_SPIKE;
    }
}
```

### Coin Trail

```c
// Coins in an arc pattern
for (int i = 0; i < 10; i++) {
    float angle = 3.14159f * i / 9;
    int x = 50 + i * 2;
    int y = 40 - (int)(5 * sin(angle));
    level->tiles[y][x] = TILE_COIN;
}
```

## Complete Custom Level Example

Here's a complete mini-level design:

```c
void level_init(Level *level) {
    // Clear
    memset(level->tiles, TILE_EMPTY, sizeof(level->tiles));

    // === Section 1: Tutorial (0-40) ===

    // Ground floor
    for (int x = 0; x < 40; x++) {
        level->tiles[45][x] = TILE_GROUND;
    }

    // First platform to teach jumping
    for (int x = 15; x < 22; x++) {
        level->tiles[41][x] = TILE_BRICK;
    }
    level->tiles[40][18] = TILE_COIN;  // Reward for climbing

    // === Section 2: Platforming (40-80) ===

    // Gap challenge
    for (int x = 40; x < 45; x++) level->tiles[45][x] = TILE_GROUND;
    for (int x = 50; x < 55; x++) level->tiles[45][x] = TILE_GROUND;
    for (int x = 60; x < 80; x++) level->tiles[45][x] = TILE_GROUND;

    // One-way platforms over gaps
    for (int x = 46; x < 50; x++) level->tiles[43][x] = TILE_PLATFORM;
    for (int x = 55; x < 60; x++) level->tiles[43][x] = TILE_PLATFORM;

    // Coins on platforms
    level->tiles[42][48] = TILE_COIN;
    level->tiles[42][57] = TILE_COIN;

    // === Section 3: Hazards (80-120) ===

    // Ground with spike obstacles
    for (int x = 80; x < 120; x++) {
        level->tiles[45][x] = TILE_GROUND;
    }
    level->tiles[44][90] = TILE_SPIKE;
    level->tiles[44][91] = TILE_SPIKE;
    level->tiles[44][100] = TILE_SPIKE;
    level->tiles[44][101] = TILE_SPIKE;
    level->tiles[44][102] = TILE_SPIKE;

    // Safe platforms above spikes
    for (int x = 88; x < 95; x++) level->tiles[40][x] = TILE_PLATFORM;
    for (int x = 98; x < 106; x++) level->tiles[40][x] = TILE_PLATFORM;

    // === Section 4: Victory (120-140) ===

    // Final stretch
    for (int x = 120; x < 140; x++) {
        level->tiles[45][x] = TILE_GROUND;
    }

    // Victory platform
    for (int x = 132; x < 138; x++) {
        level->tiles[42][x] = TILE_BRICK;
    }
    level->tiles[41][135] = TILE_GOAL;

    // Celebration coins
    level->tiles[40][133] = TILE_COIN;
    level->tiles[40][135] = TILE_COIN;
    level->tiles[40][137] = TILE_COIN;

    level->coins_collected = 0;
}
```

## Level Design Tips

### Difficulty Curve

1. **Start easy** - Flat ground, low platforms
2. **Introduce mechanics** - One new concept at a time
3. **Combine mechanics** - Apply what was learned
4. **Challenge** - Harder versions of earlier sections
5. **Reward** - Victory area with celebration

### Visual Rhythm

```
Ground:  ####___####___####___####
Action:  run  jump run  jump run  jump
```

Alternate between safe zones and challenges.

### Testing Your Level

```bash
# Build with debug to see coordinates
make debug && ./tario

# Look for:
# - Impossible jumps (too far/high)
# - Cheap deaths (unavoidable spikes)
# - Boring stretches (nothing happening)
# - Missing rewards (hard section with no coins)
```

### Spawn Point

Set spawn point in `game_init()`:

```c
void game_init(Game *game) {
    // ...
    player_init(&game->player, 5.0f, 40.0f);  // x=5, y=40
}
```

Ensure spawn point is:
- Above solid ground
- Not inside a tile
- Not near hazards

## Future: File-Based Levels

See [LEVEL_FORMAT.md](../../LEVEL_FORMAT.md) for the planned file format that will allow loading levels from external files.
