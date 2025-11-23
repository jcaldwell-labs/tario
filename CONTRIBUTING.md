# Contributing to Tario

Thank you for your interest in contributing to Tario! This document provides guidelines for contributing to this educational platformer game project.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Setup](#development-setup)
- [How to Contribute](#how-to-contribute)
- [Coding Standards](#coding-standards)
- [Testing Guidelines](#testing-guidelines)
- [Pull Request Process](#pull-request-process)
- [Areas for Contribution](#areas-for-contribution)

## Code of Conduct

This is an educational project. Please be respectful, constructive, and helpful in all interactions. We're here to learn and build cool stuff together!

## Getting Started

### Prerequisites

- GCC or compatible C compiler
- GNU Make
- POSIX-compliant operating system (Linux, macOS, WSL)
- Terminal emulator with ANSI support
- Optional: valgrind (for memory leak detection)
- Optional: clang-format (for code formatting)

### Initial Setup

1. Fork the repository on GitHub
2. Clone your fork locally:
   ```bash
   git clone https://github.com/YOUR-USERNAME/tario.git
   cd tario
   ```
3. Build the project:
   ```bash
   make
   ```
4. Run the game to ensure everything works:
   ```bash
   ./tario
   ```

## Development Setup

### Building

```bash
# Release build
make

# Debug build with symbols and DEBUG flag
make debug

# Clean build artifacts
make clean

# Build and run
make run
```

### Testing

```bash
# Run the game
./tario

# Memory leak check
valgrind --leak-check=full ./tario

# Run test suite
make test
```

## How to Contribute

### Reporting Bugs

If you find a bug, please create an issue with:
- **Title**: Brief description of the bug
- **Environment**: OS, terminal emulator, compiler version
- **Steps to Reproduce**: Detailed steps to trigger the bug
- **Expected Behavior**: What should happen
- **Actual Behavior**: What actually happens
- **Additional Context**: Screenshots, error messages, etc.

### Suggesting Enhancements

We welcome feature suggestions! Please create an issue with:
- **Title**: Brief description of the feature
- **Motivation**: Why this feature would be useful
- **Description**: Detailed explanation of the feature
- **Alternatives**: Any alternative solutions you've considered
- **Implementation Ideas**: Technical approach if you have one

### Submitting Changes

1. Create a new branch for your feature/fix:
   ```bash
   git checkout -b feature/your-feature-name
   # or
   git checkout -b fix/bug-description
   ```

2. Make your changes following our [coding standards](#coding-standards)

3. Test your changes thoroughly

4. Commit your changes with clear, descriptive messages:
   ```bash
   git commit -m "Add variable jump height to player mechanics

   - Implement jump button hold detection
   - Add velocity cancellation on release
   - Update physics constants for better feel
   - Document new behavior in GAMEPLAY.md"
   ```

5. Push to your fork:
   ```bash
   git push origin feature/your-feature-name
   ```

6. Create a Pull Request on GitHub

## Coding Standards

### C Code Style

- **Standard**: C99 with POSIX extensions
- **Indentation**: 4 spaces (no tabs)
- **Line Length**: Maximum 100 characters
- **Braces**: K&R style (opening brace on same line)
- **Naming Conventions**:
  - Functions: `snake_case` (e.g., `player_update`)
  - Variables: `snake_case` (e.g., `vel_x`)
  - Constants: `UPPER_SNAKE_CASE` (e.g., `MAX_FALL_SPEED`)
  - Structs: `PascalCase` (e.g., `Player`, `Level`)
  - Enums: `PascalCase` with `UPPER_SNAKE_CASE` values

### Code Quality Requirements

```bash
# All code must compile without warnings
make clean && make
# Should produce zero warnings

# Format code (if clang-format is available)
clang-format -i src/*.c include/*.h

# Check for memory leaks
valgrind --leak-check=full ./tario
# Should report "All heap blocks were freed -- no leaks are possible"
```

### Comments

- Add comments for complex algorithms or non-obvious logic
- Use `//` for single-line comments
- Use `/* */` for multi-line comments
- Document function purpose at definition (especially in headers)
- Keep comments up-to-date with code changes

Example:
```c
// Check collision with all solid tiles in the player's vicinity
// Uses AABB (Axis-Aligned Bounding Box) collision detection
bool player_check_collision(Player *player, Level *level) {
    // Implementation...
}
```

### Header Guards

All header files must use include guards:
```c
#ifndef MODULE_NAME_H
#define MODULE_NAME_H

// Header content...

#endif
```

## Testing Guidelines

### Manual Testing

Before submitting a PR, test your changes by:
1. Playing through the entire level
2. Testing edge cases (level boundaries, corners, etc.)
3. Verifying no memory leaks with valgrind
4. Testing on different terminal sizes
5. Checking that the game still compiles with `-Wall -Wextra`

### Automated Testing

Automated tests are available for Tario. Please ensure that you:
- Add unit tests for any new functions
- Add integration tests for new features
- Ensure all existing tests pass before submitting your PR
- Aim for reasonable test coverage of all new code

## Pull Request Process

### PR Checklist

Before submitting your PR, ensure:

- [ ] Code compiles without warnings (`make clean && make`)
- [ ] Code follows the coding standards
- [ ] Manual testing completed successfully
- [ ] No memory leaks detected by valgrind
- [ ] Documentation updated (README.md, GAMEPLAY.md, CLAUDE.md)
- [ ] Commit messages are clear and descriptive
- [ ] Changes are focused (one feature/fix per PR)

### PR Description Template

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix (non-breaking change fixing an issue)
- [ ] New feature (non-breaking change adding functionality)
- [ ] Breaking change (fix or feature causing existing functionality to change)
- [ ] Documentation update

## Testing Performed
- Describe how you tested your changes
- List edge cases tested
- Include valgrind output if relevant

## Screenshots/Gameplay
If applicable, add screenshots or describe gameplay changes

## Checklist
- [ ] Code compiles without warnings
- [ ] Manual testing completed
- [ ] No memory leaks
- [ ] Documentation updated
```

### Review Process

1. A maintainer will review your PR within a few days
2. Address any requested changes
3. Once approved, your PR will be merged
4. Your contribution will be acknowledged in release notes

## Areas for Contribution

### High Priority

- **Test Suite**: Create automated tests for physics, collision, level
- **Level Format**: Design and implement level file format for custom levels
- **Sound Framework**: Terminal-based visual feedback for game events
- **CI/CD**: Set up GitHub Actions for automated testing

### Feature Ideas

- **Multiple Levels**: Level selection menu, progression system
- **Enemies**: Moving obstacles, patrol AI, collision damage
- **Power-Ups**: Double jump, speed boost, invincibility
- **Checkpoints**: Mid-level respawn points
- **Score System**: Time tracking, perfect run bonuses
- **Level Editor**: Tool to create and test custom levels

### Code Quality

- **Performance**: Profile and optimize hot paths
- **Portability**: Test on different platforms and terminals
- **Refactoring**: Improve code structure and readability
- **Documentation**: Expand inline comments and guides

### Bug Fixes

Check the issue tracker for open bugs. Good first issues are labeled appropriately.

## Development Tips

### Understanding the Codebase

1. Read CLAUDE.md for architecture overview
2. Read GAMEPLAY.md for mechanics documentation
3. Start with main.c to understand program flow
4. Explore game.c for game loop structure
5. Study player.c for physics implementation

### Debugging

```bash
# Build with debug symbols
make debug

# Run under GDB
gdb ./tario

# Common GDB commands:
# break main        - Set breakpoint at main
# run               - Start execution
# next              - Step over
# step              - Step into
# print variable    - Print variable value
# continue          - Continue execution
```

### Code Navigation

- **Entry Point**: `src/main.c` - Signal handling, main loop initialization
- **Game Loop**: `src/game.c` - Update, render, input handling
- **Physics**: `src/player.c` - Gravity, velocity, collision
- **Level Data**: `src/level.c` - Tile layout, collision queries
- **Rendering**: `src/render.c` - Double buffering, screen output
- **Terminal**: `src/terminal.c` - Raw mode, ANSI codes

## Questions?

If you have questions not covered here:
- Check existing issues and discussions
- Create a new issue with the "question" label
- Reference CLAUDE.md for technical details
- Read the source code comments

## License

By contributing to Tario, you agree that your contributions will be licensed under the same license as the project.

## Recognition

All contributors will be acknowledged in the project. Thank you for helping make Tario better!

---

**Happy Coding!** 🎮

Whether you're fixing a typo, adding a feature, or just learning C game development, your contribution is valuable and appreciated!
