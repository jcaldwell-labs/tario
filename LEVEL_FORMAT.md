# Tario Level Format Documentation

This document describes the level data structure and format used in Tario, as well as guidelines for creating custom levels.

## Overview

Tario uses a tile-based level system where each level is represented as a 2D grid of tiles. The current implementation uses a hardcoded level in `src/level.c`, but the system is designed to support loading levels from files in the future.

## Level Specifications

### Dimensions
- **Width**: 200 tiles (defined by `LEVEL_WIDTH` in `level.h`)
- **Height**: 50 tiles (defined by `LEVEL_HEIGHT` in `level.h`)
- **Total tiles**: 10,000 tiles per level
- **Coordinate system**: (0,0) is top-left, X increases right, Y increases down

### Tile Types

Each tile in the level is represented by a character from the `TileType` enum:

| Character | Type | Name | Collision | Deadly | Description |
|-----------|------|------|-----------|--------|-------------|
| `' '` (space) | TILE_EMPTY | Empty | No | No | Empty space, player can move through |
| `'#'` | TILE_GROUND | Ground | Solid | No | Solid ground block, cannot pass through |
| `'='` | TILE_BRICK | Brick | Solid | No | Brick platform block, cannot pass through |
| `'-'` | TILE_PLATFORM | Platform | One-way | No | Can jump through from below, land on from above |
| `'^'` | TILE_SPIKE | Spike | No | Yes | Deadly hazard, causes instant death |
| `'o'` | TILE_COIN | Coin | No | No | Collectible item, increments score |
| `'F'` | TILE_GOAL | Goal Flag | No | No | Victory condition, touching completes level |
| `'?'` | TILE_QUESTION | Question Block | Solid | No | Mystery block (currently decorative) |
| `'['` | TILE_PIPE_LEFT | Pipe (Left) | Solid | No | Left half of pipe obstacle |
| `']'` | TILE_PIPE_RIGHT | Pipe (Right) | Solid | No | Right half of pipe obstacle |

### Collision Properties

Tiles have the following collision properties:

**Solid Tiles** (`level_is_solid()`):
- `TILE_GROUND` - Ground blocks
- `TILE_BRICK` - Brick blocks
- `TILE_QUESTION` - Question blocks
- `TILE_PIPE_LEFT` - Left pipe
- `TILE_PIPE_RIGHT` - Right pipe

**One-Way Platforms** (`level_is_platform()`):
- `TILE_PLATFORM` - Can jump through from below, collides only when falling from above

**Deadly Tiles** (`level_is_deadly()`):
- `TILE_SPIKE` - Causes player death on contact

**Collectible Tiles** (`level_is_coin()`):
- `TILE_COIN` - Collected on contact, removed from level

**Goal Tiles** (`level_is_goal()`):
- `TILE_GOAL` - Triggers victory when reached

## Current Level Structure

The first level (`level_init()` in `src/level.c`) is divided into 4 sections:

### Section 1: Tutorial (X: 0-40)
- **Purpose**: Introduce basic movement and jumping
- **Features**:
  - Flat ground for walking
  - Low platforms for simple jumps
  - Scattered coins
  - No hazards
- **Spawn Point**: (5, 40)

### Section 2: Platforming Challenge (X: 40-130)
- **Purpose**: Teach platforming skills
- **Features**:
  - Gap jumps between platforms
  - Vertical platforming sections
  - Introduction to spikes
  - One-way platforms
  - Coins in challenging locations

### Section 3: Advanced Challenges (X: 130-180)
- **Purpose**: Test mastery of mechanics
- **Features**:
  - Pipe obstacles
  - Question blocks
  - Staircase formations
  - Combined mechanics from earlier sections

### Section 4: Final Challenge & Goal (X: 180-195)
- **Purpose**: Final test before victory
- **Features**:
  - Spike gauntlet
  - One-way platform escape routes
  - Victory platform with goal flag
  - Celebratory coins

## Creating Custom Levels

### Method 1: Hardcoded Level (Current)

To modify the current level, edit `src/level.c` in the `level_init()` function:

```c
void level_init(Level *level) {
    // Initialize entire level to empty
    for (int y = 0; y < LEVEL_HEIGHT; y++) {
        for (int x = 0; x < LEVEL_WIDTH; x++) {
            level->tiles[y][x] = TILE_EMPTY;
        }
    }

    // Add ground at bottom
    for (int x = 0; x < 50; x++) {
        level->tiles[LEVEL_HEIGHT - 1][x] = TILE_GROUND;
    }

    // Add platform at (10, 35)
    for (int x = 10; x < 20; x++) {
        level->tiles[35][x] = TILE_BRICK;
    }

    // Add coin at (15, 34)
    level->tiles[34][15] = TILE_COIN;

    // Add spike at (30, LEVEL_HEIGHT - 2)
    level->tiles[LEVEL_HEIGHT - 2][30] = TILE_SPIKE;

    // Add goal at end
    level->tiles[40][195] = TILE_GOAL;
}
```

### Method 2: Level File Format (Future)

For extensibility, levels could be loaded from text files. Proposed format:

**File Format**: ASCII text file with `.tario` extension

**Structure**:
```
LEVEL 1
WIDTH 200
HEIGHT 50
SPAWN 5 40

# Level data (50 rows of 200 characters each)


# ... (empty rows)

                                            o
                                         ========

# ... (continue for all 50 rows)
################################################################################################################################################################################################################
```

**Parsing Logic** (to be implemented):
```c
int level_load_from_file(Level *level, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return -1;

    char line[256];
    int width, height, spawn_x, spawn_y;

    // Parse header
    fscanf(fp, "LEVEL %*d\n");
    fscanf(fp, "WIDTH %d\n", &width);
    fscanf(fp, "HEIGHT %d\n", &height);
    fscanf(fp, "SPAWN %d %d\n", &spawn_x, &spawn_y);

    // Skip empty line
    fgets(line, sizeof(line), fp);

    // Parse level data
    for (int y = 0; y < height; y++) {
        if (!fgets(line, sizeof(line), fp)) break;

        for (int x = 0; x < width && line[x] != '\0' && line[x] != '\n'; x++) {
            level->tiles[y][x] = (TileType)line[x];
        }
    }

    fclose(fp);
    return 0;
}
```

## Level Design Guidelines

### Balance

- **Tutorial First**: Start with simple, flat areas
- **Progressive Difficulty**: Gradually introduce mechanics
- **Fair Challenges**: Difficult sections should be skill-based, not luck-based
- **Safe Respawns**: Spawn point should be safe from hazards

### Pacing

- **Breathing Room**: Include safe areas between challenges
- **Visual Clarity**: Use different tile types to guide the player
- **Coin Placement**: Reward exploration and skill

### Technical Constraints

- **Level Bounds**: Stay within 0 ≤ x < 200, 0 ≤ y < 50
- **Viewport**: Player sees ~40 tiles horizontally (depends on terminal width)
- **Jump Height**: Player can jump ~6-7 tiles high
- **Jump Distance**: Player can jump ~10-12 tiles horizontally
- **Fall Distance**: Unlimited falling (player dies if falling off bottom)

### Best Practices

1. **Ground Layer**: Always have ground or platforms for the player to stand on
2. **Escape Routes**: Provide ways to escape difficult situations
3. **Clear Goals**: Make the goal flag visible or clearly indicated
4. **Coin Trails**: Use coins to guide the player through intended paths
5. **One-Way Platforms**: Use `-` tiles to create shortcuts or escape routes
6. **Spike Placement**: Leave enough space to avoid spikes (not instant death traps)
7. **Testing**: Playtest your level extensively

## Example Level Snippets

### Simple Platform Gap
```

  ===  ===

##########
```

### Staircase
```
        ===
      ===
    ===
  ===
===
```

### Spike Hazard with Safe Path
```

  -------

  ^ ^ ^ ^
##########
```

### Coin Collection Challenge
```
     o
   ===
  o   o
 ===  ===
o       o
###########
```

### Pipe Obstacle
```

  []
  []
  []
########
```

## Modifying Level Dimensions

To change level dimensions:

1. Edit `LEVEL_WIDTH` and `LEVEL_HEIGHT` in `include/level.h`
2. Update the `level_init()` function in `src/level.c`
3. Adjust camera bounds in `src/game.c` if needed
4. Recompile: `make clean && make`

**Warning**: Large levels may impact performance or exceed terminal display capabilities.

## Future Enhancements

Planned improvements to the level system:

- **Level Loading**: Load levels from text or binary files
- **Level Editor**: Visual tool for creating levels
- **Multiple Levels**: Support for level progression and selection
- **Metadata**: Level name, author, difficulty rating
- **Scripting**: Triggers, moving platforms, dynamic elements
- **Compression**: Compressed level format for larger levels
- **Validation**: Tool to validate level design (reachable goal, no impossible sections)

## API Reference

Key functions for working with levels:

```c
// Initialize level (src/level.c)
void level_init(Level *level);

// Query tile properties (src/level.c)
bool level_is_solid(Level *level, int x, int y);
bool level_is_deadly(Level *level, int x, int y);
bool level_is_platform(Level *level, int x, int y);
bool level_is_goal(Level *level, int x, int y);
bool level_is_coin(Level *level, int x, int y);

// Get tile type (src/level.c)
TileType level_get_tile(Level *level, int x, int y);

// Collect coin (src/level.c)
void level_collect_coin(Level *level, int x, int y);
```

## Contributing Levels

If you create a custom level and would like to share it:

1. Create a level file following the proposed format
2. Playtest thoroughly
3. Submit via pull request with:
   - Level file in `levels/` directory
   - Screenshot or description
   - Estimated difficulty
   - Any special mechanics used

---

**Happy Level Designing!** 🎮
