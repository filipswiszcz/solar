#include "g_game.h"

int main(int argc, char **argv) {
  g_game_init();
  g_game_loop();
  g_game_stop();
  return 0;
}
