#include "node_record.h"

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <utility>

namespace foolgo {
namespace player {

using std::make_pair;
using std::lock_guard;
using std::mutex;
using board::PositionIndex;

namespace {
log4cplus::Logger logger = log4cplus::Logger::getInstance(
    "foolgo.player.NodeRecord");
}

NodeRecord::NodeRecord() : NodeRecord(0, 0.0f, false) {}

NodeRecord::NodeRecord(const NodeRecord &node_record)
    : child_hash_keys_(node_record.child_hash_keys_) {
  visited_time_ = node_record.visited_time_.load();
  average_profit_ = node_record.average_profit_.load();
  is_in_search_ = node_record.is_in_search_.load();
}

const HashKey* NodeRecord::GetChildHashKeyPtr(
    PositionIndex position_index) const {
  lock_guard<mutex> lock(mutex_);
  auto it = child_hash_keys_.find(position_index);
  return it == child_hash_keys_.end() ? nullptr : &(it->second);
}

void NodeRecord::InsertChildHashKey(PositionIndex position_index,
                                    HashKey hash_key) {
  lock_guard<mutex> lock(mutex_);
  if (child_hash_keys_.find(position_index) == child_hash_keys_.end()) {
    child_hash_keys_.insert(make_pair(position_index, hash_key));
  }
}

}
}
