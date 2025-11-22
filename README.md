# Tario

A low-level ASCII side-scrolling platformer game inspired by classic titles like Super Mario Bros and Ghosts 'n Goblins. Built with C and ANSI escape codes for terminal rendering.

## Features

- **Classic Platformer Gameplay**: Jump, run, and collect coins through a challenging level
- **Advanced Jump Mechanics**: Variable jump height, coyote time, and jump buffering for responsive controls
- **Smooth 60 FPS Gameplay**: Consistent frame timing for fluid movement and physics
- **ASCII Art Graphics**: Retro "boxes-live" style terminal graphics using ANSI escape codes
- **Scrolling Camera**: Dynamically follows the player through expansive levels
- **Lives System**: 3 lives with death and respawn mechanics
- **Collectibles**: Coins scattered throughout the level
- **Progressive Difficulty**: Tutorial section leading to challenging platforming

## Quick Start

### Build and Run

```bash
# Compile the game
make

# Run the game
./tario

# Clean build artifacts
make clean

# Build with debug symbols
make debug && ./tario
```

### Controls

| Key | Action |
|-----|--------|
| **A** / **Left Arrow** | Move left |
| **D** / **Right Arrow** | Move right |
| **W** / **Up Arrow** / **Space** | Jump |
| **P** | Pause/Unpause |
| **Q** / **ESC** | Quit |

## Requirements

- **Compiler**: GCC or compatible C compiler
- **Platform**: POSIX-compliant system (Linux, macOS, WSL)
- **Terminal**: ANSI-compatible terminal emulator
- **Minimum Size**: 80 columns × 24 rows
- **Dependencies**: Standard C library, math library (-lm)

## Gameplay

Navigate through a side-scrolling level filled with platforms, hazards, and collectibles. Master the advanced jump mechanics to overcome challenges and reach the goal flag!

### Game Elements

- **# = Ground**: Solid terrain blocks
- **= = Brick**: Solid platform blocks
- **- = Platform**: One-way platforms (jump through from below)
- **^ = Spikes**: Deadly hazard - avoid at all costs!
- **o = Coin**: Collectible items
- **F = Goal**: Reach the flag to win!
- **[ ] = Pipes**: Decorative obstacles

### Advanced Jump Mechanics

1. **Variable Jump Height**: Hold jump longer for higher jumps, release early for shorter hops
2. **Coyote Time**: 0.15s grace period to jump after walking off edges
3. **Jump Buffering**: 0.1s window to press jump before landing for instant response

See [GAMEPLAY.md](GAMEPLAY.md) for detailed mechanics and strategies.

## Project Structure

```
tario/
├── src/
│   ├── main.c          # Entry point and signal handling
│   ├── game.c          # Game loop, update, and render logic
│   ├── player.c        # Player physics and animation
│   ├── level.c         # Level data and tile system
│   ├── render.c        # Screen buffering and rendering
│   └── terminal.c      # Terminal control and ANSI codes
├── include/
│   ├── game.h
│   ├── player.h
│   ├── level.h
│   ├── render.h
│   └── terminal.h
├── Makefile            # Build configuration
├── README.md           # This file
├── GAMEPLAY.md         # Detailed gameplay documentation
└── CLAUDE.md           # Project guidance for Claude Code
```

## Architecture

### Core Components

1. **Game Loop** (game.c)
   - Fixed timestep at 60 FPS
   - Update/render cycle
   - Input handling
   - Camera system

2. **Player System** (player.c)
   - Physics simulation (gravity, velocity, friction)
   - Advanced jump mechanics
   - Animation states
   - Collision response

3. **Level System** (level.c)
   - Tile-based level representation (200×50 tiles)
   - Multiple tile types with different properties
   - Coin collection tracking
   - Goal detection

4. **Rendering Engine** (render.c)
   - Double buffering for flicker-free display
   - Viewport culling
   - Character-based graphics

5. **Terminal Control** (terminal.c)
   - Raw mode for real-time input
   - ANSI escape codes for cursor and colors
   - Terminal size detection

## Technical Highlights

### Physics Engine
- Gravity: 25 units/s²
- Jump force: 15 units/s upward velocity
- Max fall speed: 20 units/s terminal velocity
- Friction: 0.85 multiplier per frame when grounded

### Performance
- **Frame Rate**: Locked to 60 FPS
- **Frame Time**: ~16.67ms sleep between frames
- **Delta Time**: Calculated for frame-independent physics
- **Delta Capping**: 0.1s maximum to prevent physics explosions

### Camera System
- Smooth scrolling following player position
- Centered on player with boundary clamping
- Viewport adapts to terminal size
- Only renders visible tiles (culling)

## Development

### Building

The Makefile supports several targets:

```bash
make              # Build release version
make debug        # Build with debug symbols and DEBUG flag
make clean        # Remove build artifacts
make run          # Build and run in one command
```

### Code Style

- C99 standard
- POSIX-compliant system calls
- Compiler flags: `-Wall -Wextra` for strict warnings
- No external dependencies beyond standard library

### Testing

```bash
# Run the game
./tario

# Check for memory leaks (requires valgrind)
valgrind --leak-check=full ./tario

# Run with debug output
make debug && ./tario
```

## Known Limitations

- **Input System**: Terminal raw mode doesn't detect key release, so variable jump height relies on timer-based detection rather than true key up events
- **Terminal Compatibility**: Some terminals may render ANSI codes differently
- **No Persistence**: Game state is not saved between sessions
- **Single Level**: Currently features one level (extensible for future levels)

## Future Enhancements

- Multiple levels with progressive difficulty
- Enemies and combat mechanics
- Power-ups (double jump, speed boost, invincibility)
- Checkpoints within levels
- High score persistence
- Level editor
- Sound effects (visual feedback)
- More tile types and interactive elements

## Contributing

This is an educational project demonstrating low-level game development in C. Feel free to:
- Study the code to learn about game loops, physics, and terminal graphics
- Extend the game with new features
- Design additional levels
- Optimize performance
- Fix bugs or improve code quality

## License

This project is provided as-is for educational purposes.

## Credits

Inspired by classic platformers:
- Super Mario Bros (Nintendo)
- Ghosts 'n Goblins (Capcom)

Built with:
- C programming language
- ANSI escape codes for terminal graphics
- POSIX terminal control (termios)

## Acknowledgments

Thanks to the retro gaming community and the tradition of ASCII art games that prove you don't need fancy graphics to create engaging gameplay!
