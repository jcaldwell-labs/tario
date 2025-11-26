# GitHub Copilot Instructions

This file provides guidance to GitHub Copilot when working with code in this repository.

## Project Overview

Tario is a low-level ASCII side-scrolling platformer game inspired by Super Mario Bros and Ghosts 'n Goblins. Built entirely in C using ANSI escape codes for terminal rendering, featuring 60 FPS gameplay with advanced jump mechanics.

Key features:
- Complete platformer physics with gravity, friction, and variable jump height
- Advanced mechanics: coyote time (0.15s) and jump buffering (0.1s)
- 60 FPS game loop with delta time for frame-independent physics
- Double-buffered rendering for flicker-free display
- Lives system, collectibles (coins), and hazards (spikes)
- No external dependencies beyond standard C library and math

## Build System

```bash
make              # Build the game (release mode)
make debug        # Build with debug symbols and DEBUG flag
make run          # Build and run the game
make test         # Run test suite
make check        # Check code quality (strict compilation)
make valgrind     # Run memory leak detection (requires valgrind)
make format       # Format code with clang-format
make install      # Install to /usr/local/bin (may require sudo)
make uninstall    # Uninstall from system
make clean        # Remove all build artifacts
make help         # Display help message
```

**Dependencies:**
- **Required**: GCC compiler, math library (`-lm`)
- **Optional**: valgrind (memory checking), clang-format (code formatting)

**Compiler Flags**: `-Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L`

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
├── tests/            # Test files
│   └── test_main.c
├── build/            # Generated object files (gitignored)
└── Makefile
```

### Core Components

1. **Game Loop (game.c)**: Fixed 60 FPS timestep, delta time calculation, update/render cycle
2. **Player System (player.c)**: Physics (gravity 25 u/s², jump force 15 u/s), coyote time, jump buffering
3. **Level System (level.c)**: 200x50 tile grid, collision queries, tile types
4. **Rendering (render.c)**: Double-buffered screen buffer, viewport culling
5. **Terminal (terminal.c)**: Raw mode, non-blocking input, ANSI escape codes

### Key Technical Details

**Physics Constants:**
- Gravity: 25 units/second²
- Jump force: 15 units/second upward
- Max fall speed: 20 units/second
- Friction: 0.85 multiplier when grounded
- Horizontal speed: 15 units/second

**Jump Mechanics:**
- Coyote time: 0.15s grace period after leaving ledge
- Jump buffering: 0.1s input window before landing
- Variable height: Cancel upward velocity on button release

**Tile Types:**
- Ground (#), Brick (=), Platform (-), Spike (^)
- Coin (o), Goal (F), Pipes ([]), Question blocks (?)

## Code Style and Conventions

- **Standard**: C99 with POSIX extensions
- **Naming**: snake_case for functions/variables, UPPER_CASE for constants
- **Headers**: Guard macros, public API only
- **Memory**: Proper allocation/deallocation, no leaks (verify with valgrind)
- **Error handling**: Check return values, graceful degradation
- **Signal handling**: SIGINT/SIGTERM cleanup terminal state

**Module Pattern:**
- Each module has init/cleanup functions
- Separate update and render phases
- Delta-time scaled physics

## Before Committing (Required Steps)

Run these commands before every commit:

1. **Build**: `make clean && make` - Ensure zero warnings
2. **Test**: `make test` - All tests must pass
3. **Memory check**: `make valgrind` - No memory leaks
4. **Manual test**: `./tario` - Play through game, test controls

```bash
# Quick pre-commit check
make clean && make && make test
```

**Optional but recommended**:
- `make check` - Strict compilation with -Werror
- `make format` - Format code consistently

## Common Development Tasks

### Adding a New Tile Type
1. Add enum to `TileType` in `level.h`
2. Update `level_is_solid()`, `level_is_deadly()`, etc. in `level.c`
3. Update `game_render()` to draw new tile type
4. Add tiles to level layout in `level_init()`

### Modifying Physics
1. Adjust constants in `player.c`: `GRAVITY`, `JUMP_FORCE`, `MAX_FALL_SPEED`
2. Test in-game for feel and balance
3. Update documentation in `GAMEPLAY.md`

### Creating a New Level
1. Define level layout in `level_init()` or load from file
2. Set spawn point in `game_init()`
3. Place goal flag at end
4. Playtest for difficulty curve

## Pull Request Standards

When creating PRs, follow these rules:

1. **Always link the issue**: Use `Fixes #N` or `Closes #N` in the PR body
2. **Fill in all sections**: Never leave placeholder text or empty bullets

**Required PR format:**
```markdown
## Summary
[2-3 sentences describing what and why]

Fixes #[issue-number]

## Changes
- [Actual change 1]
- [Actual change 2]

## Testing
- [x] All tests pass (`make test`)
- [x] No memory leaks (`make valgrind`)
- [x] Manual gameplay tested

## Type
- [x] New feature | Bug fix | Refactor | Docs | CI
```

## Debugging Tips

### Terminal Display Corrupted
- Ensure `terminal_cleanup()` is called on exit
- Check signal handlers restore terminal state
- Verify terminal size is at least 80x24

### Physics Feels Wrong
- Check delta time calculation in game loop
- Verify delta time is capped at 0.1s
- Ensure frame rate is stable

### Input Lag or Missing Presses
- Verify terminal is in raw mode
- Check stdin is set to non-blocking
- Ensure input is polled every frame

## Additional Documentation

- **README.md** - Project overview and quick start
- **GAMEPLAY.md** - Detailed gameplay mechanics and physics values
