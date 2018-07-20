#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "board/full_board.h"
#include "game/game_info.h"
#include "game/sgf_game.h"
#include "def.h"
#include "util/cxxopts.hpp"
#include "util/SGFParser.h"
#include "util/rand.h"
#include "deep_learning/sample.h"
#include "deep_learning/engine.h"
#include "deep_learning/cnn/graph_builder.h"
#include "N3LDG.h"

using namespace foolgo;
using namespace std;
using namespace cxxopts;

int main(int argc, char *argv[]) {
  uint32_t seed = GetTimeSeed();
  cout << "seed:" << seed << endl;
  ZobHasher<19>::Init(seed);
  Options options("FoolGo", "A montecarlo Go A.I.");
  options.add_options()
    ("s,sgf", "sgf file name", cxxopts::value<string>());
  auto args = options.parse(argc, argv);
  string sgf_file_name = args["sgf"].as<string>();

  SGFParser parser;
  vector<string> strs = parser.chop_all(sgf_file_name);
  cout << strs.size() << endl;
  vector<GameInfo> game_infos = parser.get_game_infos(sgf_file_name);

  int iter = 0;
  cout << "game count:" << game_infos.size() << endl;
  vector<Sample<19>> samples;

  for (int i=0; i < 1000; ++i) {
    int rand_game_i = Rand(game_infos.size() - 1, seed);
    const GameInfo &game_info = game_infos.at(rand_game_i);
    vector<Sample<19>> single_game_samples;
    auto game = SgfGame<19>::BuildSgfGame(game_info, &single_game_samples);
    game->Run();
    int rand_sample_i = Rand(game_info.moves.size() - 1, seed);
    samples.push_back(single_game_samples.at(rand_sample_i));
  }

  cout << "samples count:" << samples.size() << endl;

  Engine engine;

  return 0;
}
