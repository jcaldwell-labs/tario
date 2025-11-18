# Tario - Gameplay Documentation

## Overview

Tario is an ASCII platformer game inspired by classic games like Super Mario Bros and Ghosts 'n Goblins. Navigate through a challenging level filled with platforms, obstacles, and collectibles to reach the goal flag.

## Controls

### Keyboard Controls
- **A / Left Arrow**: Move left
- **D / Right Arrow**: Move right
- **W / Up Arrow / Space**: Jump
- **P**: Pause/Unpause
- **Q / ESC**: Quit game

## Game Mechanics

### Movement
- **Horizontal Movement**: Press A/D or Left/Right arrows to move
- **Friction**: The character slides to a stop when you release movement keys
- **Speed**: Responsive movement at 15 units/second

### Jumping
The game features advanced jump mechanics for precise platforming:

1. **Variable Jump Height**
   - Hold jump button longer for higher jumps
   - Release early for shorter hops
   - Useful for navigating tight spaces and precise landings

2. **Coyote Time**
   - 0.15 second grace period after leaving a platform
   - Allows jumping shortly after walking off an edge
   - Makes platforming more forgiving and fluid

3. **Jump Buffering**
   - 0.1 second input buffer window
   - Press jump slightly before landing to jump immediately upon touching ground
   - Enables faster platforming without perfect timing

### Physics
- **Gravity**: 25 units/second² - balanced for arcade-style gameplay
- **Jump Force**: 15 units/second upward velocity
- **Max Fall Speed**: 20 units/second terminal velocity
- **Frame Rate**: 60 FPS for smooth, responsive gameplay

### Lives and Death
- **Starting Lives**: 3 lives
- **Death Triggers**:
  - Touching spikes (^)
  - Falling off the bottom of the level
- **Death Animation**: 2-second respawn delay with visual effect
- **Respawn**: Return to spawn point (preserving coins collected)
- **Game Over**: Occurs when all lives are lost

### Collectibles
- **Coins (o)**: Collect coins scattered throughout the level
- **Score Tracking**: HUD displays total coins collected
- **Persistence**: Coins remain collected even after death

### Camera System
- **Auto-Scrolling**: Camera follows player character
- **Centering**: Player stays centered in viewport
- **Boundary Clamping**: Camera stops at level edges
- **Viewport**: Dynamically adjusts to terminal size

## Level Elements

### Terrain Types
- **Ground (#)**: Solid ground blocks - cannot pass through
- **Brick (=)**: Solid brick platforms - cannot pass through
- **Platform (-)**: One-way platform - jump through from below, land on from above
- **Pipe ([ ])**: Decorative pipe obstacles - solid like bricks

### Hazards
- **Spikes (^)**: Instant death on contact - avoid at all costs!

### Interactive Elements
- **Coins (o)**: Collectible items that increase your score
- **Question Blocks (?)**: Decorative blocks (future: may contain coins)
- **Goal Flag (F)**: Reach the flag to complete the level!

## Level Design

The first level is divided into four sections:

### Section 1: Tutorial (x: 0-40)
- Flat ground for learning basic movement
- Low platforms introducing jumping
- Coins teaching collection mechanics
- Gentle difficulty curve

### Section 2: Platforming Challenge (x: 40-120)
- Gap jumps between platforms
- Vertical platforming sections
- Introduction to hazards (spikes)
- One-way platforms demonstrating advanced mechanics

### Section 3: Advanced Challenges (x: 130-180)
- Pipe obstacles requiring precise jumps
- Question blocks
- Staircase formations
- Combined mechanics from earlier sections

### Section 4: Final Challenge & Goal (x: 180-195)
- Spike gauntlet requiring skillful platforming
- One-way platforms for safe passage
- Victory platform with goal flag
- Celebratory coins around the finish

## Winning the Game

Reach the Goal Flag (F) on the victory platform to complete the level!
- Victory message appears: "*** VICTORY! Press Q to quit ***"
- Your final score (coins collected) is displayed in the HUD

## Tips and Strategies

1. **Master the Jump**: Practice variable jump heights in the tutorial section
2. **Use Coyote Time**: Don't be afraid to walk off edges - you have a grace period
3. **Jump Buffering**: Press jump just before landing for faster movement chains
4. **One-Way Platforms**: Use them to escape hazards by jumping up through them
5. **Coin Hunting**: Explore high and low - some coins require clever platforming
6. **Study the Level**: Use death as a learning opportunity to plan your route
7. **Pause When Needed**: Take breaks to strategize difficult sections

## Technical Details

### Performance
- **Target Frame Rate**: 60 FPS
- **Frame Time**: ~16.67ms per frame
- **Delta Time Capping**: 0.1s maximum to prevent physics errors

### Level Specifications
- **Level Size**: 200 tiles wide × 50 tiles tall
- **Viewport**: Adapts to terminal size (minimum 80×24 recommended)
- **Spawn Point**: (5, 40) - near the start of Section 1
- **Goal Location**: (192, ~42-44) - on the victory platform

### Animation System
- **Idle**: Directional character (> or <)
- **Walk**: Two-frame animation (> ) or ( <)
- **Jump**: Upward arrow (^)
- **Fall**: Downward arrow (v)
- **Death**: Four-frame cycling animation (x X * +)

## Accessibility

### Terminal Requirements
- Minimum terminal size: 80 columns × 24 rows
- Supports any ANSI-compatible terminal
- No color required (monochrome ASCII art)

### Input
- Simple keyboard controls
- No timing-critical input required (thanks to coyote time and jump buffering)
- Pause functionality for breaks

## Troubleshooting

### Game Running Too Fast/Slow
- The game targets 60 FPS but adapts to system performance
- Delta time capping prevents extreme speed variations

### Terminal Display Issues
- Ensure terminal is at least 80×24 characters
- Use a monospace font for proper alignment
- Some terminal emulators may handle ANSI codes differently

### Input Not Responding
- Terminal must be in raw mode (handled automatically)
- Check that no other programs are capturing keyboard input

## Future Enhancements

Potential additions for future versions:
- Multiple levels with increasing difficulty
- Power-ups (double jump, speed boost)
- Enemies and combat
- Checkpoints within levels
- Level editor
- High score system
- Sound effects (ASCII art-based visual feedback)
- More tile types and mechanics
