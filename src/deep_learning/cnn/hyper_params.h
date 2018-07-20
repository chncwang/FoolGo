#ifndef FOOLGO_SRC_DEEP_LEARNING_RESNET_HYPER_PARAMS_H
#define FOOLGO_SRC_DEEP_LEARNING_RESNET_HYPER_PARAMS_H

struct HyperParams {
  int input_dim = 5;
  int batch = 1;
  int hidden_dim = 200;
  float learning_rate = 0.001f;
  float ada_eps = 1e-8;
  float reg = 1e-8;
};

#endif
