#ifndef FOOLGO_SRC_DEEP_LEARNING_ENGINE_H
#define FOOLGO_SRC_DEEP_LEARNING_ENGINE_H

#include "N3LDG.h"
#include "cnn/graph_builder.h"

#include <vector>

namespace foolgo {

class Engine {
 public:
  Engine() = default;

  explicit Engine(const HyperParams &hyper_params) :
    hyper_params_(hyper_params) {}

  void Init() {
    model_params_.Init(hyper_params_);
    model_params_.ExportModelParams(model_update_);
    builders_.resize(hyper_params_.batch);

    for (GraphBuilder &builder : builders_) {
      builder.CreateNodes();
      builder.Init(graph, hyper_params_, model_params_);
    }
  }

 private:
  Graph graph;
  std::vector<GraphBuilder> builders_;
  ModelParams model_params_;
  HyperParams hyper_params_;
  Metric metric_;
  ModelUpdate model_update_;
};

}

#endif
