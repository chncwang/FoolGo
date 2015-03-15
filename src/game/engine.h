#ifndef FOOLGO_SRC_GAME_ENGINE_H_
#define FOOLGO_SRC_GAME_ENGINE_H_

#include <stdint.h>
#include <unordered_map>

#include "../board/force.h"
#include "../board/full_board.h"
#include "../board/pos_cal.h"
#include "../board/position.h"
#include "../def.h"
#include "../player/mc_simulator.h"

namespace foolgo {
namespace game {

template<board::BoardLen BOARD_LEN>
class Engine {
 public:
  Engine(const board::ZobHasher<BOARD_LEN> *zob_hasher_ptr)
      : next_player_(board::BLACK_FORCE),
        brdingm_(zob_hasher_ptr),
        zob_hasher_ptr_(zob_hasher_ptr) {}
  ~Engine() = default;
  DISALLOW_COPY_AND_ASSIGN_AND_MOVE(Engine)
  void Init();

  void Play(board::PositionIndex nexti);
  void Pass();
  board::PositionIndex NextMove() const;
 private:
  struct TableItem {
    int32_t visited_times_;
    float avg_prft_;
    HashKey children_key_[board::BoardLenSquare<BOARD_LEN>()];

    TableItem(int32_t t, float a)
        : visited_times_(t),
          avg_prft_(a) {
      Init();
    }
    TableItem() {
      Init();
    }

    void Init() {
      std::fill(children_key_,
                children_key_ + board::BoardLenSquare<BOARD_LEN>(), NONE);
    }
  };
  typedef std::unordered_map<HashKey, TableItem> TrnspsitionTbl;

  static const int SIMULATE_COUNT = 30000;
  static const HashKey NONE;

  board::FullBoard<BOARD_LEN> brdingm_;
  board::Force next_player_;
  const board::ZobHasher<BOARD_LEN> *zob_hasher_ptr_;
  mutable TrnspsitionTbl table_;

  HashKey ChildKey(const board::FullBoard<BOARD_LEN> &parent,
                   board::PositionIndex indx) const;
  bool HasNewChild(const board::FullBoard<BOARD_LEN> &node,
                   board::PositionIndex *nexti) const;
  board::PositionIndex MaxUCBChild(const board::FullBoard<BOARD_LEN> &node) const;
  board::PositionIndex BestChild(const board::FullBoard<BOARD_LEN> &node) const;

  inline player::MCSimulator<BOARD_LEN> &Simulator() const {
    return player::MCSimulator<BOARD_LEN>::Ins();
  }
  inline board::PstionAndIndxCcltr<BOARD_LEN> &GetPosClcltr() const {
    return board::PstionAndIndxCcltr<BOARD_LEN>::Ins();
  }
};

template<board::BoardLen BOARD_LEN>
const HashKey Engine<BOARD_LEN>::NONE = 0;

}
}

#endif
