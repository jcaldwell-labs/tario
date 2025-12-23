# Tario Roadmap

## Vision

Tario aims to be a complete, educational platformer that demonstrates low-level game development techniques in C while providing an enjoyable retro gaming experience.

## Current State (v1.0)

- Complete single-level platformer
- Advanced physics (coyote time, jump buffering)
- 60 FPS game loop with delta time
- Double-buffered terminal rendering
- Lives system and coin collection

## Short-Term (v1.1)

### Multiple Levels
- [ ] Level file format implementation (see LEVEL_FORMAT.md)
- [ ] Level loading from external files
- [ ] Level selection menu
- [ ] Level progression system

### Quality of Life
- [ ] Pause menu with options
- [ ] Improved HUD display
- [ ] Terminal size warnings

## Medium-Term (v1.2)

### Enemies
- [ ] Basic enemy AI (patrol pattern)
- [ ] Collision damage system
- [ ] Enemy types (walker, jumper)
- [ ] Stomp mechanic

### Power-Ups
- [ ] Double jump
- [ ] Speed boost
- [ ] Invincibility star

## Long-Term (v2.0)

### Level Editor
- [ ] In-game level creation
- [ ] Tile palette interface
- [ ] Save/load custom levels
- [ ] Share levels (export format)

### Advanced Features
- [ ] Checkpoints
- [ ] High score persistence
- [ ] Time trial mode
- [ ] Multiple characters

## Stretch Goals

- Boss battles
- World map
- Multiplayer (turn-based or split-screen)
- Web/WASM port

## Non-Goals

To keep Tario focused and educational:
- No external graphics libraries (SDL, etc.)
- No sound (terminal limitation)
- No network play (complexity)
- No monetization

## Priorities

1. **Educational value** - Code should be readable and instructive
2. **Fun gameplay** - Core mechanics must feel good
3. **Simplicity** - Avoid over-engineering
4. **Portability** - Work on POSIX systems

## Contributing to Roadmap

Roadmap items can be discussed via:
- GitHub Issues (feature requests)
- Pull Requests (with discussion)
- Discussions (if enabled)

## Related Documents

- [Issue Backlog Summary](backlog-summary.md)
- [CONTRIBUTING.md](../../CONTRIBUTING.md)
- [LEVEL_FORMAT.md](../../LEVEL_FORMAT.md)
