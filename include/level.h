#ifndef LEVEL_H
#define LEVEL_H

#include <stdbool.h>

#define LEVEL_WIDTH 200
#define LEVEL_HEIGHT 50

typedef enum {
  TILE_EMPTY = ' ',
  TILE_GROUND = '#',
  TILE_BRICK = '=',
  TILE_COIN = 'o',
  TILE_SPIKE = '^',
  TILE_GOAL = 'F',     // Flag
  TILE_PLATFORM = '-', // One-way platform (can jump through from below)
  TILE_QUESTION = '?', // Question block (can be hit from below for coin)
  TILE_PIPE_LEFT = '[',
  TILE_PIPE_RIGHT = ']'
} TileType;

typedef struct {
  TileType tiles[LEVEL_HEIGHT][LEVEL_WIDTH];
} Level;

// Initialize a test level
void level_init(Level *level);

// Check if a tile is solid
bool level_is_solid(Level *level, int x, int y);

// Check if a tile is deadly
bool level_is_deadly(Level *level, int x, int y);

// Check if a tile is a platform (one-way)
bool level_is_platform(Level *level, int x, int y);

// Check if a tile is the goal
bool level_is_goal(Level *level, int x, int y);

// Check if a tile is a coin
bool level_is_coin(Level *level, int x, int y);

// Collect a coin at position
void level_collect_coin(Level *level, int x, int y);

// Get tile at position
TileType level_get_tile(Level *level, int x, int y);

#endif
