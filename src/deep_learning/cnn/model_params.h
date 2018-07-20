#ifndef FOOLGO_SRC_DEEP_LEARNING_RESNET_MODEL_PARAMS_H
#define FOOLGO_SRC_DEEP_LEARNING_RESNET_MODEL_PARAMS_H

#include "N3LDG.h"
#include "hyper_params.h"

namespace foolgo {

struct ModelParams {
  std::array<UniParams, 9> uni_params_arr;
  UniParams output_param;

  void Init(const HyperParams &hyper_params) {
    for (int i = 0; i < 9; ++i) {
      int len = 19 - i * 2;
      uni_params_arr.at(i).initial(hyper_params.hidden_dim, (i == 0?
            hyper_params.input_dim : hyper_params.hidden_dim)* 9, true);
    }

    output_param.initial(362, hyper_params.hidden_dim, true);
  }

  void ExportModelParams(ModelUpdate &update) {
    for (UniParams &param : uni_params_arr) {
      param.exportAdaParams(update);
    }

    output_param.exportAdaParams(update);
  }
};

}

#endif
