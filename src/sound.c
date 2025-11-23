#include "sound.h"
#include "render.h"
#include <string.h>

void sound_system_init(SoundSystem *sound) {
    memset(sound, 0, sizeof(SoundSystem));
    sound->next_slot = 0;
}

void sound_play(SoundSystem *sound, SoundEffect effect, int x, int y) {
    // Find an available effect slot or reuse the oldest one
    int slot = sound->next_slot;
    sound->next_slot = (sound->next_slot + 1) % MAX_VISUAL_EFFECTS;

    VisualEffect *vfx = &sound->effects[slot];
    vfx->active = 1;
    vfx->effect_type = effect;
    vfx->timer = 0.0f;

    // Configure visual effect based on sound type
    switch (effect) {
        case SOUND_JUMP:
            vfx->symbol = '^';
            vfx->duration = 0.2f;
            vfx->offset_x = 0;
            vfx->offset_y = 1;
            break;

        case SOUND_COIN_COLLECT:
            vfx->symbol = '$';
            vfx->duration = 0.5f;
            vfx->offset_x = 0;
            vfx->offset_y = -1;
            break;

        case SOUND_DEATH:
            vfx->symbol = 'X';
            vfx->duration = 0.8f;
            vfx->offset_x = 0;
            vfx->offset_y = 0;
            break;

        case SOUND_VICTORY:
            vfx->symbol = '*';
            vfx->duration = 1.0f;
            vfx->offset_x = 0;
            vfx->offset_y = -2;
            break;

        case SOUND_HIT_BLOCK:
            vfx->symbol = '!';
            vfx->duration = 0.3f;
            vfx->offset_x = 0;
            vfx->offset_y = -1;
            break;

        case SOUND_LAND:
            vfx->symbol = '_';
            vfx->duration = 0.15f;
            vfx->offset_x = 0;
            vfx->offset_y = 1;
            break;

        default:
            vfx->active = 0;
            break;
    }

    // Store position (implementation note: this is simplified)
    // In full implementation, would store x,y coordinates
    (void)x;
    (void)y;
}

void sound_update(SoundSystem *sound, float delta_time) {
    for (int i = 0; i < MAX_VISUAL_EFFECTS; i++) {
        VisualEffect *vfx = &sound->effects[i];
        if (vfx->active) {
            vfx->timer += delta_time;
            if (vfx->timer >= vfx->duration) {
                vfx->active = 0;
            }
        }
    }
}

void sound_render_effects(SoundSystem *sound, void *screen_buffer, int camera_x, int camera_y) {
    // Render each active visual effect
    // Note: Full implementation would draw effects at their stored positions
    // This is a simplified version demonstrating the concept
    (void)screen_buffer;
    (void)camera_x;
    (void)camera_y;

    for (int i = 0; i < MAX_VISUAL_EFFECTS; i++) {
        VisualEffect *vfx = &sound->effects[i];
        if (vfx->active) {
            // In full implementation:
            // screen_buffer_draw_char(screen_buffer,
            //                         vfx->x - camera_x + vfx->offset_x,
            //                         vfx->y - camera_y + vfx->offset_y,
            //                         vfx->symbol);

            // For now, this framework is ready for integration
            // but not yet connected to the rendering pipeline
        }
    }
}
