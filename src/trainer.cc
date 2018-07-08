#include <cstdint>
#include <iostream>
#include <string>

#include "board/full_board.h"
#include "def.h"
#include "util/cxxopts.hpp"
#include "util/SGFParser.h"

using foolgo::board::ZobHasher;
using foolgo::util::GetTimeSeed;
using foolgo::MAIN_BOARD_LEN;
using std::cout;
using std::endl;
using cxxopts::Options;
using std::string;

int main(int argc, char *argv[]) {
  Options options("FoolGo", "A montecarlo Go A.I.");
  options.add_options()
    ("s,sgf", "sgf file name", cxxopts::value<string>());
  auto args = options.parse(argc, argv);
  string sgf_file_name = args["sgf"].as<string>();
  cout << "sgf:" << sgf_file_name << endl;

  uint32_t seed = GetTimeSeed();
  cout << "seed:" << seed << std::endl;
  ZobHasher<MAIN_BOARD_LEN>::Init(seed);

  return 0;
}
