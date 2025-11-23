#include "game.h"
#include <signal.h>
#include <stdio.h>

static Game *g_game = NULL;

void signal_handler(int signum) {
  (void)signum; // Unused parameter
  if (g_game) {
    g_game->running = false;
  }
}

int main(void) {
  Game game;
  g_game = &game;

  // Set up signal handlers for clean exit
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  if (game_init(&game) != 0) {
    fprintf(stderr, "Failed to initialize game\n");
    return 1;
  }

  game_run(&game);
  game_cleanup(&game);

  return 0;
}
