#ifndef FOOLGO_SRC_PLAYER_NODE_RECORD_H_
#define FOOLGO_SRC_PLAYER_NODE_RECORD_H_

#include <atomic>
#include <cstdint>
#include <map>
#include <mutex>

#include "../board/position.h"
#include "../def.h"

namespace foolgo {
namespace player {

class NodeRecord {
 public:
  NodeRecord();
  NodeRecord(int32_t visited_time, float average_profit, bool is_in_search)
      : visited_time_(visited_time),
        average_profit_(average_profit),
        is_in_search_(is_in_search) {}
  NodeRecord(const NodeRecord &node_record);
  NodeRecord& operator =(const NodeRecord &node_record) = default;
  int32_t GetVisitedTime() const {
    return visited_time_;
  }
  void SetVisitedTimes(int32_t visited_times) {
    visited_time_ = visited_times;
  }
  float GetAverageProfit() const {
    return average_profit_;
  }
  void SetAverageProfit(float average_profit) {
    average_profit_ = average_profit;
  }
  bool IsInSearch() const {
    return is_in_search_;
  }
  void SetIsInSearch(bool is_in_search) {
    is_in_search_ = is_in_search;
  }
  const HashKey* GetChildHashKeyPtr(board::PositionIndex position_index) const;
  void InsertChildHashKey(board::PositionIndex position_index,
                          HashKey hash_key);

 private:
  std::atomic<int32_t> visited_time_;
  std::atomic<float> average_profit_;
  std::atomic<bool> is_in_search_;
  std::map<board::PositionIndex, HashKey> child_hash_keys_;
  mutable std::mutex mutex_;
};

}
}

#endif /* FOOLGO_SRC_PLAYER_NODE_RECORD_H_ */
