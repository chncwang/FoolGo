#ifndef FOOLGO_SRC_DEEP_LEARNING_RESNET
#define FOOLGO_SRC_DEEP_LEARNING_RESNET

#include <vector>
#include <array>

#include "N3LDG.h"

struct GraphBuilder {
  std::vector<BucketNode> input_nodes;
  std::array<std::vector<ConcatNode>, > concat_nodes;

  void CreateNodes() {
    input_nodes.resize(441);
  }
};

#endif
