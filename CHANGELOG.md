# Changelog

All notable changes to the Tario project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Comprehensive test suite with 12 unit tests covering player physics, level mechanics, and rendering
- GitHub Actions CI/CD workflow for automated testing and code quality checks
- Enhanced Makefile with targets: test, check, valgrind, format, install, uninstall, help
- CONTRIBUTING.md with detailed contribution guidelines
- Issue templates for bug reports and feature requests
- Pull request template for consistent PR submissions
- Extensive CLAUDE.md documentation for AI-assisted development
- This CHANGELOG.md file

### Changed
- Improved code documentation and inline comments

### Fixed
- N/A (no bugs fixed in this release)

## [0.1.0] - 2025-11-23

### Added
- Initial release of Tario ASCII platformer game
- Complete platformer physics engine with gravity, friction, and collision detection
- Advanced jump mechanics: variable jump height, coyote time, and jump buffering
- 60 FPS game loop with delta time for frame-independent physics
- Double-buffered rendering system for flicker-free terminal display
- Terminal control with raw mode and ANSI escape codes
- Lives system (3 lives) with death and respawn mechanics
- First playable level with 4 sections:
  - Tutorial area
  - Platforming challenge
  - Advanced challenges
  - Final section with goal flag
- Multiple tile types: ground, bricks, platforms, spikes, coins, pipes, question blocks
- Collectible coins with score tracking
- Scrolling camera system that follows the player
- Pause functionality
- Victory condition (reach the goal flag)
- Game over when all lives are lost
- Player animations: idle, walk, jump, fall, death
- Keyboard controls: WASD + Arrow keys for movement, Space/W/Up for jump, P for pause, Q/ESC to quit
- Comprehensive README.md and GAMEPLAY.md documentation
- Build system with Makefile (build, clean, debug, run targets)
- Signal handlers for graceful terminal cleanup
- Memory-safe code with no leaks (verified with valgrind)
- POSIX-compliant implementation for Linux, macOS, and WSL

### Technical Details
- Written in C99 with POSIX extensions
- Compiles cleanly with `-Wall -Wextra` (zero warnings)
- Only dependencies: standard C library and math library
- Terminal requirements: 80x24 minimum, ANSI escape code support
- Level size: 200×50 tiles
- Physics constants tuned for arcade-style gameplay

---

## Version History Summary

- **[0.1.0]** - Initial release with complete platformer mechanics and first playable level
- **[Unreleased]** - Phase 2 improvements: testing, CI/CD, documentation, contribution guidelines

---

## Future Releases

Planned features for future versions:

### v0.2.0 (Planned)
- Multiple levels with progression system
- Level loading from files
- Terminal-based sound/visual feedback framework
- Enhanced documentation

### v0.3.0 (Planned)
- Enemy system with basic AI
- Power-ups (double jump, speed boost, invincibility)
- Checkpoints within levels
- Additional tile types and mechanics

### v1.0.0 (Planned)
- Complete game with multiple worlds
- Level editor
- High score persistence
- Comprehensive feature set

---

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on how to contribute to this project.

## License

This project is provided as-is for educational purposes.
