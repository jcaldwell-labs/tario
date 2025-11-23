#include "level.h"
#include <string.h>

// Helper function to draw a horizontal line of tiles
static void draw_hline(Level *level, int x1, int x2, int y, TileType tile) {
  for (int x = x1; x <= x2 && x < LEVEL_WIDTH; x++) {
    if (x >= 0 && y >= 0 && y < LEVEL_HEIGHT) {
      level->tiles[y][x] = tile;
    }
  }
}

// Helper function to draw a vertical line of tiles
static void draw_vline(Level *level, int x, int y1, int y2, TileType tile) {
  for (int y = y1; y <= y2 && y < LEVEL_HEIGHT; y++) {
    if (x >= 0 && x < LEVEL_WIDTH && y >= 0) {
      level->tiles[y][x] = tile;
    }
  }
}

// Helper function to draw a filled rectangle
static void draw_rect(Level *level, int x1, int y1, int x2, int y2,
                      TileType tile) {
  for (int y = y1; y <= y2; y++) {
    for (int x = x1; x <= x2; x++) {
      if (x >= 0 && x < LEVEL_WIDTH && y >= 0 && y < LEVEL_HEIGHT) {
        level->tiles[y][x] = tile;
      }
    }
  }
}

void level_init(Level *level) {
  // Clear all tiles
  memset(level->tiles, TILE_EMPTY, sizeof(level->tiles));

  // Create ground floor (bottom 2 rows)
  for (int x = 0; x < LEVEL_WIDTH; x++) {
    level->tiles[LEVEL_HEIGHT - 1][x] = TILE_GROUND;
    level->tiles[LEVEL_HEIGHT - 2][x] = TILE_GROUND;
  }

  // === SECTION 1: TUTORIAL (x: 0-40) ===
  // Flat ground to start

  // First platform (low)
  draw_hline(level, 10, 18, LEVEL_HEIGHT - 6, TILE_BRICK);

  // Coins above first platform (teach collection)
  for (int x = 11; x < 18; x++) {
    level->tiles[LEVEL_HEIGHT - 7][x] = TILE_COIN;
  }

  // Second platform (higher - teach jumping)
  draw_hline(level, 22, 30, LEVEL_HEIGHT - 9, TILE_BRICK);

  // More coins
  for (int x = 23; x < 30; x++) {
    level->tiles[LEVEL_HEIGHT - 10][x] = TILE_COIN;
  }

  // === SECTION 2: PLATFORMING CHALLENGE (x: 40-120) ===

  // Gap with platforms
  draw_hline(level, 45, 52, LEVEL_HEIGHT - 6, TILE_BRICK);
  draw_hline(level, 58, 65, LEVEL_HEIGHT - 8, TILE_BRICK);
  draw_hline(level, 71, 78, LEVEL_HEIGHT - 10, TILE_BRICK);

  // Coins on platforms
  for (int x = 46; x < 52; x++) {
    level->tiles[LEVEL_HEIGHT - 7][x] = TILE_COIN;
  }

  // Vertical platforming section
  draw_hline(level, 85, 90, LEVEL_HEIGHT - 5, TILE_BRICK);
  draw_hline(level, 92, 97, LEVEL_HEIGHT - 9, TILE_BRICK);
  draw_hline(level, 99, 104, LEVEL_HEIGHT - 13, TILE_BRICK);
  draw_hline(level, 106, 111, LEVEL_HEIGHT - 17, TILE_BRICK);

  // Spikes to introduce hazard (can jump over or platform around)
  for (int x = 115; x < 125; x++) {
    level->tiles[LEVEL_HEIGHT - 3][x] = TILE_SPIKE;
  }

  // Platform above spikes
  draw_hline(level, 115, 124, LEVEL_HEIGHT - 7, TILE_PLATFORM);

  // === SECTION 3: ADVANCED CHALLENGES (x: 130-180) ===

  // Pipe obstacles
  draw_vline(level, 135, LEVEL_HEIGHT - 6, LEVEL_HEIGHT - 3, TILE_PIPE_LEFT);
  draw_vline(level, 136, LEVEL_HEIGHT - 6, LEVEL_HEIGHT - 3, TILE_PIPE_RIGHT);

  draw_vline(level, 142, LEVEL_HEIGHT - 8, LEVEL_HEIGHT - 3, TILE_PIPE_LEFT);
  draw_vline(level, 143, LEVEL_HEIGHT - 8, LEVEL_HEIGHT - 3, TILE_PIPE_RIGHT);

  // Question blocks
  for (int x = 150; x < 156; x += 2) {
    level->tiles[LEVEL_HEIGHT - 8][x] = TILE_QUESTION;
  }

  // Staircase
  for (int i = 0; i < 8; i++) {
    draw_rect(level, 160 + i, LEVEL_HEIGHT - 3 - i, 160 + i, LEVEL_HEIGHT - 3,
              TILE_BRICK);
  }

  // Down staircase
  for (int i = 0; i < 8; i++) {
    draw_rect(level, 168 + i, LEVEL_HEIGHT - 11 + i, 168 + i, LEVEL_HEIGHT - 3,
              TILE_BRICK);
  }

  // === SECTION 4: FINAL CHALLENGE & GOAL (x: 180-195) ===

  // Final spike gauntlet
  for (int x = 178; x < 186; x++) {
    level->tiles[LEVEL_HEIGHT - 3][x] = TILE_SPIKE;
  }

  // One-way platforms above
  draw_hline(level, 179, 182, LEVEL_HEIGHT - 6, TILE_PLATFORM);
  draw_hline(level, 181, 184, LEVEL_HEIGHT - 9, TILE_PLATFORM);

  // Victory platform
  draw_hline(level, 188, 195, LEVEL_HEIGHT - 5, TILE_BRICK);

  // GOAL FLAG!
  level->tiles[LEVEL_HEIGHT - 6][192] = TILE_GOAL;
  level->tiles[LEVEL_HEIGHT - 7][192] = TILE_GOAL;
  level->tiles[LEVEL_HEIGHT - 8][192] = TILE_GOAL;

  // Celebratory coins around flag
  for (int x = 189; x < 195; x++) {
    level->tiles[LEVEL_HEIGHT - 9][x] = TILE_COIN;
  }
}

bool level_is_solid(Level *level, int x, int y) {
  if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT) {
    return false;
  }

  TileType tile = level->tiles[y][x];
  return (tile == TILE_GROUND || tile == TILE_BRICK || tile == TILE_PIPE_LEFT ||
          tile == TILE_PIPE_RIGHT || tile == TILE_QUESTION);
}

bool level_is_deadly(Level *level, int x, int y) {
  if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT) {
    return false;
  }

  return level->tiles[y][x] == TILE_SPIKE;
}

bool level_is_platform(Level *level, int x, int y) {
  if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT) {
    return false;
  }

  return level->tiles[y][x] == TILE_PLATFORM;
}

bool level_is_goal(Level *level, int x, int y) {
  if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT) {
    return false;
  }

  return level->tiles[y][x] == TILE_GOAL;
}

bool level_is_coin(Level *level, int x, int y) {
  if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT) {
    return false;
  }

  return level->tiles[y][x] == TILE_COIN;
}

void level_collect_coin(Level *level, int x, int y) {
  if (x >= 0 && x < LEVEL_WIDTH && y >= 0 && y < LEVEL_HEIGHT) {
    if (level->tiles[y][x] == TILE_COIN) {
      level->tiles[y][x] = TILE_EMPTY;
    }
  }
}

TileType level_get_tile(Level *level, int x, int y) {
  if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT) {
    return TILE_EMPTY;
  }
  return level->tiles[y][x];
}
