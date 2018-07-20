#ifndef FOOLGO_SRC_DEEP_LEARNING_ENGINE_H
#define FOOLGO_SRC_DEEP_LEARNING_ENGINE_H

#include "N3LDG.h"
#include "cnn/graph_builder.h"
#include "sample.h"

#include <vector>

namespace foolgo {

std::vector<dtype> PositionIndexToVector(PositionIndex index) {
  std::vector<dtype> result;
  result.resize(362);
  PositionIndex clean_index = index ==
    (POSITION_INDEX_PASS || index == POSITION_INDEX_END) ? 361 : index;
  for (int i = 0; i < 362; ++i) {
    result.at(i) = clean_index == i;
  }
  return result;
}

class Engine {
 public:
  Engine() = default;

  explicit Engine(const HyperParams &hyper_params) :
    hyper_params_(hyper_params) {}

  void Init() {
    model_params_.Init(hyper_params_);
    model_params_.ExportModelParams(model_updater_);
    builders_.resize(hyper_params_.batch);

    for (GraphBuilder &builder : builders_) {
      builder.CreateNodes();
      builder.Init(graph_, hyper_params_, model_params_);
    }
    
    model_updater_._alpha = hyper_params_.learning_rate;
    model_updater_._eps = hyper_params_.ada_eps;
    model_updater_._reg = hyper_params_.reg;
  }

  dtype Train(const std::vector<Sample<19>> &samples) {
    metric_.reset();
    graph_.clearValue();
    graph_.train = true;

    int i = 0;
    for (const Sample<19> &sample : samples) {
      builders_.at(i++).forward(sample.full_board);
    }

    graph_.compute();

    dtype cost = 0.0f;
    i = 0;
    for (const Sample<19> &sample : samples) {
      PositionIndex index = sample.position_index;
      cost += softMaxLoss(&builders_.at(i++).output_node,
          PositionIndexToVector(index), metric_, hyper_params_.batch);
    }

    graph_.backward();
    return cost;
  }

 private:
  Graph graph_;
  std::vector<GraphBuilder> builders_;
  ModelParams model_params_;
  HyperParams hyper_params_;
  Metric metric_;
  ModelUpdate model_updater_;
};

}

#endif
