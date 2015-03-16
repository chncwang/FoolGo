#ifndef FOOLGO_SRC_PLAYER_NODE_RECORD_H_
#define FOOLGO_SRC_PLAYER_NODE_RECORD_H_

#include <stdint.h>
#include <map>

#include "../board/position.h"
#include "../def.h"

namespace foolgo {
namespace player {

class NodeRecord {
 public:
  NodeRecord() = default;
  NodeRecord(int32_t visited_time, float average_profit)
      : visited_time_(visited_time),
        average_profit_(average_profit) {}
  NodeRecord(const NodeRecord &node_record) = default;
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
  const HashKey* GetChildHashKeyPtr(board::PositionIndex position_index) const;
  void InsertChildHashKey(board::PositionIndex position_index,
                          HashKey hash_key);

 private:
  int32_t visited_time_ = 0;
  float average_profit_ = 0.0f;
  std::map<board::PositionIndex, HashKey> child_hash_keys_;
};

}
}

#endif /* FOOLGO_SRC_PLAYER_NODE_RECORD_H_ */
