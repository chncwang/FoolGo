#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <cstdint>
#include <iostream>

#include "board/zob_hasher.h"
#include "def.h"
#include "game/fresh_game.h"
#include "game/game.h"
#include "math/rand.h"
#include "util/log_util.h"

using foolgo::board::FullBoard;
using foolgo::board::ZobHasher;
using foolgo::game::Game;
using foolgo::game::FreshGame;
using foolgo::math::GetTimeSeed;
using foolgo::util::InitLogConfig;
using foolgo::MAIN_BOARD_LEN;
using log4cplus::Logger;

int main(int argc, const char *argv[]) {
  InitLogConfig();

  Logger logger = Logger::getRoot();

//  uint32_t seed = GetTimeSeed();
  uint32_t seed = 3129530396;
  LOG4CPLUS_INFO(logger, "seed:" << seed);

  ZobHasher<MAIN_BOARD_LEN>::Init(seed);

  Game<MAIN_BOARD_LEN>* game =
      FreshGame<MAIN_BOARD_LEN>::BuildAiVsAiGame(seed, 500000, 1, true);
  game->Run();

  delete game;

  return 0;
}
