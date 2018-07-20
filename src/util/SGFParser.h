#ifndef SGFPARSER_H_INCLUDED
#define SGFPARSER_H_INCLUDED

#include <cstddef>
#include <cstdint>
#include <climits>
#include <sstream>
#include <string>
#include <vector>

#include "board/full_board.h"
#include "game/game_info.h"

namespace foolgo {

class SGFParser {
private:
    static std::string parse_property_name(std::istringstream & strm);
    static bool parse_property_value(std::istringstream & strm, std::string & result);
public:
    static std::string chop_from_file(const std::string &fname, size_t index);
    static std::vector<std::string> chop_all(const std::string &fname,
                                             size_t stopat = SIZE_MAX);
    static std::vector<std::string> chop_stream(std::istream& ins,
                                                size_t stopat = SIZE_MAX);
    static std::vector<GameInfo> get_game_infos(const std::string &fname);
};

}


#endif
