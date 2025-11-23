# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Tario is a low-level ASCII side-scrolling platformer game inspired by classic titles like Super Mario Bros and Ghosts 'n Goblins. Built entirely in C using ANSI escape codes for terminal rendering, the game demonstrates retro game development techniques with modern C99 standards.

### Key Features
- Complete platformer physics with gravity, friction, and variable jump height
- Advanced mechanics: coyote time and jump buffering for responsive controls
- 60 FPS game loop with delta time for frame-independent physics
- Double-buffered rendering for flicker-free display
- Lives system, collectibles (coins), and hazards (spikes)
- One playable level with tutorial, challenges, and goal flag

## Build and Run Commands

```bash
# Compile the game (release build)
make

# Run the compiled game
./tario

# Build and run in one command
make run

# Clean build artifacts
make clean

# Build with debug symbols and DEBUG flag
make debug

# Run debug build
./tario

# Check for memory leaks (requires valgrind)
valgrind --leak-check=full ./tario

# Run tests (once test suite is created)
make test
```

### Build System Details
- Compiler: GCC with `-Wall -Wextra` flags enabled
- Standard: C99 with POSIX extensions (`-D_POSIX_C_SOURCE=200809L`)
- Object files: Built to `build/` directory
- Dependencies: Math library (`-lm`) for physics calculations
- All source files auto-discovered via wildcard pattern

## Architecture

### Directory Structure
```
tario/
├── src/              # Implementation files
│   ├── main.c        # Entry point, signal handling, main loop
│   ├── game.c        # Game loop, update/render, camera system
│   ├── player.c      # Player physics, animation, collision
│   ├── level.c       # Level data, tile system, collision queries
│   ├── render.c      # Screen buffer, double buffering, viewport
│   └── terminal.c    # Terminal control, raw mode, ANSI codes
├── include/          # Header files
│   ├── game.h
│   ├── player.h
│   ├── level.h
│   ├── render.h
│   └── terminal.h
├── build/            # Generated object files (gitignored)
├── Makefile          # Build configuration
├── README.md         # User-facing documentation
├── GAMEPLAY.md       # Detailed gameplay mechanics
└── CLAUDE.md         # This file
```

### Core Components

#### 1. Game Loop (game.c, game.h)
- **Fixed timestep**: 60 FPS target with delta time calculation
- **Frame timing**: Uses `clock_gettime()` for microsecond precision
- **Delta capping**: Maximum 0.1s to prevent physics explosions
- **Update/render cycle**: Separate update and render phases
- **Camera system**: Smooth scrolling, centered on player, boundary clamping
- **Input polling**: Non-blocking keyboard input via terminal raw mode

Key functions:
- `game_init()`: Initialize all systems
- `game_run()`: Main game loop
- `game_update()`: Update physics, collision, player state
- `game_render()`: Render game world to screen buffer
- `game_handle_input()`: Process keyboard input
- `game_cleanup()`: Free resources and restore terminal

#### 2. Player System (player.c, player.h)
- **Physics simulation**: Gravity (25 u/s²), velocity, friction (0.85x)
- **Movement**: Horizontal speed (15 u/s), max fall speed (20 u/s)
- **Jump mechanics**:
  - Variable height (hold/release for control)
  - Jump force (15 u/s upward velocity)
  - Coyote time (0.15s grace period after leaving ledge)
  - Jump buffering (0.1s input window before landing)
- **Collision detection**: AABB against level tiles, platforms
- **Animation states**: Idle, walk, jump, fall, death
- **Lives system**: 3 lives, respawn after death, game over

Key functions:
- `player_init()`: Initialize player at spawn point
- `player_update()`: Apply physics, timers, collision
- `player_move_left/right()`: Apply horizontal movement
- `player_jump_press/release()`: Handle jump input
- `player_kill()`: Trigger death animation
- `player_get_sprite()`: Return current animation character

#### 3. Level System (level.c, level.h)
- **Tile-based**: 200×50 tile grid
- **Tile types**: Ground (#), Brick (=), Platform (-), Spike (^), Coin (o), Goal (F), Pipes ([]), Question blocks (?)
- **Collision properties**: Solid, deadly, one-way platform, collectible
- **Level data**: Hardcoded in `level_init()` (extensible to file loading)
- **Sections**: Tutorial, platforming challenge, advanced, final/goal

Key functions:
- `level_init()`: Create level layout
- `level_is_solid/deadly/platform/goal/coin()`: Collision queries
- `level_collect_coin()`: Remove collected coins
- `level_get_tile()`: Query tile at position

#### 4. Rendering Engine (render.c, render.h)
- **Double buffering**: Prevents screen flicker
- **Screen buffer**: 2D character array matching terminal size
- **Viewport culling**: Only render visible tiles
- **Buffer clearing**: Reset buffer each frame
- **Commit**: Write entire buffer to terminal in one operation
- **ANSI codes**: Cursor positioning for efficient updates

Key functions:
- `screen_buffer_create()`: Allocate buffer
- `screen_buffer_clear()`: Reset buffer
- `screen_buffer_set()`: Write character to buffer
- `screen_buffer_draw()`: Render buffer to terminal
- `screen_buffer_destroy()`: Free buffer

#### 5. Terminal Control (terminal.c, terminal.h)
- **Raw mode**: Disable line buffering, echo, special chars
- **Non-canonical input**: Read individual keystrokes
- **ANSI escape codes**: Cursor movement, screen clearing, colors
- **Terminal restoration**: Cleanup on exit (signal handlers)
- **Size detection**: Query terminal dimensions

Key functions:
- `terminal_init()`: Enter raw mode, hide cursor
- `terminal_cleanup()`: Restore original terminal state
- `terminal_read_key()`: Non-blocking key read
- `terminal_get_size()`: Query terminal width/height
- ANSI helpers: `term_clear()`, `term_move_cursor()`, etc.

### Key Technical Considerations

#### Physics System
- **Gravity**: 25 units/second² (balanced for arcade feel)
- **Jump force**: 15 units/second upward velocity
- **Max fall speed**: 20 units/second terminal velocity
- **Friction**: 0.85 multiplier when grounded (sliding stop effect)
- **Frame independence**: Physics use delta time for consistent behavior

#### Collision Detection
- **AABB collision**: Axis-aligned bounding box against tiles
- **Multi-sampling**: Check corners and edges for precise collision
- **Separation**: Push player out of solid tiles
- **One-way platforms**: Only collide from above when falling
- **Coin collection**: Remove coin tile when player overlaps

#### Advanced Jump Mechanics
- **Variable jump**: Cancel upward velocity when button released
- **Coyote time**: 0.15s timer allows jump after leaving platform
- **Jump buffering**: 0.1s timer stores jump input before landing
- **Implementation**: Timers tracked in Player struct, decremented each frame

#### Camera System
- **Following**: Camera x/y centered on player position
- **Boundaries**: Clamp camera to level edges (0 to level_size - viewport)
- **Viewport**: Calculated from terminal size
- **Culling**: Only tiles within camera view are rendered

#### Input Handling
- **Terminal raw mode**: No line buffering, immediate char delivery
- **Non-blocking**: Uses `O_NONBLOCK` flag on stdin
- **Key mapping**: Multiple keys per action (WASD + arrows)
- **Jump release detection**: Timer-based since terminal can't detect key up

#### Terminal Compatibility
- **ANSI escape codes**: Standard codes for wide compatibility
- **Cursor positioning**: Absolute positioning for consistent display
- **Screen clearing**: Full screen clear before each frame
- **No color dependency**: Monochrome ASCII art (extensible to color)

### Code Quality Standards

- **C99 standard**: Modern C features while maintaining portability
- **POSIX compliance**: Uses POSIX APIs for terminal, timing
- **Compiler warnings**: All code compiles cleanly with `-Wall -Wextra`
- **No external dependencies**: Only standard library and math library
- **Error handling**: Check return values, graceful degradation
- **Memory management**: Proper allocation/deallocation, no leaks
- **Signal handling**: SIGINT, SIGTERM cleanup terminal state

### Common Development Tasks

#### Adding a New Tile Type
1. Add enum to `TileType` in `level.h`
2. Update `level_is_solid()`, `level_is_deadly()`, etc. in `level.c`
3. Update `game_render()` to draw new tile type
4. Add tiles to level layout in `level_init()`

#### Modifying Physics
1. Adjust constants in `player.c`: `GRAVITY`, `JUMP_FORCE`, `MAX_FALL_SPEED`, etc.
2. Test in-game for feel and balance
3. Update documentation in `GAMEPLAY.md` with new values

#### Creating a New Level
1. Define level layout in `level_init()` or load from file
2. Set spawn point in `game_init()`
3. Place goal flag at end
4. Playtest for difficulty curve and fun factor

#### Adding Features
1. Update relevant header file with new functions/structs
2. Implement in corresponding .c file
3. Integrate into game loop in `game.c`
4. Test thoroughly
5. Update README.md and GAMEPLAY.md

## Testing and Debugging

### Manual Testing
- Run game and playtest all mechanics
- Test edge cases: level boundaries, collision corners, death/respawn
- Verify terminal size adaptation
- Check input responsiveness

### Memory Leak Detection
```bash
make debug
valgrind --leak-check=full --show-leak-kinds=all ./tario
```

### Performance Profiling
```bash
# Compile with profiling
gcc -pg -O2 -Iinclude src/*.c -o tario -lm

# Run and generate profile
./tario
gprof tario gmon.out > analysis.txt
```

### Debug Output
- Compile with `make debug` to enable DEBUG preprocessor flag
- Add `#ifdef DEBUG` blocks for debug logging
- Example: Print player position, velocity, state

## Common Issues and Solutions

### Terminal Display Corrupted
- Ensure `terminal_cleanup()` is called on exit
- Check signal handlers restore terminal state
- Verify terminal size is at least 80×24

### Physics Feels Wrong
- Check delta time calculation in game loop
- Verify delta time is capped at 0.1s
- Ensure frame rate is stable (measure with debug output)

### Input Lag or Missing Presses
- Verify terminal is in raw mode (canonical disabled)
- Check stdin is set to non-blocking
- Ensure input is polled every frame

### Flickering Display
- Verify double buffering is working
- Check that screen is cleared before each render
- Ensure entire buffer is written in one operation

## Future Development Ideas

- **Multiple levels**: Level loading from files, level selection menu
- **Enemies**: Patrol AI, collision damage, enemy types
- **Power-ups**: Double jump, speed boost, invincibility
- **Sound**: ASCII art visual feedback for events (can't do audio in terminal)
- **Checkpoints**: Mid-level respawn points
- **Scoreboard**: Track high scores, time trials
- **Level editor**: Create custom levels
- **Portability**: Test on different terminals and platforms

## Performance Considerations

- **Frame rate**: 60 FPS is achievable on modern systems
- **Rendering**: Terminal I/O is the bottleneck, minimize writes
- **Memory**: Level is static array (200×50 tiles = 10KB)
- **CPU**: Physics calculations are lightweight
- **Optimization**: Profile before optimizing, focus on hot paths

## Resources and References

- **ANSI Escape Codes**: https://en.wikipedia.org/wiki/ANSI_escape_code
- **termios**: `man termios` for terminal control
- **Game Loop**: "Fix Your Timestep" by Glenn Fiedler
- **Platformer Physics**: Celeste dev blog (coyote time, jump buffering)

## Contributing

When working on this project:
1. Maintain C99 standard compliance
2. Keep code compiling with `-Wall -Wextra` (zero warnings)
3. Test on multiple terminal emulators
4. Update documentation when adding features
5. Follow existing code style and naming conventions
6. Add comments for complex logic
7. Test memory safety with valgrind
