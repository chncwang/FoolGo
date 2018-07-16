#include <cstdint>
#include <iostream>

#include "board/zob_hasher.h"
#include "def.h"
#include "game/fresh_game.h"
#include "game/game.h"
#include "util/rand.h"

using namespace foolgo;
using std::cout;

int main(int argc, const char *argv[]) {
  uint32_t seed = GetTimeSeed();
//  uint32_t seed = 2479583645;
  cout << "seed:" << seed << std::endl;

  ZobHasher<MAIN_BOARD_LEN>::Init(seed);

  auto game = FreshGame<MAIN_BOARD_LEN>::BuildHumanVsAiGame(false, seed, 10000,
      4);
  game->Run();

  return 0;
}
