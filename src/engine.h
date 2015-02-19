#ifndef ENGINE
#define ENGINE

#include <board_in_gm.h>
#include <def.h>
#include <mc_simulator.h>
#include <pos_cal.h>
#include <position.h>
#include <stdint.h>
#include <types_and_constants.h>
#include <unordered_map>

template<BoardLen BOARD_LEN>
class Engine {
 public:
  Engine() : next_player_(BLACK_PLAYER) {}
  ~Engine() = default;
  DISALLOW_COPY_AND_ASSIGN(Engine)
  void Init();

  void Play(PointIndex nexti);
  void Pass();
  PointIndex NextMove() const;
 private:
  struct TableItem {
    int32_t visited_times_;
    float avg_prft_;
    HashKeyType children_key_[BoardLenSquare<BOARD_LEN>()];

    TableItem(int32_t t, float a)
        : visited_times_(t),
          avg_prft_(a) {
      Init();
    }
    TableItem() {
      Init();
    }

    void Init() {
      std::fill(children_key_, children_key_ + BoardLenSquare<BOARD_LEN>(),
                NONE);
    }
  };
  typedef std::unordered_map<HashKeyType, TableItem> TrnspsitionTbl;

  static const int SIMULATE_COUNT = 30000;
  static const HashKeyType NONE = 0;

  BoardInGm<BOARD_LEN> brdingm_;
  PlayerColor next_player_;
  mutable TrnspsitionTbl table_;

  HashKeyType ChildKey(const BoardInGm<BOARD_LEN> &parent,
                       PointIndex indx) const;
  bool HasNewChild(const BoardInGm<BOARD_LEN> &node, PointIndex *nexti) const;
  PointIndex MaxUCBChild(const BoardInGm<BOARD_LEN> &node) const;
  PointIndex BestChild(const BoardInGm<BOARD_LEN> &node) const;

  inline MCSimulator<BOARD_LEN> &Simulator() const {
    return MCSimulator<BOARD_LEN>::Ins();
  }
  inline PosCalculator<BOARD_LEN> &GetPosClcltr() const {
    return PosCalculator<BOARD_LEN>::Ins();
  }
};

#include "engine-TLT.h"

#endif
