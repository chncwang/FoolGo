#include "node_record.h"

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <utility>

namespace foolgo {
namespace player {

using std::make_pair;

namespace {
log4cplus::Logger logger = log4cplus::Logger::getInstance(
    "foolgo.player.NodeRecord");
}

const HashKey* NodeRecord::GetChildHashKeyPtr(
    board::PositionIndex position_index) const {
  auto it = child_hash_keys_.find(position_index);
  return it == child_hash_keys_.end() ? nullptr : &(it->second);
}

void NodeRecord::InsertChildHashKey(board::PositionIndex position_index,
                                    HashKey hash_key) {
  child_hash_keys_.insert(make_pair(position_index, hash_key));
}

}
}
