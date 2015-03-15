#ifndef FOOLGO_SRC_BOARD_FULL_BOARD_H_
#define FOOLGO_SRC_BOARD_FULL_BOARD_H_

#include <functional>
#include <bitset>
#include <cassert>
#include <cstddef>
#include <ostream>
#include <vector>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

#include "../def.h"
#include "../math/bitset_util.h"
#include "../math/vector_util.h"
#include "../piece_structure/chain_set.h"
#include "../piece_structure/eye_set.h"
#include "board.h"
#include "board_difference.h"
#include "force.h"
#include "full_board_hasher.h"
#include "pos_cal.h"
#include "position.h"
#include "zob_hasher.h"

namespace foolgo {
namespace board {

const PositionIndex POSITION_INDEX_PASS = -1;

struct Move {
  Force force;
  PositionIndex position_index;

  Move() : force(Force::BLACK_FORCE), position_index(0) {}
  Move(Force force, PositionIndex indx) : force(force), position_index(indx) {}
};

typedef Move ForceAndPositionIndex;

template<BoardLen BOARD_LEN>
std::string ForceAndPositionIndexToString(
    const ForceAndPositionIndex &force_and_position_index) {
  auto &calculator = PstionAndIndxCcltr<BOARD_LEN>::Ins();
  const Position &position = calculator.GetPosition(
      force_and_position_index.position_index);
  std::string position_str = PositionToString(position);
  std::string force_str = ForceToString(force_and_position_index.force);
  return (boost::format("{force:%1%, position:%2%}") % force_str % position_str)
      .str();
}

template<BoardLen BOARD_LEN>
class FullBoard : private Board<BOARD_LEN> {
 public:
  typedef std::bitset<BoardLenSquare<BOARD_LEN>()> BitSet;

  static const PositionIndex NONE = -1;

  explicit FullBoard(const ZobHasher<BOARD_LEN> *zob_hasher_ptr)
      : ko_indx_(-1),
        last_force_(WHITE_FORCE),
        black_pieces_count_(0),
        full_board_hasher_(zob_hasher_ptr),
        hash_key_(0) {}
  ~FullBoard() = default;
  void Init();
  void Copy(const FullBoard &b);

  PointState GetPointState(PositionIndex indx) const {
    return Board<BOARD_LEN>::GetPoint(indx);
  }
  PointState GetPointState(const Position &pos) const {
    return Board<BOARD_LEN>::GetPoint(pos);
  }
  Force LastForce() const {
    return last_force_;
  }
  PositionIndex KoIndex() const {
    return ko_indx_;
  }
  const BitSet &PlayableIndexes(Force color) const {
    return point_playable_states_array_[color];
  }
  PositionIndex BlackRegion() const {
    return black_pieces_count_ + eye_states_[BLACK_FORCE].RealCount();
  }
  HashKey HashKey() const {
    return hash_key_;
  }

  void PlayMove(const Move &move);
  void Pass(Force force);

  std::string ToString(
      std::function<std::string(const Board<BOARD_LEN>&)> board_to_string) const;
  std::string ToString(const PositionIndex &last_move_index) const;
  std::string ToString() const;

 private:
  typedef std::vector<PositionIndex> PointIndxVector;

  static log4cplus::Logger logger_;

  piece_structure::ChainSet<BOARD_LEN> chain_sets_[2];
  BitSet point_playable_states_array_[2];
  piece_structure::EyeSet<BOARD_LEN> eye_states_[2];
  PositionIndex ko_indx_;
  Force last_force_;
  PositionIndex black_pieces_count_;
  const FullBoardHasher<BOARD_LEN> *full_board_hasher_;
  foolgo::HashKey hash_key_;

  void SetSpecifiedAirForAdjacentChains(PositionIndex indx, bool v);

  bool IsSelfPieceOrEye(const Move &force_and_position_index) const;

  bool IsEmptySingly(PositionIndex indx) const;
  bool IsSuicide(const Move &move) const;
  bool IsPlayable(const Move &move) const;

  /**
   * Execute basic operations for the move. The basic operations area as below:
   * 1) Modify the Board object to complete the move.
   * 2) Modify chain sets.
   */
  void PlayBasicMove(const Move &move, PointIndxVector *ate_piecies_indexes,
                     PointIndxVector *suicided_pieces_indexes);
  PointIndxVector RemoveChain(const Move &move);

  void ModifyEyesStateAndObliqueRealEyesState(
      const ForceAndPositionIndex &force_and_position_index);
  void ModifyRealEyesState(
      const ForceAndPositionIndex &force_and_position_index);

  void ModifyAdjacentIndexesPlayableStateOfChain(PositionIndex indx);
  void ModifyAdjacentIndexesPlayableState(const Move &move);
  void ModifyAtePiecesAdjacentChains(const PointIndxVector &ate_pieces,
                                     Force ate_force);

  friend std::ostream &operator <<(std::ostream &os,
                                   const FullBoard<BOARD_LEN> &full_board) {
    return os << full_board.ToString();
  }

  std::string PlayableStatesToString() const;

  DISALLOW_COPY_AND_ASSIGN_AND_MOVE(FullBoard)
};

template<BoardLen BOARD_LEN>
log4cplus::Logger FullBoard<BOARD_LEN>::logger_ =
    log4cplus::Logger::getInstance("foolgo.board.FullBoard");

template<BoardLen BOARD_LEN>
typename FullBoard<BOARD_LEN>::BitSet PlayableIndexesConsideringKo(
    const FullBoard<BOARD_LEN> &b, Force color) {
  if (b.KoIndex() == FullBoard<BOARD_LEN>::NONE) {
    return b.PlayableIndexes(color);
  } else {
    typename FullBoard<BOARD_LEN>::BitSet kobits;
    kobits.set();
    kobits.reset(b.KoIndex());
    return kobits & b.PlayableIndexes(color);
  }
}

template<BoardLen BOARD_LEN>
inline bool IsEnd(const FullBoard<BOARD_LEN> &b) {
  return b.PlayableIndexes(BLACK_FORCE).count() == 0
      && b.PlayableIndexes(WHITE_FORCE).count() == 0;
}

template<BoardLen BOARD_LEN>
inline Force NextForce(const FullBoard<BOARD_LEN> &b) {
  return OppositeForce(b.LastForce());
}

template<BoardLen BOARD_LEN>
void FullBoard<BOARD_LEN>::Init() {
  Board<BOARD_LEN>::Init();
  for (int i = 0; i < 2; ++i) {
    point_playable_states_array_[i].set();
  }
  hash_key_ = full_board_hasher_->GetHash(*this);
}

template<BoardLen BOARD_LEN>
void FullBoard<BOARD_LEN>::Copy(const FullBoard &b) {
  Board<BOARD_LEN>::Copy(b);

  ko_indx_ = b.ko_indx_;
  last_force_ = b.last_force_;
  black_pieces_count_ = b.black_pieces_count_;
  hash_key_ = b.hash_key_;
  full_board_hasher_ = b.full_board_hasher_;

  for (int i = 0; i < 2; ++i) {
    point_playable_states_array_[i] = b.point_playable_states_array_[i];
    eye_states_[i].Copy(b.eye_states_[i]);
    chain_sets_[i].Copy(b.chain_sets_[i]);
  }
}

template<BoardLen BOARD_LEN>
void FullBoard<BOARD_LEN>::PlayMove(const Move &move) {
  Force move_force = move.force;
  PositionIndex move_index = move.position_index;

  assert(PstionAndIndxCcltr<BOARD_LEN>::Ins().IsInBoard(move_index));
  assert(GetPointState(move_index) == EMPTY_POINT);

  BoardDifference board_difference;
  board_difference.Init(LastForce(), KoIndex());
  black_pieces_count_ += OppositeForce(move_force);

  for (int i = 0; i < 2; ++i) {
    point_playable_states_array_[i].reset(move_index);
    eye_states_[i].SetEye(move_index, false);
    eye_states_[i].SetRealEye(move_index, false);
  }

  Force opposite_force = OppositeForce(move_force);
  auto &calculator = PstionAndIndxCcltr<BOARD_LEN>::Ins();
  const Position &move_position = calculator.GetPosition(move_index);
  typename FullBoard<BOARD_LEN>::PointIndxVector ate_piece_indexes_array[4],
      suisided_piece_indexes;

  PlayBasicMove(move, ate_piece_indexes_array, &suisided_piece_indexes);

  if (suisided_piece_indexes.size() != 1) {
    for (int i = 0; i < 2; ++i) {
      point_playable_states_array_[i].reset(move_index);
      eye_states_[i].SetEye(move_index, false);
      eye_states_[i].SetRealEye(move_index, false);
    }

    // Modify adjacent eyes state.
    // Modify eyes state which is oblique to adjacent eyes.
    for (int i = 0; i < 4; ++i) {
      Position adjacent_position = AdjacentPosition(move_position, i);
      if (!calculator.IsInBoard(adjacent_position)) {
        continue;
      }
      if (GetPointState(adjacent_position) != EMPTY_POINT) {
        continue;
      }

      PositionIndex adjacent_index = calculator.GetIndex(adjacent_position);
      ModifyEyesStateAndObliqueRealEyesState(
          ForceAndPositionIndex(move_force, adjacent_index));
    }

    // Modify adjacent real eyes state.
    // If it is a fake eye, modify the playable state.
    for (int i = 0; i < 4; ++i) {
      Position adjacent_position = AdjacentPosition(move_position, i);
      if (!calculator.IsInBoard(adjacent_position)) {
        continue;
      }
      if (GetPointState(adjacent_position) != EMPTY_POINT) {
        continue;
      }

      PositionIndex adjacent_indx = calculator.GetIndex(adjacent_position);
      ForceAndPositionIndex adjcnt_frce_and_indx(move_force, adjacent_indx);
      ModifyRealEyesState(adjcnt_frce_and_indx);
      if (IsFakeEye(eye_states_[adjcnt_frce_and_indx.force],
                    adjcnt_frce_and_indx.position_index)) {
        point_playable_states_array_[opposite_force][adjacent_indx] = IsSuicide(
            Move(opposite_force, adjacent_indx));
      }
    }

    LOG4CPLUS_DEBUG(
        logger_,
        "After modify adjacent real eyes state - PlayableStatesToString:" <<
        PlayableStatesToString());

    // Modify Oblique real eyes state.
    for (int i = 0; i < 4; ++i) {
      Position oblq_pos = ObliquePosition(move_position, i);
      if (!calculator.IsInBoard(oblq_pos)) {
        continue;
      }
      if (GetPointState(oblq_pos) != EMPTY_POINT) {
        continue;
      }

      PositionIndex oblq_indx = calculator.GetIndex(oblq_pos);
      Move oblq_move(move_force, oblq_indx);
      ModifyRealEyesState(oblq_move);
    }

    LOG4CPLUS_DEBUG(
        logger_,
        "After modify oblique real eyes state - PlayableStatesToString:" <<
        PlayableStatesToString());

    ModifyAdjacentIndexesPlayableState(move);

    // The playable state of adjacent positions of the chains, which is adjacent
    // to ate pieces, should be modified.
    for (int i = 0; i < 4; ++i) {
      ModifyAtePiecesAdjacentChains(ate_piece_indexes_array[i], opposite_force);
    }

    ko_indx_ = FullBoard<BOARD_LEN>::NONE;
    PositionIndex single_ate_piece_index = FullBoard<BOARD_LEN>::NONE;

    for (int i = 0; i < 4; ++i) {
      const auto & ate_piece_indexes = ate_piece_indexes_array[i];
      if (ate_piece_indexes.size() == 1) {
        single_ate_piece_index = ate_piece_indexes[0];
      }
    }

    if (single_ate_piece_index != FullBoard<BOARD_LEN>::NONE
        && GetPointState(move_index) == move_force
        && chain_sets_[move_force].GetAirCount(move_index) == 1) {
      ko_indx_ = single_ate_piece_index;
    }
  }

  last_force_ = move_force;

  if (GetPointState(move_index) == EMPTY_POINT) {
    board_difference.ModifyToCurrentState(ko_indx_, move_index, true,
                                          suisided_piece_indexes);
  } else {
    auto ates = math::ConcatVectors(ate_piece_indexes_array);
    board_difference.ModifyToCurrentState(ko_indx_, move_index, false, ates);
  }

  hash_key_ = full_board_hasher_->GetHash(hash_key_, board_difference);
}

template<BoardLen BOARD_LEN>
void FullBoard<BOARD_LEN>::Pass(Force force) {
  last_force_ = force;
  ko_indx_ = FullBoard<BOARD_LEN>::NONE;
  hash_key_ = full_board_hasher_->GetHash(*this);
}

template<BoardLen BOARD_LEN>
std::string FullBoard<BOARD_LEN>::ToString(
    std::function<std::string(const Board<BOARD_LEN>&)> board_to_string) const {
  std::string result = board_to_string(
      static_cast<const Board<BOARD_LEN>&>(*this));

  result += PlayableStatesToString();

  return result;
}

template<BoardLen BOARD_LEN>
std::string FullBoard<BOARD_LEN>::ToString(
    const PositionIndex &last_move_index) const {
  auto get_output =
      [last_move_index](PointState point_state, PositionIndex position_index) {
        return GetPointStateOutput(point_state, last_move_index == position_index);
      };
  auto board_to_string =
      [this, get_output](const Board<BOARD_LEN> &board) {
        return board::ToString(static_cast<const Board<BOARD_LEN>&>(*this), get_output);
      };
  return ToString(board_to_string);
}

template<BoardLen BOARD_LEN>
std::string FullBoard<BOARD_LEN>::ToString() const {
  auto board_to_string = [this](const Board<BOARD_LEN> &board) {
    return board::ToString(board);
  };
  return ToString(board_to_string);
}

template<BoardLen BOARD_LEN>
void FullBoard<BOARD_LEN>::SetSpecifiedAirForAdjacentChains(PositionIndex indx,
                                                            bool v) {
  auto &ins = PstionAndIndxCcltr<BOARD_LEN>::Ins();
  const Position &pos = ins.GetPosition(indx);

  for (int i = 0; i < 4; ++i) {
    Position adj_pos = AdjacentPosition(pos, i);
    if (!ins.IsInBoard(adj_pos)) {
      continue;
    }

    PositionIndex adj_i = ins.GetIndex(adj_pos);
    PointState pnt = GetPointState(adj_i);
    if (pnt == EMPTY_POINT) {
      continue;
    }

    chain_sets_[pnt].SetAir(adj_i, indx, v);
  }
}

template<BoardLen BOARD_LEN>
bool FullBoard<BOARD_LEN>::IsSelfPieceOrEye(
    const ForceAndPositionIndex &force_and_position_index) const {
  PositionIndex indx = force_and_position_index.position_index;
  Force color = force_and_position_index.force;
  return GetPointState(indx) == color || eye_states_[color].IsEye(indx);
}

template<BoardLen BOARD_LEN>
bool FullBoard<BOARD_LEN>::IsEmptySingly(PositionIndex indx) const {
  auto &ins = this->GetPosClcltr();
  const Position &pos = ins.GetPos(indx);

  for (int i = 0; i < 4; ++i) {
    Position adj_pos = AdjacentPosition(pos, i);
    if (!ins.IsInBoard(adj_pos)) {
      continue;
    }

    if (GetPointState(adj_pos) == EMPTY_POINT) {
      return false;
    }
  }

  return true;
}

template<BoardLen BOARD_LEN>
bool FullBoard<BOARD_LEN>::IsSuicide(const Move &move) const {
  assert(GetPointState(move.position_index) == EMPTY_POINT);

  auto &pc = PstionAndIndxCcltr<BOARD_LEN>::Ins();
  PositionIndex indx = move.position_index;
  const Position &pos = pc.GetPosition(indx);
  Force color = move.force;
  Force oc = OppositeForce(color);

  for (int i = 0; i < 4; ++i) {
    Position adj_pos = AdjacentPosition(pos, i);
    if (!pc.IsInBoard(adj_pos)) {
      continue;
    }

    PointState point = GetPointState(adj_pos);
    PositionIndex adj_indx = pc.GetIndex(adj_pos);
    if (point == EMPTY_POINT) {
      return false;
    } else if (point == color) {
      if (chain_sets_[color].GetAirCount(adj_indx) > 1) {
        return false;
      }
    } else {
      if (chain_sets_[oc].GetAirCount(adj_indx) == 1) {
        return false;
      }
    }
  }

  return true;
}

template<BoardLen BOARD_LEN>
bool FullBoard<BOARD_LEN>::IsPlayable(const Move &move) const {
  return point_playable_states_array_[move.force][move.position_index];
}

template<BoardLen BOARD_LEN>
void FullBoard<BOARD_LEN>::PlayBasicMove(
    const Move &move,
    typename FullBoard<BOARD_LEN>::PointIndxVector *ate_piecies_indexes,
    typename FullBoard<BOARD_LEN>::PointIndxVector *suicided_pieces_indexes) {
  assert(ate_piecies_indexes != nullptr);
  Force move_force = move.force;
  PositionIndex move_index = move.position_index;
  assert(GetPointState(move_index) == EMPTY_POINT);

  std::bitset<BoardLenSquare<BOARD_LEN>()> air_set;
  Force opposite_force = OppositeForce(move_force);
  auto &calculator = PstionAndIndxCcltr<BOARD_LEN>::Ins();
  const Position &move_position = calculator.GetPosition(move_index);

  for (int i = 0; i < 4; ++i) {
    Position adj_pos = AdjacentPosition(move_position, i);
    if (!calculator.IsInBoard(adj_pos)) {
      continue;
    }

    PositionIndex adjacent_index = calculator.GetIndex(adj_pos);
    if (GetPointState(adjacent_index) == opposite_force
        && chain_sets_[opposite_force].GetAirCount(adjacent_index) == 1) {
      ate_piecies_indexes[i] = RemoveChain(Move(opposite_force, adjacent_index));
    } else if (GetPointState(adj_pos) == EMPTY_POINT) {
      air_set.set(adjacent_index);
    }
  }

  Board<BOARD_LEN>::SetPoint(move_index, move_force);
  SetSpecifiedAirForAdjacentChains(move_index, false);
  chain_sets_[move_force].AddPiece(move_index, air_set);

  if (chain_sets_[move_force].GetAirCount(move_index) == 0) {
    *suicided_pieces_indexes = RemoveChain(move);
  }
}

template<BoardLen BOARD_LEN>
typename FullBoard<BOARD_LEN>::PointIndxVector FullBoard<BOARD_LEN>::RemoveChain(
    const Move &move) {
  auto chain_set_ptr = chain_sets_ + move.force;
  auto chain_set_pieces = chain_set_ptr->GetPieces(move.position_index);
  chain_sets_[move.force].RemoveListByPiece(move.position_index);

  for (PositionIndex indx : chain_set_pieces) {
    Board<BOARD_LEN>::SetPoint(indx, EMPTY_POINT);
    for (int i = 0; i < 2; ++i) {
      point_playable_states_array_[i].set(indx);
    }
  }

  for (PositionIndex indx : chain_set_pieces) {
    SetSpecifiedAirForAdjacentChains(indx, true);
  }

  if (move.force == Force::BLACK_FORCE) {
    black_pieces_count_ -= chain_set_pieces.size();
  }
  return chain_set_pieces;
}

template<BoardLen BOARD_LEN>
void FullBoard<BOARD_LEN>::ModifyEyesStateAndObliqueRealEyesState(
    const ForceAndPositionIndex &force_and_position_index) {
  assert(GetPointState(force_and_position_index.position_index) == EMPTY_POINT);
  const auto &calculator = PstionAndIndxCcltr<BOARD_LEN>::Ins();
  PositionIndex indx = force_and_position_index.position_index;
  const Position &position = calculator.GetPosition(indx);
  Force force = force_and_position_index.force;

  for (int i = 0; i < 4; ++i) {
    const Position adjacent_position = AdjacentPosition(position, i);
    if (!calculator.IsInBoard(adjacent_position)) {
      continue;
    } else if (GetPointState(AdjacentPosition(position, i))
        != force) {
      eye_states_[force].SetEye(indx, false);
      return;
    }
  }

  eye_states_[force].SetEye(indx, true);

  for (int i = 0; i < 4; ++i) {
    const Position oblq_position = ObliquePosition(position, i);
    if (!calculator.IsInBoard(oblq_position)) {
      continue;
    }

    PositionIndex oblq_indx = calculator.GetIndex(oblq_position);
    if (GetPointState(oblq_indx) == EMPTY_POINT) {
      ModifyRealEyesState(ForceAndPositionIndex(force, oblq_indx));
    }
  }
}

template<BoardLen BOARD_LEN>
void FullBoard<BOARD_LEN>::ModifyRealEyesState(
    const ForceAndPositionIndex &force_and_position_index) {
  assert(GetPointState(force_and_position_index.position_index) == EMPTY_POINT);

  Force force = force_and_position_index.force;
  PositionIndex indx = force_and_position_index.position_index;
  bool is_eye = eye_states_[force].IsEye(indx);
  std::string force_and_position_index_str = ForceAndPositionIndexToString<
      BOARD_LEN>(force_and_position_index);
  LOG4CPLUS_DEBUG(
      logger_,
      "is_eye:" << is_eye << ", force_and_position_index:" <<
      force_and_position_index_str);

  if (!is_eye) {
    eye_states_[force].SetRealEye(indx, false);
    return;
  }

  auto &calculator = PstionAndIndxCcltr<BOARD_LEN>::Ins();
  const Position &position = calculator.GetPosition(indx);
  PositionIndex piece_or_eye_count = 0;

  for (int i = 0; i < 4; ++i) {
    Position oblq_pos = ObliquePosition(position, i);
    if (!calculator.IsInBoard(oblq_pos)) {
      continue;
    }

    PositionIndex oblq_indx = calculator.GetIndex(oblq_pos);

    if (IsSelfPieceOrEye(ForceAndPositionIndex(force, oblq_indx))) {
      ++piece_or_eye_count;
    }
  }

  static const PositionIndex TABLE[3] = { 3, 2, 1 };
  auto state = calculator.CentralOrEdgeOrCorner(position);
  if (TABLE[state] <= piece_or_eye_count) {
    eye_states_[force].SetRealEye(indx, true);
    point_playable_states_array_[force].reset(indx);
    point_playable_states_array_[OppositeForce(force)].reset(indx);
  }
}

template<BoardLen BOARD_LEN>
void FullBoard<BOARD_LEN>::ModifyAdjacentIndexesPlayableStateOfChain(
    PositionIndex indx) {
  assert(GetPointState(indx) != EMPTY_POINT);
  Force force = PointToForce(GetPointState(indx));
  const auto chain_set_ptr = chain_sets_ + force;
  piece_structure::AirCount air_count = chain_set_ptr->GetAirCount(indx);
  auto air_set = chain_sets_[force].GetAirSetByPiece(indx);
  auto frce_plb_stts_ptr = point_playable_states_array_ + force;
  auto oppst_frce_plb_stts_ptr = point_playable_states_array_
      + OppositeForce(force);
  if (air_count == 1) {
    PositionIndex air_indx = math::GetLowestOne<BoardLenSquare<BOARD_LEN>()>(
        air_set);
    ForceAndPositionIndex air_force_and_index(force, air_indx);
    if (eye_states_[force].IsRealEye(air_indx)) {
      frce_plb_stts_ptr->reset(air_indx);
      oppst_frce_plb_stts_ptr->set(air_indx);
    } else if (IsFakeEye(eye_states_[force], air_indx)) {
      frce_plb_stts_ptr->set(air_indx);
      oppst_frce_plb_stts_ptr->set(air_indx);
    } else {
      frce_plb_stts_ptr->set(air_indx);
      oppst_frce_plb_stts_ptr->set(air_indx);
    }
  } else {
    const auto &real_eyes = eye_states_[force].GetRealEyes();
    auto not_real_eyes = ~real_eyes;
    *frce_plb_stts_ptr = (*frce_plb_stts_ptr | air_set) & not_real_eyes;
    *oppst_frce_plb_stts_ptr &= ~(air_set & real_eyes);
  }
}

template<BoardLen BOARD_LEN>
void FullBoard<BOARD_LEN>::ModifyAdjacentIndexesPlayableState(
    const Move &move) {
  PositionIndex indx = move.position_index;
  if (GetPointState(indx) != EMPTY_POINT) {
    ModifyAdjacentIndexesPlayableStateOfChain(indx);
  }

  auto &calculator = PstionAndIndxCcltr<BOARD_LEN>::Ins();
  Position pos = calculator.GetPosition(indx);
  Force color = move.force;
  Force oc = OppositeForce(color);
  std::function<void(PositionIndex)> Funs[3];

  Funs[color] = [](PositionIndex) {
  };

  Funs[oc] = [this](PositionIndex i) {
      ModifyAdjacentIndexesPlayableStateOfChain(i);
    };

  Funs[EMPTY_POINT] = [this](PositionIndex index) {
  };

  for (int i = 0; i < 4; ++i) {
    Position adj_pos = AdjacentPosition(pos, i);
    if (!calculator.IsInBoard(adj_pos)) {
      continue;
    }

    PointState point = GetPointState(adj_pos);
    PositionIndex adj_indx = calculator.GetIndex(adj_pos);
    Funs[point](adj_indx);
  }
}

template<BoardLen BOARD_LEN>
void FullBoard<BOARD_LEN>::ModifyAtePiecesAdjacentChains(
    const FullBoard<BOARD_LEN>::PointIndxVector &ate_pieces, Force ate_force) {
  auto &calculator = PstionAndIndxCcltr<BOARD_LEN>::Ins();

  for (PositionIndex indx : ate_pieces) {
    const Position &pos = calculator.GetPosition(indx);
    for (int i = 0; i < 4; ++i) {
      Position adj_pos = AdjacentPosition(pos, i);
      if (!calculator.IsInBoard(adj_pos)) {
        continue;
      }

      if (GetPointState(adj_pos) == OppositeForce(ate_force)) {
        PositionIndex adj_indx = calculator.GetIndex(adj_pos);
        ModifyAdjacentIndexesPlayableStateOfChain(adj_indx);
      }
    }
  }
}

template<BoardLen BOARD_LEN>
std::string FullBoard<BOARD_LEN>::PlayableStatesToString() const {
  auto get_output =
      [this](PointState point_state, PositionIndex position_index) {
        bool black_playable_state =
            point_playable_states_array_[BLACK_FORCE][position_index];
        bool white_playable_state =
            point_playable_states_array_[WHITE_FORCE][position_index];
        char output;
        if (black_playable_state && white_playable_state) {
          assert(point_state == EMPTY_POINT);
          output = 'A';
        } else if (black_playable_state) {
          assert(point_state == EMPTY_POINT);
          output = 'B';
        } else if (white_playable_state) {
          assert(point_state == EMPTY_POINT);
          output = 'W';
        } else if (point_state == BLACK_POINT) {
          output = BLACK_OUTPUT;
        } else if (point_state == WHITE_POINT) {
          output = WHITE_OUTPUT;
        } else {
          output = 'N';
        }
        return output;
      };
  return board::ToString(static_cast<const Board<BOARD_LEN>&>(*this),
                         get_output);
}

}
}

#endif
