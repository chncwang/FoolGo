#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "board/full_board.h"
#include "game/game_info.h"
#include "def.h"
#include "util/cxxopts.hpp"
#include "util/SGFParser.h"

using namespace foolgo;
using std::cout;
using std::endl;
using cxxopts::Options;
using std::string;
using std::vector;

int main(int argc, char *argv[]) {
  Options options("FoolGo", "A montecarlo Go A.I.");
  options.add_options()
    ("s,sgf", "sgf file name", cxxopts::value<string>());
  auto args = options.parse(argc, argv);
  string sgf_file_name = args["sgf"].as<string>();
  cout << "sgf:" << sgf_file_name << endl;

  SGFParser parser;
  vector<string> strs = parser.chop_all(sgf_file_name);
  cout << strs.size() << endl;
  for (string &str : strs) {
    cout << str << endl;
  }
  vector<GameInfo> game_infos = parser.get_game_infos(sgf_file_name);

  for (const GameInfo &info : game_infos) {
    for (const Move &move : info.moves) {
      cout << move.force << " " << move.position_index << std::endl;
    }
  }

  uint32_t seed = GetTimeSeed();
  cout << "seed:" << seed << std::endl;
  ZobHasher<MAIN_BOARD_LEN>::Init(seed);

  return 0;
}
