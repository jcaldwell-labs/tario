#ifndef SOUND_H
#define SOUND_H

/*
 * Terminal-based Sound Framework
 *
 * Since terminals cannot produce audio, this framework provides visual
 * feedback for game events through ASCII art effects and visual cues.
 */

typedef enum {
  SOUND_JUMP,
  SOUND_COIN_COLLECT,
  SOUND_DEATH,
  SOUND_VICTORY,
  SOUND_HIT_BLOCK,
  SOUND_LAND
} SoundEffect;

// Visual feedback system
typedef struct {
  int active;              // Is effect currently showing
  SoundEffect effect_type; // Type of effect
  float timer;             // How long effect has been showing
  float duration;          // How long effect should show
  char symbol;             // Visual symbol to display
  int offset_x;            // X offset from source
  int offset_y;            // Y offset from source
} VisualEffect;

#define MAX_VISUAL_EFFECTS 8

typedef struct {
  VisualEffect effects[MAX_VISUAL_EFFECTS];
  int next_slot;
} SoundSystem;

// Initialize sound system
void sound_system_init(SoundSystem *sound);

// Play a sound effect (triggers visual feedback)
void sound_play(SoundSystem *sound, SoundEffect effect, int x, int y);

// Update sound system (for timed effects)
void sound_update(SoundSystem *sound, float delta_time);

// Render visual effects (called during game render)
void sound_render_effects(SoundSystem *sound, void *screen_buffer, int camera_x,
                          int camera_y);

#endif
