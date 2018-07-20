#ifndef FOOLGO_SRC_GAME_SGF_GAME_H_
#define FOOLGO_SRC_GAME_SGF_GAME_H_

#include <cstdint>
#include <memory>
#include <functional>
#include <vector>

#include "board/position.h"
#include "def.h"
#include "player/player.h"
#include "player/sgf_player.h"
#include "deep_learning/sample.h"
#include "game.h"

namespace foolgo {

template<BoardLen BOARD_LEN>
class SgfGame : public Game<BOARD_LEN> {
 public:
  SgfGame(const GameInfo &game_info) {
    game_info_ = game_info;
  }

  ~SgfGame() = default;

  static std::unique_ptr<SgfGame> BuildSgfGame(const GameInfo &game_info,
      std::vector<Sample<BOARD_LEN>> *samples = nullptr) {
    SgfPlayer<BOARD_LEN> *black_player = new SgfPlayer<BOARD_LEN>(game_info);
    SgfPlayer<BOARD_LEN> *white_player = new SgfPlayer<BOARD_LEN>(game_info);
    FullBoard<BOARD_LEN> full_board;
    full_board.Init();

    return std::unique_ptr<SgfGame>(new SgfGame<BOARD_LEN>(
          full_board, black_player, white_player, game_info, samples));
  }

 protected:
  SgfGame(const FullBoard<BOARD_LEN> &full_board,
       Player<BOARD_LEN> *black_player,
       Player<BOARD_LEN> *white_player,
       const GameInfo &game_info,
       std::vector<Sample<BOARD_LEN>> *samples,
       bool only_log_board = true) : Game<BOARD_LEN>(full_board, black_player,
           white_player, only_log_board), game_info_(game_info),
           samples_(samples) {}

  bool ShouldLog() const override {
    return false;
  }

  void BeforePlay (PositionIndex index) override {
    if (samples_ != nullptr) {
      Sample<BOARD_LEN> sample;
      sample.full_board.Copy(this->GetFullBoard());
      sample.position_index = index;
      samples_->push_back(sample);
    }
  }

 private:
  DISALLOW_COPY_AND_ASSIGN_AND_MOVE(SgfGame);
  GameInfo game_info_;
  std::vector<Sample<BOARD_LEN>> *samples_ = nullptr;
};

}

#endif
