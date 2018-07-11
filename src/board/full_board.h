#ifndef FOOLGO_SRC_BOARD_FULL_BOARD_H_
#define FOOLGO_SRC_BOARD_FULL_BOARD_H_

#include <functional>
#include <array>
#include <bitset>
#include <cassert>
#include <cstddef>
#include <ostream>
#include <vector>

#include "def.h"
#include "util/bitset_util.h"
#include "util/vector_util.h"
#include "piece_structure/chain_set.h"
#include "piece_structure/eye_set.h"
#include "board.h"
#include "board_difference.h"
#include "force.h"
#include "full_board_hasher.h"
#include "pos_cal.h"
#include "position.h"
#include "zob_hasher.h"

namespace foolgo {

const PositionIndex POSITION_INDEX_PASS = -1;
const PositionIndex POSITION_INDEX_END = -2;

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
  static const PositionIndex NONE = -1;

  explicit FullBoard()
      : ko_indx_(-1),
        last_force_(WHITE_FORCE),
        black_pieces_count_(0),
        hash_key_(0) {}
  ~FullBoard() = default;
  DISALLOW_COPY_AND_ASSIGN_AND_MOVE(FullBoard);
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

  PositionIndex BlackRegion() const {
    return black_pieces_count_ + eye_states_array_[BLACK_FORCE].RealCount();
  }

  foolgo::HashKey HashKey() const {
    return hash_key_;
  }

  int MoveCount() const {
    return move_count_;
  }

  void PlayMove(const Move &move);
  void Pass(Force force);

  std::vector<PositionIndex> PlayableIndexes(Force force) const;
  bool IsEnd() const;

  void SetAsEnd() {
    is_end = true;
  }

  bool IsSuicide(const Move &move) const;

  std::string ToString(
      std::function<std::string(const Board<BOARD_LEN>&)> board_to_string,
      bool board_only) const;
  std::string ToString(const PositionIndex &last_move_index,
                       bool board_only) const;
  std::string ToString(bool board_only) const;

 private:
  typedef std::vector<PositionIndex> PointIndxVector;

  piece_structure::ChainSet<BOARD_LEN> chain_sets_[2];
  std::array<BitSet<BOARD_LEN>, 2> playable_states_array_;
  std::array<piece_structure::EyeSet<BOARD_LEN>, 2> eye_states_array_;
  PositionIndex ko_indx_;
  Force last_force_;
  PositionIndex black_pieces_count_;
  foolgo::HashKey hash_key_;
  int move_count_ = 0;
  bool is_end = false;

  void SetSpecifiedAirForAdjacentChains(PositionIndex indx, bool v);

  bool IsSelfPieceOrEye(const Move &force_and_position_index) const;

  bool IsEmptySingly(PositionIndex indx) const;

  BitSet<BOARD_LEN> PlayableIndexBitSet(Force force) const;

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

  void ModifyAtePiecesAdjacentChains(const PointIndxVector &ate_pieces,
                                     Force ate_force);
  void ModifyRealEyesPlayableState();
  void SetRealEyeAsTrue(const ForceAndPositionIndex &force_and_index);

  friend std::ostream &operator <<(std::ostream &os,
                                   const FullBoard<BOARD_LEN> &full_board) {
    return os << full_board.ToString(false);
  }

  std::string PlayableStatesToString() const;
  std::string EyeStatesToString() const;
};

template<BoardLen BOARD_LEN>
Force NextForce(const FullBoard<BOARD_LEN> &b) {
  return OppositeForce(b.LastForce());
}

template<BoardLen BOARD_LEN>
void Play(FullBoard<BOARD_LEN> *full_board, PositionIndex position_index) {
  Force force = NextForce(*full_board);
  if (position_index == POSITION_INDEX_PASS) {
    full_board->Pass(force);
  } else {
    full_board->PlayMove(Move(force, position_index));
  }
}

template<BoardLen BOARD_LEN>
bool FullBoard<BOARD_LEN>::IsEnd() const {
  return is_end || (PlayableIndexBitSet(Force::BLACK_FORCE).none()
      && PlayableIndexBitSet(Force::WHITE_FORCE).none());
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
void FullBoard<BOARD_LEN>::Init() {
  Board<BOARD_LEN>::Init();
  for (int i = 0; i < 2; ++i) {
    playable_states_array_[i].set();
  }
  hash_key_ = ZobHasher<BOARD_LEN>::InstancePtr()->GetHash(*this);
}

template<BoardLen BOARD_LEN>
void FullBoard<BOARD_LEN>::Copy(const FullBoard &b) {
  Board<BOARD_LEN>::Copy(b);

  ko_indx_ = b.ko_indx_;
  last_force_ = b.last_force_;
  black_pieces_count_ = b.black_pieces_count_;
  hash_key_ = b.hash_key_;

  for (int i = 0; i < 2; ++i) {
    playable_states_array_[i] = b.playable_states_array_[i];
    eye_states_array_[i].Copy(b.eye_states_array_[i]);
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
    playable_states_array_[i].reset(move_index);
    eye_states_array_[i].SetEye(move_index, false);
    eye_states_array_[i].SetRealEye(move_index, false);
  }

  Force opposite_force = OppositeForce(move_force);
  auto &calculator = PstionAndIndxCcltr<BOARD_LEN>::Ins();
  const Position &move_position = calculator.GetPosition(move_index);
  typename FullBoard<BOARD_LEN>::PointIndxVector ate_piece_indexes_array[4],
      suisided_piece_indexes;

  PlayBasicMove(move, ate_piece_indexes_array, &suisided_piece_indexes);

  for (PositionIndex indx : suisided_piece_indexes) {
    for (auto &playable_states : playable_states_array_) {
      playable_states.set(indx);
    }
  }

  if (suisided_piece_indexes.empty()) {
    for (int i = 0; i < 2; ++i) {
      playable_states_array_[i].reset(move_index);
      eye_states_array_[i].SetEye(move_index, false);
      eye_states_array_[i].SetRealEye(move_index, false);
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
//      if (IsFakeEye(eye_states_array_[adjcnt_frce_and_indx.force],
//                    adjcnt_frce_and_indx.position_index)) {
//        playable_states_array_[opposite_force][adjacent_indx] = IsSuicide(
//            Move(opposite_force, adjacent_indx));
//      }
    }

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
      for (int j = 0; j < 2; ++j) {
        Force force = static_cast<Force>(j);
        ForceAndPositionIndex oblq_force_and_index(force, oblq_indx);
        ModifyRealEyesState(oblq_force_and_index);
      }
    }

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
        && chain_sets_[move_force].GetAirCount(move_index) == 1
        && chain_sets_[move_force].GetPieces(move_index).size() == 1) {
      ko_indx_ = single_ate_piece_index;
    }
  } else if (suisided_piece_indexes.size() == 1) {
    eye_states_array_[opposite_force].SetEye(move_index, true);
  }

  ModifyRealEyesPlayableState();

  last_force_ = move_force;

  if (GetPointState(move_index) == EMPTY_POINT) {
    board_difference.ModifyToCurrentState(ko_indx_, move_index, true,
                                          suisided_piece_indexes);
  } else {
    auto ates = util::ConcatVectors(ate_piece_indexes_array);
    board_difference.ModifyToCurrentState(ko_indx_, move_index, false, ates);
  }

  hash_key_ = ZobHasher<BOARD_LEN>::InstancePtr()->GetHash(hash_key_,
      board_difference);
  ++move_count_;
}

template<BoardLen BOARD_LEN>
void FullBoard<BOARD_LEN>::Pass(Force force) {
  last_force_ = force;
  ko_indx_ = FullBoard<BOARD_LEN>::NONE;
  hash_key_ = ZobHasher<BOARD_LEN>::InstancePtr()->GetHash(*this);
  ++move_count_;
}

template<BoardLen BOARD_LEN>
std::vector<PositionIndex> FullBoard<BOARD_LEN>::PlayableIndexes(
    Force force) const {
  auto index_bitset = PlayableIndexBitSet(force);
  return util::GetOnePositionIndexes<BoardLenSquare<BOARD_LEN>()>(
      index_bitset);
}

template<BoardLen BOARD_LEN>
std::string FullBoard<BOARD_LEN>::ToString(
    std::function<std::string(const Board<BOARD_LEN>&)> board_to_string,
    bool board_only) const {
  std::string result = board_to_string(
      static_cast<const Board<BOARD_LEN>&>(*this));

  if (!board_only) {
    result += PlayableStatesToString();
    result += EyeStatesToString();
  }

  return result;
}

template<BoardLen BOARD_LEN>
std::string FullBoard<BOARD_LEN>::ToString(const PositionIndex &last_move_index,
                                           bool board_only) const {
  auto get_output = [this, last_move_index](PositionIndex position_index) {
    PointState point_state = GetPointState(position_index);
    return GetPointStateOutput(point_state, last_move_index == position_index);
  };
  auto board_to_string = [this, get_output](const Board<BOARD_LEN> &board) {
    return BoardToString<BOARD_LEN>(get_output);
  };
  return ToString(board_to_string, board_only);
}

template<BoardLen BOARD_LEN>
std::string FullBoard<BOARD_LEN>::ToString(bool board_only) const {
  auto board_to_string = [this](const Board<BOARD_LEN> &board) {
    return BoardToString(board);
  };
  return ToString(board_to_string, board_only);
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
  return GetPointState(indx) == color || eye_states_array_[color].IsEye(indx);
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
BitSet<BOARD_LEN> FullBoard<BOARD_LEN>::PlayableIndexBitSet(
    Force force) const {
  if (KoIndex() == FullBoard<BOARD_LEN>::NONE) {
    return playable_states_array_.at(force);
  } else {
    BitSet<BOARD_LEN> kobits;
    kobits.set();
    kobits.reset(KoIndex());
    return kobits & playable_states_array_.at(force);
  }
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
      air_set.set(adjacent_index);
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
      playable_states_array_[i].set(indx);
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
      eye_states_array_[force].SetEye(indx, false);
      return;
    }
  }

  eye_states_array_[force].SetEye(indx, true);

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
  bool is_eye = eye_states_array_[force].IsEye(indx);

  if (!is_eye) {
    eye_states_array_[force].SetRealEye(indx, false);
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
    SetRealEyeAsTrue(ForceAndPositionIndex(force, indx));
  } else {
    playable_states_array_[force].set(indx);
    playable_states_array_[OppositeForce(force)].set(indx);
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
      }
    }
  }
}

template<BoardLen BOARD_LEN>
void FullBoard<BOARD_LEN>::ModifyRealEyesPlayableState() {
  auto &calculator = PstionAndIndxCcltr<BOARD_LEN>::Ins();

  for (int i=0; i<2; ++i) {
    const auto &eye_states = eye_states_array_.at(i);
    std::vector<PositionIndex> real_eyes = eye_states.GetRealEyes();

    for (PositionIndex eye_index : real_eyes) {
      const Position &eye_position = calculator.GetPosition(eye_index);

      for (int j=0; j<4; ++j) {
        Position adjacent_position = AdjacentPosition(eye_position, j);
        if (calculator.IsInBoard(adjacent_position)) {
          PositionIndex adjacent_index = calculator.GetIndex(adjacent_position);
          Force force = static_cast<Force>(i);
          Force opposite_force = OppositeForce(force);
          if (chain_sets_[i].GetAirCount(adjacent_index) == 1) {
            playable_states_array_.at(opposite_force).set(eye_index);
          } else {
            playable_states_array_.at(opposite_force).reset(eye_index);
          }
          break;
        }
      }
    }
  }
}

template<BoardLen BOARD_LEN>
void FullBoard<BOARD_LEN>::SetRealEyeAsTrue(
    const ForceAndPositionIndex &force_and_index) {
  Force force = force_and_index.force;
  PositionIndex position_index = force_and_index.position_index;
  eye_states_array_[force].SetRealEye(position_index, true);
  playable_states_array_.at(force).reset(position_index);

  Force oppsite_force = OppositeForce(force);
  playable_states_array_.at(oppsite_force).reset(position_index);
}

template<BoardLen BOARD_LEN>
std::string FullBoard<BOARD_LEN>::PlayableStatesToString() const {
  auto get_output = [this](PositionIndex position_index) {
    PointState point_state = GetPointState(position_index);
    bool black_playable_state =
    playable_states_array_[BLACK_FORCE][position_index];
    bool white_playable_state =
    playable_states_array_[WHITE_FORCE][position_index];
    std::string output;
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
  return BoardToString<BOARD_LEN>(get_output);
}

template<BoardLen BOARD_LEN>
std::string FullBoard<BOARD_LEN>::EyeStatesToString() const {
  std::string result;

  for (int i=0; i<2; ++i) {
    Force force = static_cast<Force>(i);
    result += "force:" + ForceToString(force);
    result += "eye states:";

    auto get_output = [this, force](PositionIndex position_index) {
      std::string output;
      PointState point_state = GetPointState(position_index);
      auto &eye_set = eye_states_array_[force];
      if (eye_set.IsRealEye(position_index)) {
        assert(point_state == EMPTY_POINT);
        output = 'R';
      } else if (eye_set.IsEye(position_index)) {
        assert(point_state == EMPTY_POINT);
        output = 'F';
      } else if (point_state == BLACK_POINT) {
        output = BLACK_OUTPUT;
      } else if (point_state == WHITE_POINT) {
        output = WHITE_OUTPUT;
      } else {
        output = EMPTY_OUTPUT;
      }
      return output;
    };

    result += BoardToString<BOARD_LEN>(get_output);
  }

  return result;
}

}

#endif
