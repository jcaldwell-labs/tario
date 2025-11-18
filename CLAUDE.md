# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Tario is a low-level ASCII side-scrolling video game inspired by classic platformers like Super Mario Brothers and Ghosts 'n Goblins. The game is built using C and ANSI escape codes for terminal rendering.

## Build and Run Commands

Development commands will be added as the build system is established. Expected commands:

```bash
# Compile the game
make

# Run the game
./tario

# Clean build artifacts
make clean

# Run with debug symbols
make debug && ./tario
```

## Architecture

### Core Components

The game architecture follows a classic game loop pattern with terminal-based rendering:

1. **Rendering Engine**: Uses ANSI escape codes for terminal graphics, following the "boxes-live" style for ASCII art aesthetics
2. **Game Loop**: Standard update/render cycle handling input, physics, and display
3. **Player Character**: Side-scrolling platformer mechanics with jump and movement physics
4. **Level System**: Tile-based level representation for platforms and obstacles

### Key Technical Considerations

- **ANSI Escape Codes**: All rendering uses ANSI codes for cursor positioning, colors, and screen clearing
- **Terminal Raw Mode**: The game requires terminal raw mode for real-time input handling
- **Frame Timing**: Consistent frame rate management for smooth gameplay
- **Double Buffering**: Screen buffer management to prevent flickering during rendering

### Code Organization

Expected structure as development progresses:
- Core game loop and initialization
- Input handling (keyboard/terminal control)
- Player character state and physics
- Rendering system (ANSI-based terminal output)
- Level data and tile system
- Collision detection and game mechanics

## Development Notes

- Use `termios.h` for terminal control and raw mode
- Target POSIX-compliant systems (Linux, macOS, WSL)
- Keep ANSI sequences portable across terminal emulators
- Consider terminal size detection for responsive layouts
