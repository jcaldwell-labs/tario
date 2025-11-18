#include "level.h"
#include <string.h>

void level_init(Level *level) {
    // Clear all tiles
    memset(level->tiles, TILE_EMPTY, sizeof(level->tiles));

    // Create ground at bottom
    for (int x = 0; x < LEVEL_WIDTH; x++) {
        level->tiles[LEVEL_HEIGHT - 1][x] = TILE_GROUND;
        level->tiles[LEVEL_HEIGHT - 2][x] = TILE_GROUND;
    }

    // Add some platforms
    // Platform 1
    for (int x = 10; x < 20; x++) {
        level->tiles[LEVEL_HEIGHT - 7][x] = TILE_BRICK;
    }

    // Platform 2
    for (int x = 25; x < 35; x++) {
        level->tiles[LEVEL_HEIGHT - 10][x] = TILE_BRICK;
    }

    // Platform 3
    for (int x = 40; x < 50; x++) {
        level->tiles[LEVEL_HEIGHT - 7][x] = TILE_BRICK;
    }

    // Platform 4 (higher)
    for (int x = 55; x < 65; x++) {
        level->tiles[LEVEL_HEIGHT - 13][x] = TILE_BRICK;
    }

    // Add some coins
    for (int x = 12; x < 18; x++) {
        level->tiles[LEVEL_HEIGHT - 8][x] = TILE_COIN;
    }
    for (int x = 27; x < 33; x++) {
        level->tiles[LEVEL_HEIGHT - 11][x] = TILE_COIN;
    }
}

bool level_is_solid(Level *level, int x, int y) {
    if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT) {
        return false;
    }

    TileType tile = level->tiles[y][x];
    return (tile == TILE_GROUND || tile == TILE_BRICK);
}

TileType level_get_tile(Level *level, int x, int y) {
    if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT) {
        return TILE_EMPTY;
    }
    return level->tiles[y][x];
}
