#include <cstdint>

#include "board/zob_hasher.h"
#include "game/fresh_game.h"
#include "math/rand.h"
#include "util/log_util.h"

using foolgo::board::FullBoard;
using foolgo::board::ZobHasher;
using foolgo::game::Game;
using foolgo::game::FreshGame;
using foolgo::math::GetTimeSeed;
using foolgo::util::InitLogConfig;

int main(int argc, const char *argv[]) {
  InitLogConfig();

  uint32_t seed = GetTimeSeed();
  ZobHasher<9>::Init(seed);

  Game<9>* game = FreshGame<9>::BuildHumanVsAiGame(true, seed);

  game->Run();

  delete game;

  return 0;
}
