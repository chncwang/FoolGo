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
#include "deep_learning/sample.h"
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
  cout << "sgf:" << sgf_file_name << endl;

  SGFParser parser;
  vector<string> strs = parser.chop_all(sgf_file_name);
  cout << strs.size() << endl;
  vector<GameInfo> game_infos = parser.get_game_infos(sgf_file_name);
  vector<Sample> samples;
  auto collect_samples = [&](const Sample &sample) {
  }

  for (const GameInfo &game_info : game_infos) {
    auto sgf_game = SgfGame<19>::BuildSgfGame(game_info, collect_samples);
    sgf_game->Run();
  }

  return 0;
}
