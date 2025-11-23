# Terminal-Based Sound Framework

## Overview

Since terminal applications cannot produce audio, Tario includes a visual feedback framework that provides "sound effects" through ASCII art and visual cues. This creates an engaging audiovisual experience despite the limitations of terminal output.

## Concept

The sound framework translates traditional game sound effects into visual feedback:
- **Jump Sound** → Upward caret (^) appears briefly below player
- **Coin Collection** → Dollar sign ($) floats upward
- **Death** → X symbol pulses at death location
- **Victory** → Star (*) appears above goal
- **Block Hit** → Exclamation (!) appears when hitting blocks
- **Landing** → Underscore (_) appears when player lands

## Architecture

### Components

1. **SoundSystem**: Manages all active visual effects
2. **VisualEffect**: Individual effect with position, symbol, duration
3. **SoundEffect**: Enum of available effect types

### Data Structures

```c
typedef enum {
    SOUND_JUMP,
    SOUND_COIN_COLLECT,
    SOUND_DEATH,
    SOUND_VICTORY,
    SOUND_HIT_BLOCK,
    SOUND_LAND
} SoundEffect;

typedef struct {
    int active;              // Is effect currently showing
    SoundEffect effect_type; // Type of effect
    float timer;             // How long effect has been showing
    float duration;          // How long effect should show
    char symbol;             // Visual symbol to display
    int offset_x;            // X offset from source
    int offset_y;            // Y offset from source
} VisualEffect;

typedef struct {
    VisualEffect effects[MAX_VISUAL_EFFECTS];
    int next_slot;
} SoundSystem;
```

## API Usage

### Initialization

```c
#include "sound.h"

SoundSystem sound;
sound_system_init(&sound);
```

### Playing Effects

```c
// When player jumps
sound_play(&sound, SOUND_JUMP, player_x, player_y);

// When collecting a coin
sound_play(&sound, SOUND_COIN_COLLECT, coin_x, coin_y);

// When player dies
sound_play(&sound, SOUND_DEATH, player_x, player_y);

// When reaching goal
sound_play(&sound, SOUND_VICTORY, goal_x, goal_y);

// When hitting a block from below
sound_play(&sound, SOUND_HIT_BLOCK, block_x, block_y);

// When player lands on ground
sound_play(&sound, SOUND_LAND, player_x, player_y);
```

### Game Loop Integration

```c
void game_update(Game *game, float delta_time) {
    // ... other update logic ...

    // Update sound effects
    sound_update(&game->sound, delta_time);
}

void game_render(Game *game) {
    // ... render game world ...

    // Render visual effects
    sound_render_effects(&game->sound, game->screen,
                        game->camera_x, game->camera_y);

    // ... commit buffer to screen ...
}
```

## Current Implementation Status

### ✅ Implemented
- Sound system structure and data types
- Effect initialization and timing
- Multiple simultaneous effects (up to 8)
- Effect lifecycle management
- Symbol and duration configuration per effect type

### ⚠️ Partial Implementation
- Position storage (simplified in current version)
- Rendering integration (framework ready, not yet connected)

### ❌ Not Yet Implemented
- Full integration with game loop
- Position tracking for each effect
- Animation sequences (multi-frame effects)
- Color support for effects
- Particle-like effects

## Integration Guide

To fully integrate the sound framework into Tario:

### Step 1: Add to Game Structure

Edit `include/game.h`:
```c
#include "sound.h"

typedef struct {
    // ... existing fields ...
    SoundSystem sound;
} Game;
```

### Step 2: Initialize in Game Init

Edit `src/game.c`:
```c
int game_init(Game *game) {
    // ... existing initialization ...

    sound_system_init(&game->sound);

    return 0;
}
```

### Step 3: Trigger Effects in Game Events

Edit `src/game.c` and `src/player.c`:
```c
// In player_jump_press():
if (can_jump) {
    player->vel_y = -JUMP_FORCE;
    sound_play(game_sound, SOUND_JUMP, player->x, player->y);
}

// In coin collection:
if (level_is_coin(&game->level, tile_x, tile_y)) {
    level_collect_coin(&game->level, tile_x, tile_y);
    game->player.coins_collected++;
    sound_play(&game->sound, SOUND_COIN_COLLECT, tile_x, tile_y);
}

// In player death:
void player_kill(Player *player, SoundSystem *sound) {
    player->is_dead = true;
    player->lives--;
    sound_play(sound, SOUND_DEATH, player->x, player->y);
}

// In victory:
if (level_is_goal(&game->level, tile_x, tile_y)) {
    game->victory = true;
    sound_play(&game->sound, SOUND_VICTORY, tile_x, tile_y);
}
```

### Step 4: Update and Render Effects

Edit `src/game.c`:
```c
void game_update(Game *game, float delta_time) {
    // ... existing update code ...

    sound_update(&game->sound, delta_time);
}

void game_render(Game *game) {
    // ... render level and player ...

    sound_render_effects(&game->sound, game->screen,
                        (int)game->camera_x, (int)game->camera_y);

    // ... commit buffer ...
}
```

### Step 5: Implement Effect Rendering

Edit `src/sound.c` to store positions:
```c
typedef struct {
    int active;
    SoundEffect effect_type;
    float timer;
    float duration;
    char symbol;
    int x, y;          // Add position storage
    int offset_x;
    int offset_y;
} VisualEffect;

void sound_play(SoundSystem *sound, SoundEffect effect, int x, int y) {
    // ... existing code ...

    vfx->x = x;  // Store position
    vfx->y = y;
}

void sound_render_effects(SoundSystem *sound, void *sb, int camera_x, int camera_y) {
    ScreenBuffer *screen = (ScreenBuffer *)sb;

    for (int i = 0; i < MAX_VISUAL_EFFECTS; i++) {
        VisualEffect *vfx = &sound->effects[i];
        if (vfx->active) {
            int screen_x = vfx->x - camera_x + vfx->offset_x;
            int screen_y = vfx->y - camera_y + vfx->offset_y;
            screen_buffer_draw_char(screen, screen_x, screen_y, vfx->symbol);
        }
    }
}
```

## Advanced Features

### Animation Sequences

For multi-frame effects:
```c
typedef struct {
    char symbols[4];   // Array of symbols for animation
    int frame_count;   // Number of frames
    float frame_time;  // Time per frame
} AnimatedEffect;
```

### Color Support

Add ANSI color codes:
```c
typedef struct {
    // ... existing fields ...
    int color_fg;  // Foreground color (0-7)
    int color_bg;  // Background color (0-7)
} VisualEffect;
```

### Particle Effects

Multiple small effects:
```c
void sound_play_particles(SoundSystem *sound, int x, int y, int count) {
    for (int i = 0; i < count; i++) {
        int offset_x = (rand() % 5) - 2;
        int offset_y = (rand() % 5) - 2;
        sound_play(sound, SOUND_PARTICLE, x + offset_x, y + offset_y);
    }
}
```

## Customization

### Adding New Effects

1. Add enum value to `SoundEffect`
2. Add case to switch statement in `sound_play()`
3. Define symbol, duration, and offsets
4. Trigger in appropriate game event

Example:
```c
// In sound.h:
typedef enum {
    // ... existing effects ...
    SOUND_POWERUP
} SoundEffect;

// In sound.c:
case SOUND_POWERUP:
    vfx->symbol = '+';
    vfx->duration = 0.6f;
    vfx->offset_x = 0;
    vfx->offset_y = -2;
    break;
```

### Adjusting Timing

Edit duration values in `sound_play()`:
```c
case SOUND_JUMP:
    vfx->duration = 0.3f;  // Increase from 0.2f for longer display
    break;
```

## Performance Considerations

- **Effect Limit**: Maximum 8 simultaneous effects to prevent performance degradation
- **Slot Reuse**: Oldest effects are replaced when all slots are full
- **Rendering Cost**: Minimal (one character draw per active effect)
- **Memory**: ~100 bytes for entire sound system

## Future Enhancements

Planned improvements:
- **Terminal Bell**: Use `\a` bell character for simple audio feedback
- **Color Coding**: Different colors for different effect types
- **Trail Effects**: Multiple symbols following movement
- **Screen Shake**: Brief camera offset for impactful events
- **Text Effects**: Short text messages ("JUMP!", "COIN!", etc.)
- **Combo System**: Visual feedback for consecutive actions

## Testing

Test the sound framework:
```bash
# Build with sound framework
make clean && make

# Run game and trigger events:
# - Jump (W/Space/Up) → Should see ^ briefly
# - Collect coin → Should see $ float up
# - Hit spikes → Should see X
# - Reach goal → Should see *
```

## Troubleshooting

### Effects Not Showing

1. Check integration steps completed
2. Verify `sound_update()` called in game loop
3. Confirm `sound_render_effects()` called after level rendering
4. Ensure camera offsets are correct

### Performance Issues

1. Reduce `MAX_VISUAL_EFFECTS` in `sound.h`
2. Shorten effect durations
3. Profile with `gprof` to identify bottlenecks

## Contributing

To contribute to the sound framework:
1. Follow coding standards in CONTRIBUTING.md
2. Test visual effects in various terminal sizes
3. Update this documentation with new features
4. Submit PR with screenshots or description of effects

---

**Enhance the experience!** 🔊 (visually, of course)
