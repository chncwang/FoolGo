#include <cstdint>
#include <iostream>

#include "board/zob_hasher.h"
#include "def.h"
#include "game/fresh_game.h"
#include "game/game.h"
#include "util/rand.h"

using foolgo::board::FullBoard;
using foolgo::board::ZobHasher;
using foolgo::game::Game;
using foolgo::game::FreshGame;
using foolgo::util::GetTimeSeed;
using foolgo::MAIN_BOARD_LEN;
using std::cout;

int main(int argc, const char *argv[]) {
  uint32_t seed = GetTimeSeed();
//  uint32_t seed = 2479583645;
  cout << "seed:" << seed << std::endl;

  ZobHasher<MAIN_BOARD_LEN>::Init(seed);

  Game<MAIN_BOARD_LEN>* game =
      FreshGame<MAIN_BOARD_LEN>::BuildHumanVsAiGame(false, seed, 100000, 4);
  game->Run();

  delete game;

  return 0;
}
