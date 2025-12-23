# Tario

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![C99](https://img.shields.io/badge/C-99-blue.svg)](https://en.wikipedia.org/wiki/C99)
[![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20WSL-lightgrey.svg)](#requirements)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](CONTRIBUTING.md)

> A low-level ASCII side-scrolling platformer built entirely in C with ANSI escape codes. No dependencies. No frameworks. Just pure terminal gaming.

---

## Why Tario?

**The Problem:** Modern game engines abstract away the fundamentals. Want to understand how games really work under the hood? Most tutorials jump straight to Unity or Godot without teaching the core concepts.

**The Solution:** Tario is a complete, playable platformer that demonstrates:
- **Physics from scratch** - Gravity, velocity, friction, collision detection
- **Game loop architecture** - Fixed timestep, delta time, update/render separation
- **Terminal graphics** - Double buffering, viewport culling, ANSI escape codes
- **Advanced mechanics** - Coyote time, jump buffering, variable jump height

**Perfect for:**
- Learning low-level game development in C
- Teaching game programming concepts without engine overhead
- Retro gaming enthusiasts who appreciate ASCII art
- Anyone curious about what happens beneath modern game engines

---

## Demo

```
                    o     o
                  -----
    o           ---------
              =============
    >     ^^^^===========^^^^
####################################
```

**Try it now:**
```bash
git clone https://github.com/jcaldwell-labs/tario.git
cd tario
make && ./tario
```

---

## Quick Start

### Build and Run

```bash
# Clone and build
git clone https://github.com/jcaldwell-labs/tario.git
cd tario
make

# Play!
./tario
```

### Controls

| Key | Action |
|-----|--------|
| **A** / **Left Arrow** | Move left |
| **D** / **Right Arrow** | Move right |
| **W** / **Up Arrow** / **Space** | Jump |
| **P** | Pause/Unpause |
| **Q** / **ESC** | Quit |

### Requirements

- **Compiler**: GCC or compatible C compiler
- **Platform**: Linux, macOS, or WSL
- **Terminal**: 80×24 minimum, ANSI-compatible
- **Dependencies**: None (just libc and libm)

---

## Features

### Core Gameplay
- **Classic Platformer Mechanics** - Jump, run, collect coins, avoid hazards
- **Smooth 60 FPS** - Frame-independent physics with delta time
- **Lives System** - 3 lives with respawn mechanics
- **Progressive Difficulty** - Tutorial section leading to challenging platforming

### Advanced Jump Mechanics

| Mechanic | Description | Benefit |
|----------|-------------|---------|
| Variable Jump Height | Hold longer for higher jumps | Precise control |
| Coyote Time (0.15s) | Jump after leaving ledges | Forgiving platforming |
| Jump Buffering (0.1s) | Queue jumps before landing | Responsive controls |

### Technical Features
- **Double-buffered rendering** - Flicker-free display
- **Scrolling camera** - Smooth viewport following
- **AABB collision** - Precise hit detection
- **One-way platforms** - Jump through from below

---

## Game Elements

| Symbol | Name | Description |
|--------|------|-------------|
| `#` | Ground | Solid terrain blocks |
| `=` | Brick | Solid platform blocks |
| `-` | Platform | One-way (jump through from below) |
| `^` | Spikes | Deadly hazard - instant death! |
| `o` | Coin | Collectible items |
| `F` | Goal | Reach the flag to win! |
| `[ ]` | Pipes | Decorative obstacles |

See [GAMEPLAY.md](GAMEPLAY.md) for detailed mechanics and strategies.

---

## Project Structure

```
tario/
├── src/              # Implementation files
│   ├── main.c        # Entry point and signal handling
│   ├── game.c        # Game loop, update, and render logic
│   ├── player.c      # Player physics and animation
│   ├── level.c       # Level data and tile system
│   ├── render.c      # Screen buffering and rendering
│   └── terminal.c    # Terminal control and ANSI codes
├── include/          # Header files
├── tests/            # Test suite
├── docs/             # Documentation
└── Makefile          # Build configuration
```

---

## Architecture

### Core Components

| Component | File | Responsibility |
|-----------|------|----------------|
| Game Loop | `game.c` | Fixed 60 FPS timestep, update/render cycle |
| Player | `player.c` | Physics, animation, collision response |
| Level | `level.c` | Tile system, collision queries |
| Renderer | `render.c` | Double buffering, viewport culling |
| Terminal | `terminal.c` | Raw mode, ANSI escape codes |

### Physics Engine

```c
Gravity:        25 units/s²     // Balanced arcade feel
Jump Force:     15 units/s      // Upward velocity
Max Fall Speed: 20 units/s      // Terminal velocity
Friction:       0.85×           // Sliding stop effect
```

---

## Development

### Build Commands

```bash
make              # Build release version
make debug        # Build with debug symbols
make clean        # Remove build artifacts
make run          # Build and run
make test         # Run test suite
```

### Code Quality

```bash
# Check for memory leaks
valgrind --leak-check=full ./tario

# Code compiles with strict warnings
make  # Uses -Wall -Wextra
```

### Coding Standards

- **C99** with POSIX extensions
- Zero compiler warnings (`-Wall -Wextra`)
- No external dependencies
- Memory-leak free (verified with valgrind)

---

## Comparison

| Feature | Tario | SDL Games | ncurses Games |
|---------|-------|-----------|---------------|
| Dependencies | None | SDL2, SDL_image, etc. | ncurses |
| Learning Curve | Minimal | Moderate | Low |
| Portability | POSIX terminals | Cross-platform | POSIX/Windows |
| Educational Value | High (see everything) | Medium | Medium |
| Performance | Excellent | Excellent | Good |

---

## Use Cases

### Learning Game Development
```bash
# Study the physics implementation
cat src/player.c | grep -A 20 "apply_gravity"

# Understand the game loop
cat src/game.c | grep -A 30 "game_run"
```

### Teaching
- Demonstrate game loop architecture
- Show physics calculations step-by-step
- Explain collision detection algorithms
- Illustrate terminal graphics techniques

### Extending
- Add new tile types in `level.c`
- Implement enemies in a new `enemy.c`
- Create additional levels
- Add power-ups and abilities

---

## Roadmap

- [ ] Multiple levels with progression
- [ ] Enemy AI and combat
- [ ] Power-ups (double jump, speed boost)
- [ ] Checkpoints within levels
- [ ] High score persistence
- [ ] Level editor
- [ ] More tile types

See [.github/planning/](/.github/planning/) for detailed planning documents.

---

## Contributing

We welcome contributions! See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

### Quick Contribution Guide

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/your-feature`
3. Make changes following C99 standards
4. Test thoroughly (including valgrind)
5. Submit a pull request

### Good First Issues

Check the [issue tracker](https://github.com/jcaldwell-labs/tario/issues) for issues labeled `good first issue`.

---

## Documentation

- [GAMEPLAY.md](GAMEPLAY.md) - Detailed gameplay mechanics
- [CONTRIBUTING.md](CONTRIBUTING.md) - Contribution guidelines
- [LEVEL_FORMAT.md](LEVEL_FORMAT.md) - Level file format specification
- [SOUND_FRAMEWORK.md](SOUND_FRAMEWORK.md) - Visual feedback framework
- [CLAUDE.md](CLAUDE.md) - AI assistant guidance
- [docs/](docs/) - Additional documentation

---

## Related jcaldwell-labs Projects

Tario is part of the [jcaldwell-labs](https://github.com/jcaldwell-labs) ecosystem of terminal-focused tools and games.

### Terminal/TUI Projects

| Project | Description | Synergy with Tario |
|---------|-------------|-------------------|
| [my-grid](https://github.com/jcaldwell-labs/my-grid) | ASCII canvas editor with vim-style navigation | Level editor foundation - could edit Tario levels visually |
| [boxes-live](https://github.com/jcaldwell-labs/boxes-live) | Real-time ASCII box drawing with joystick support | Joystick input patterns for gamepad support in Tario |
| [terminal-stars](https://github.com/jcaldwell-labs/terminal-stars) | Starfield animation for terminals | Background effects for Tario's night/space levels |
| [atari-style](https://github.com/jcaldwell-labs/atari-style) | Retro visual effects and shaders for terminals | CRT effects, scanlines, and retro aesthetics for Tario |
| [smartterm-prototype](https://github.com/jcaldwell-labs/smartterm-prototype) | Smart terminal with readline-like features | Enhanced terminal input handling patterns |

### CLI Tools

| Project | Description |
|---------|-------------|
| [my-context](https://github.com/jcaldwell-labs/my-context) | Context tracking for development sessions (Go) |
| [fintrack](https://github.com/jcaldwell-labs/fintrack) | Personal finance tracking CLI (Go) |

### Game Engines

| Project | Description | Synergy with Tario |
|---------|-------------|-------------------|
| [adventure-engine-v2](https://github.com/jcaldwell-labs/adventure-engine-v2) | Multiplayer text adventure engine (C) | Shared C patterns, terminal handling, game loop architecture |

### Meta/Organization

| Project | Description |
|---------|-------------|
| [capability-catalog](https://github.com/jcaldwell-labs/capability-catalog) | Skill/capability definitions for AI agents |

---

## License

This project is provided under the MIT License. See [LICENSE](LICENSE) for details.

---

## Credits

**Inspired by:**
- Super Mario Bros (Nintendo)
- Ghosts 'n Goblins (Capcom)

**Built with:**
- C99 and POSIX APIs
- ANSI escape codes
- A love for retro gaming

---

## Acknowledgments

Thanks to the retro gaming community and the tradition of ASCII art games that prove you don't need fancy graphics to create engaging gameplay!

---

<p align="center">
  <strong>Ready to play?</strong><br>
  <code>git clone https://github.com/jcaldwell-labs/tario.git && cd tario && make && ./tario</code>
</p>
