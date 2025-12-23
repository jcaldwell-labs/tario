# Tario Documentation

Welcome to the Tario documentation! This directory contains guides, tutorials, and examples for understanding, playing, and extending Tario.

## Documentation Structure

```
docs/
├── README.md           # This file - documentation hub
├── guides/             # Reference guides
│   ├── architecture.md # System architecture overview
│   └── physics.md      # Physics engine details
├── tutorials/          # Step-by-step tutorials
│   └── adding-tiles.md # How to add new tile types
└── examples/           # Code examples
    └── custom-level.md # Creating custom levels
```

## Quick Navigation

### For Players
- [GAMEPLAY.md](../GAMEPLAY.md) - Controls, mechanics, and tips

### For Developers
- [Architecture Guide](guides/architecture.md) - Understand the codebase
- [Physics Guide](guides/physics.md) - How physics simulation works
- [Adding Tiles Tutorial](tutorials/adding-tiles.md) - Extend the game
- [CONTRIBUTING.md](../CONTRIBUTING.md) - How to contribute

### For AI Assistants
- [CLAUDE.md](../CLAUDE.md) - AI-friendly project documentation
- [llms.txt](../llms.txt) - Quick project overview

## Documentation by User Type

### "I want to play the game"
1. Read the [README Quick Start](../README.md#quick-start)
2. Check [GAMEPLAY.md](../GAMEPLAY.md) for mechanics

### "I want to understand the code"
1. Start with [Architecture Guide](guides/architecture.md)
2. Explore [Physics Guide](guides/physics.md)
3. Read source code comments in `src/`

### "I want to contribute"
1. Read [CONTRIBUTING.md](../CONTRIBUTING.md)
2. Review the [Architecture Guide](guides/architecture.md)
3. Check open issues on GitHub

### "I want to extend the game"
1. Follow [Adding Tiles Tutorial](tutorials/adding-tiles.md)
2. See [Custom Level Example](examples/custom-level.md)
3. Review [LEVEL_FORMAT.md](../LEVEL_FORMAT.md)

## Key Concepts

### Game Loop
The game runs at 60 FPS with a fixed timestep. Each frame:
1. Process input
2. Update physics
3. Check collisions
4. Render to buffer
5. Display buffer

### Physics
- Gravity-based vertical movement
- Velocity and friction for horizontal
- AABB collision detection
- One-way platform support

### Rendering
- Double-buffered for flicker-free display
- Viewport culling for performance
- ANSI escape codes for terminal control

## Contributing to Documentation

Documentation improvements are welcome! When adding docs:
- Use clear, concise language
- Include code examples where helpful
- Test any code snippets you include
- Follow the existing structure

## Related Documentation

| File | Description |
|------|-------------|
| [README.md](../README.md) | Project overview |
| [GAMEPLAY.md](../GAMEPLAY.md) | Gameplay mechanics |
| [CONTRIBUTING.md](../CONTRIBUTING.md) | Contribution guide |
| [LEVEL_FORMAT.md](../LEVEL_FORMAT.md) | Level file format |
| [SOUND_FRAMEWORK.md](../SOUND_FRAMEWORK.md) | Visual feedback system |
| [CHANGELOG.md](../CHANGELOG.md) | Version history |
