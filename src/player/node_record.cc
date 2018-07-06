#include "node_record.h"

#include <boost/format.hpp>
#include <utility>

namespace foolgo {
namespace player {

using std::make_pair;
using std::lock_guard;
using std::mutex;
using std::ostream;
using board::PositionIndex;
using boost::format;

NodeRecord::NodeRecord() : NodeRecord(0, 0.0f, false) {}

NodeRecord::NodeRecord(const NodeRecord &node_record)
    : child_hash_keys_(node_record.child_hash_keys_) {
  visited_time_ = node_record.visited_time_;
  average_profit_ = node_record.average_profit_;
  is_in_search_ = node_record.is_in_search_;
}

const HashKey* NodeRecord::GetChildHashKeyPtr(
    PositionIndex position_index) const {
  mutex_.lock();
  auto it = child_hash_keys_.find(position_index);
  mutex_.unlock();
  return it == child_hash_keys_.end() ? nullptr : &(it->second);
}

void NodeRecord::InsertChildHashKey(PositionIndex position_index,
                                    HashKey hash_key) {
  lock_guard<mutex> lock(mutex_);
  if (child_hash_keys_.find(position_index) == child_hash_keys_.end()) {
    child_hash_keys_.insert(make_pair(position_index, hash_key));
  }
}

ostream& operator <<(ostream &os, const NodeRecord &node_record) {
  os << (format("{visited_time_:%1%, average_profit_:%2%, is_in_search_:%3%")
      % node_record.visited_time_ % node_record.average_profit_ %
      node_record.is_in_search_);
  return os;
}

}
}
