#ifndef LEVEL_H
#define LEVEL_H

#include <stdbool.h>

#define LEVEL_WIDTH 80
#define LEVEL_HEIGHT 24

typedef enum {
    TILE_EMPTY = ' ',
    TILE_GROUND = '#',
    TILE_BRICK = '=',
    TILE_COIN = 'o'
} TileType;

typedef struct {
    TileType tiles[LEVEL_HEIGHT][LEVEL_WIDTH];
} Level;

// Initialize a test level
void level_init(Level *level);

// Check if a tile is solid
bool level_is_solid(Level *level, int x, int y);

// Get tile at position
TileType level_get_tile(Level *level, int x, int y);

#endif
