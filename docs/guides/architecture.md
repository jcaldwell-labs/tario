# Architecture Guide

This guide explains Tario's architecture for developers who want to understand or extend the codebase.

## Overview

Tario uses a modular C architecture with clear separation of concerns:

```
┌─────────────────────────────────────────────────────────┐
│                      main.c                             │
│              (Entry point, signal handling)             │
└───────────────────────┬─────────────────────────────────┘
                        │
┌───────────────────────▼─────────────────────────────────┐
│                      game.c                             │
│           (Game loop, update/render cycle)              │
├─────────────┬─────────────┬─────────────┬───────────────┤
│  player.c   │  level.c    │  render.c   │  terminal.c   │
│  (Physics)  │  (Tiles)    │  (Display)  │  (I/O)        │
└─────────────┴─────────────┴─────────────┴───────────────┘
```

## Components

### main.c - Entry Point

**Responsibilities:**
- Signal handling (SIGINT, SIGTERM)
- Terminal initialization and cleanup
- Main function entry

**Key functions:**
- `main()` - Initialize and run game
- Signal handlers for graceful exit

### game.c - Game Loop

**Responsibilities:**
- Fixed timestep game loop (60 FPS)
- Delta time calculation
- Update/render cycle coordination
- Camera system management
- Input polling

**Key functions:**
- `game_init()` - Initialize all systems
- `game_run()` - Main game loop
- `game_update()` - Update game state
- `game_render()` - Render current frame
- `game_handle_input()` - Process keyboard input
- `game_cleanup()` - Free resources

**Game Loop Flow:**
```c
while (running) {
    calculate_delta_time();
    handle_input();
    update(delta_time);
    render();
    sleep_until_next_frame();
}
```

### player.c - Player System

**Responsibilities:**
- Physics simulation (gravity, velocity)
- Collision detection and response
- Animation state management
- Advanced jump mechanics

**Key structures:**
```c
typedef struct {
    float x, y;           // Position
    float vel_x, vel_y;   // Velocity
    bool grounded;        // On ground?
    float coyote_timer;   // Coyote time remaining
    float jump_buffer;    // Jump buffer remaining
    int lives;            // Remaining lives
    // ... animation state
} Player;
```

**Key functions:**
- `player_init()` - Initialize at spawn
- `player_update()` - Apply physics each frame
- `player_move_left/right()` - Horizontal movement
- `player_jump_press/release()` - Jump input handling
- `player_kill()` - Trigger death

### level.c - Level System

**Responsibilities:**
- Tile-based level representation
- Collision queries (solid, deadly, etc.)
- Coin collection
- Goal detection

**Key structures:**
```c
typedef struct {
    TileType tiles[LEVEL_HEIGHT][LEVEL_WIDTH];
    int coins_collected;
} Level;

typedef enum {
    TILE_EMPTY,
    TILE_GROUND,
    TILE_BRICK,
    TILE_PLATFORM,
    TILE_SPIKE,
    TILE_COIN,
    TILE_GOAL,
    // ...
} TileType;
```

**Key functions:**
- `level_init()` - Create level layout
- `level_is_solid()` - Check if tile blocks movement
- `level_is_deadly()` - Check if tile kills player
- `level_is_platform()` - Check for one-way platforms
- `level_collect_coin()` - Remove collected coin

### render.c - Rendering Engine

**Responsibilities:**
- Double-buffered rendering
- Viewport management
- Screen buffer operations

**Key structures:**
```c
typedef struct {
    char **buffer;      // 2D character array
    int width, height;  // Buffer dimensions
} ScreenBuffer;
```

**Key functions:**
- `screen_buffer_create()` - Allocate buffer
- `screen_buffer_clear()` - Reset to empty
- `screen_buffer_set()` - Write character
- `screen_buffer_draw()` - Output to terminal
- `screen_buffer_destroy()` - Free memory

### terminal.c - Terminal Control

**Responsibilities:**
- Raw mode management
- Non-blocking input
- ANSI escape code helpers
- Terminal size detection

**Key functions:**
- `terminal_init()` - Enter raw mode
- `terminal_cleanup()` - Restore terminal
- `terminal_read_key()` - Non-blocking read
- `terminal_get_size()` - Query dimensions
- `term_clear()`, `term_move_cursor()` - ANSI helpers

## Data Flow

### Input Flow
```
Keyboard → terminal_read_key() → game_handle_input() → player_move/jump()
```

### Update Flow
```
game_update() → player_update() → collision checks → level queries
```

### Render Flow
```
game_render() → buffer_clear() → render tiles → render player → buffer_draw()
```

## Memory Management

- Level tiles: Static array (no allocation)
- Screen buffer: Allocated once at init, freed at cleanup
- Player: Stack-allocated in game state

## Extending the Architecture

### Adding a New Component

1. Create `src/newcomponent.c` and `include/newcomponent.h`
2. Add to Makefile source list
3. Include header in `game.c`
4. Initialize in `game_init()`, update in `game_update()`

### Adding New Tile Types

See [Adding Tiles Tutorial](../tutorials/adding-tiles.md)

## Thread Safety

Tario is single-threaded. All operations occur in the main game loop, ensuring no race conditions.

## Performance Considerations

- Viewport culling: Only render visible tiles
- Double buffering: Minimize terminal writes
- Delta time capping: Prevent physics explosions
- No dynamic allocation in hot paths
