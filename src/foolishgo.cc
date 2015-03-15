#include <cstdint>

#include "board/full_board.h"
#include "game/engine.h"
#include "game/engine-TLT.h"
#include "game/game.h"
#include "game/monte_carlo_game.h"
#include "math/rand.h"
#include "util/log_util.h"

using foolgo::board::FullBoard;
using foolgo::board::ZobHasher;
using foolgo::game::Engine;
using foolgo::game::Game;
using foolgo::game::MonteCarloGame;
using foolgo::math::GetTimeSeed;
using foolgo::util::InitLogConfig;

int main(int argc, const char *argv[]) {
  InitLogConfig();

  uint32_t seed = GetTimeSeed();
  ZobHasher<9> zob_hasher(seed);

  Engine<9> engine(&zob_hasher);
  engine.Init();
  engine.Play(0);
  engine.Pass();
  engine.NextMove();

  FullBoard<9> full_board(&zob_hasher);
  full_board.Init();
  MonteCarloGame<9> monte_carlo_game(full_board, seed);
  monte_carlo_game.Run();

  return 0;
}
