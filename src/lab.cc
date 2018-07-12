#include <cstdint>
#include <cstdlib>
#include <iostream>

#include "board/zob_hasher.h"
#include "def.h"
#include "game/fresh_game.h"
#include "game/game.h"
#include "util/rand.h"
#include "player/random_player.h"
#include "player/uct_player.h"

using namespace foolgo;
using namespace std;

int main(int argc, const char *argv[]) {
  uint32_t seed = GetTimeSeed();
//  uint32_t seed = 2479583645;
  cout << "seed:" << seed << std::endl;
  srand(seed);

  ZobHasher<MAIN_BOARD_LEN>::Init(seed);

  Player<MAIN_BOARD_LEN> *black_player = new RandomPlayer<MAIN_BOARD_LEN>(seed);
  cout << "black_player:" << black_player << endl;

  Player<MAIN_BOARD_LEN> *white_player =
      new UctPlayer<MAIN_BOARD_LEN>(seed, 1000, 1);

  auto game =
      FreshGame<MAIN_BOARD_LEN>::BuildFreshGame(black_player, white_player);
  game->Run();

  return 0;
}
