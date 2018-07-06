#ifndef FOOLGO_SRC_PLAYER_TRANSPOSITION_TABLE_H_
#define FOOLGO_SRC_PLAYER_TRANSPOSITION_TABLE_H_

#include <cstddef>
#include <mutex>
#include <unordered_map>

#include "../board/full_board.h"
#include "../board/position.h"
#include "../def.h"
#include "node_record.h"

namespace foolgo {
namespace player {

namespace {
struct StaySelfHasher {
  std::size_t operator ()(HashKey hash_key) const {
    return hash_key;
  }
};
}

template<board::BoardLen BOARD_LEN>
class TranspositionTable {
 public:
  NodeRecord *Get(const board::FullBoard<BOARD_LEN> &full_board) const;
  NodeRecord *GetChild(const board::FullBoard<BOARD_LEN> &full_board,
                       board::PositionIndex next_position_index);
  void Insert(const board::FullBoard<BOARD_LEN> &full_board,
              const NodeRecord &node_record);

 private:
  std::unordered_map<HashKey, NodeRecord, StaySelfHasher> node_record_map_;
  mutable std::mutex mutex_;

  NodeRecord *Get(HashKey hash_key) const;
  HashKey ChildHashKey(const board::FullBoard<BOARD_LEN> &full_board,
                       board::PositionIndex position_index);
};

template<board::BoardLen BOARD_LEN>
NodeRecord *TranspositionTable<BOARD_LEN>::Get(
    const board::FullBoard<BOARD_LEN> &full_board) const {
  HashKey hash_key = full_board.HashKey();
  return Get(hash_key);
}

template<board::BoardLen BOARD_LEN>
NodeRecord *TranspositionTable<BOARD_LEN>::GetChild(
    const board::FullBoard<BOARD_LEN> &full_board,
    board::PositionIndex position_index) {
  HashKey hash_key = ChildHashKey(full_board, position_index);
  return Get(hash_key);
}

template<board::BoardLen BOARD_LEN>
void TranspositionTable<BOARD_LEN>::Insert(
    const board::FullBoard<BOARD_LEN> &full_board,
    const NodeRecord &node_record) {
  std::lock_guard<std::mutex> lock(mutex_);
  if (node_record_map_.find(full_board.HashKey()) == node_record_map_.end()) {
    node_record_map_.insert(std::make_pair(full_board.HashKey(), node_record));
  }
}

template<board::BoardLen BOARD_LEN>
NodeRecord *TranspositionTable<BOARD_LEN>::Get(HashKey hash_key) const {
  mutex_.lock();
  auto it = node_record_map_.find(hash_key);
  mutex_.unlock();
  NodeRecord *result;
  if (it == node_record_map_.end()) {
    result = nullptr;
  } else {
    result = const_cast<NodeRecord*>(&(it->second));
  }
  return result;
}

template<board::BoardLen BOARD_LEN>
HashKey TranspositionTable<BOARD_LEN>::ChildHashKey(
    const board::FullBoard<BOARD_LEN> &full_board,
    board::PositionIndex position_index) {
  HashKey result;
  NodeRecord *node_record_ptr = Get(full_board);

  if (node_record_ptr == nullptr) {
    NodeRecord node_record(0, 0.0f, false);
    Insert(full_board, node_record);
    node_record_ptr = Get(full_board);
  }

  const HashKey *hash_key_ptr = node_record_ptr->GetChildHashKeyPtr(
      position_index);
  if (hash_key_ptr == nullptr) {
    board::FullBoard<BOARD_LEN> child_node;
    child_node.Copy(full_board);
    board::Play(&child_node, position_index);
    node_record_ptr->InsertChildHashKey(position_index, child_node.HashKey());
    result = child_node.HashKey();
  } else {
    result = *hash_key_ptr;
  }

  return result;
}

}
}

#endif /* FOOLGO_SRC_PLAYER_TRANSPOSITION_TABLE_H_ */
