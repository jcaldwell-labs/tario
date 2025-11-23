/*
 * Tario Test Suite
 * Basic unit tests for game mechanics
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Include game headers (testing internals)
#include "../include/player.h"
#include "../include/level.h"
#include "../include/render.h"

// Test framework macros
#define TEST(name) void test_##name()
#define RUN_TEST(name) do { \
    printf("Running: %s...", #name); \
    test_##name(); \
    printf(" PASSED\n"); \
    tests_passed++; \
} while (0)

#define ASSERT(condition) do { \
    if (!(condition)) { \
        printf("\n  ASSERTION FAILED: %s at %s:%d\n", #condition, __FILE__, __LINE__); \
        exit(1); \
    } \
} while (0)

#define ASSERT_EQ(a, b) ASSERT((a) == (b))
#define ASSERT_FLOAT_EQ(a, b) ASSERT(fabsf((a) - (b)) < 0.001f)

// Global test counter
static int tests_passed = 0;

/*
 * Player Physics Tests
 */

TEST(player_init) {
    Player p;
    player_init(&p, 10.0f, 20.0f);

    ASSERT_FLOAT_EQ(p.x, 10.0f);
    ASSERT_FLOAT_EQ(p.y, 20.0f);
    ASSERT_FLOAT_EQ(p.vel_x, 0.0f);
    ASSERT_FLOAT_EQ(p.vel_y, 0.0f);
    ASSERT_EQ(p.lives, 3);
    ASSERT_EQ(p.is_dead, 0);
    ASSERT_EQ(p.coins_collected, 0);
}

TEST(player_movement) {
    Player p;
    player_init(&p, 0.0f, 0.0f);

    // Test right movement
    player_move_right(&p);
    ASSERT(p.vel_x > 0);
    ASSERT_EQ(p.facing_right, 1);

    // Test left movement
    player_move_left(&p);
    ASSERT(p.vel_x < 0);
    ASSERT_EQ(p.facing_right, 0);
}

TEST(player_jump_mechanics) {
    Player p;
    player_init(&p, 0.0f, 0.0f);

    p.on_ground = 1;

    // Test jump press
    player_jump_press(&p);
    ASSERT(p.jump_held == 1);

    // Test jump release
    player_jump_release(&p);
    ASSERT(p.jump_held == 0);
}

TEST(player_death) {
    Player p;
    player_init(&p, 0.0f, 0.0f);

    ASSERT_EQ(p.lives, 3);

    player_kill(&p);
    ASSERT_EQ(p.is_dead, 1);
    ASSERT_EQ(p.lives, 2);
}

/*
 * Level Tests
 */

TEST(level_init) {
    Level level;
    level_init(&level);

    // Test that level is initialized (basic sanity check)
    // Just check that we can access tiles without crashing
    TileType tile = level_get_tile(&level, 0, 0);
    (void)tile; // Suppress unused variable warning
}

TEST(level_solid_detection) {
    Level level;
    level_init(&level);

    // Find a ground tile and test solid detection
    // Note: This test assumes there's ground at the bottom of the level
    int found_ground = 0;
    for (int y = LEVEL_HEIGHT - 10; y < LEVEL_HEIGHT; y++) {
        for (int x = 0; x < 20; x++) {
            if (level_is_solid(&level, x, y)) {
                found_ground = 1;
                break;
            }
        }
        if (found_ground) break;
    }

    // Basic check: level should have some solid tiles
    ASSERT(found_ground == 1);
}

TEST(level_boundary_checks) {
    Level level;
    level_init(&level);

    // Test out of bounds access (should return false/empty)
    ASSERT_EQ(level_is_solid(&level, -1, 0), 0);
    ASSERT_EQ(level_is_solid(&level, 0, -1), 0);
    ASSERT_EQ(level_is_solid(&level, LEVEL_WIDTH + 1, 0), 0);
    ASSERT_EQ(level_is_solid(&level, 0, LEVEL_HEIGHT + 1), 0);
}

TEST(level_coin_collection) {
    Level level;
    level_init(&level);

    // Find a coin in the level
    int coin_x = -1, coin_y = -1;
    for (int y = 0; y < LEVEL_HEIGHT; y++) {
        for (int x = 0; x < LEVEL_WIDTH; x++) {
            if (level_is_coin(&level, x, y)) {
                coin_x = x;
                coin_y = y;
                break;
            }
        }
        if (coin_x != -1) break;
    }

    // If we found a coin, test collection
    if (coin_x != -1) {
        ASSERT(level_is_coin(&level, coin_x, coin_y) == 1);
        level_collect_coin(&level, coin_x, coin_y);
        ASSERT(level_is_coin(&level, coin_x, coin_y) == 0);
    }
}

TEST(level_deadly_tiles) {
    Level level;
    level_init(&level);

    // Find spike tiles in the level
    int found_spike = 0;
    for (int y = 0; y < LEVEL_HEIGHT; y++) {
        for (int x = 0; x < LEVEL_WIDTH; x++) {
            if (level_is_deadly(&level, x, y)) {
                found_spike = 1;
                break;
            }
        }
        if (found_spike) break;
    }

    // Level should have some hazards
    ASSERT(found_spike == 1);
}

TEST(level_goal_exists) {
    Level level;
    level_init(&level);

    // Find goal flag in the level
    int found_goal = 0;
    for (int y = 0; y < LEVEL_HEIGHT; y++) {
        for (int x = 0; x < LEVEL_WIDTH; x++) {
            if (level_is_goal(&level, x, y)) {
                found_goal = 1;
                break;
            }
        }
        if (found_goal) break;
    }

    // Level must have a goal
    ASSERT(found_goal == 1);
}

/*
 * Render Tests
 */

TEST(screen_buffer_create) {
    ScreenBuffer *sb = screen_buffer_create(80, 24);
    ASSERT(sb != NULL);
    ASSERT_EQ(sb->width, 80);
    ASSERT_EQ(sb->height, 24);
    ASSERT(sb->buffer != NULL);
    screen_buffer_free(sb);
}

TEST(screen_buffer_bounds) {
    ScreenBuffer *sb = screen_buffer_create(10, 10);
    ASSERT(sb != NULL);

    // Test drawing within bounds (should not crash)
    screen_buffer_draw_char(sb, 5, 5, 'X');

    // Test drawing out of bounds (should not crash)
    screen_buffer_draw_char(sb, -1, 0, 'X');
    screen_buffer_draw_char(sb, 0, -1, 'X');
    screen_buffer_draw_char(sb, 100, 0, 'X');
    screen_buffer_draw_char(sb, 0, 100, 'X');

    screen_buffer_free(sb);
}

/*
 * Main test runner
 */

int main(void) {
    printf("\n");
    printf("=================================\n");
    printf("      Tario Test Suite\n");
    printf("=================================\n");
    printf("\n");

    // Player tests
    printf("Player Tests:\n");
    RUN_TEST(player_init);
    RUN_TEST(player_movement);
    RUN_TEST(player_jump_mechanics);
    RUN_TEST(player_death);
    printf("\n");

    // Level tests
    printf("Level Tests:\n");
    RUN_TEST(level_init);
    RUN_TEST(level_solid_detection);
    RUN_TEST(level_boundary_checks);
    RUN_TEST(level_coin_collection);
    RUN_TEST(level_deadly_tiles);
    RUN_TEST(level_goal_exists);
    printf("\n");

    // Render tests
    printf("Render Tests:\n");
    RUN_TEST(screen_buffer_create);
    RUN_TEST(screen_buffer_bounds);
    printf("\n");

    printf("=================================\n");
    printf("  %d tests passed!\n", tests_passed);
    printf("=================================\n");
    printf("\n");

    return 0;
}
