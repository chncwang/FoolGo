#ifndef FOOLGO_SRC_DEEP_LEARNING_RESNET_GRAPH_BUILDER_H
#define FOOLGO_SRC_DEEP_LEARNING_RESNET_GRAPH_BUILDER_H

#include <vector>
#include <array>

#include "N3LDG.h"
#include "model_params.h"
#include "hyper_params.h"
#include "board/full_board.h"

namespace foolgo {

std::array<std::vector<dtype>, 441> GetBoardFreature(
    const FullBoard<19> &full_board) {
  std::array<std::vector<dtype>, 441> result;
  for (auto &v : result) {
    v.resize(5);
  }

  for (int x = 0; x < 21; ++x) {
    for (int y = 0; y < 21; ++y) {
      auto &v = result.at(21 * y + x);
      if (x == 0 || x == 20 || y == 0 || y == 20) {
        v.at(0) = 0.0f;
        v.at(1) = 0.0f;
        v.at(2) = full_board.LastForce();
        v.at(3) = 1.0f; // border
        v.at(4) = 0.0f;
      } else {
        int position_index = PstionAndIndxCcltr<19>::Ins().GetIndex(
            Position(x - 1, y - 1));
        PointState point_state = full_board.GetPointState(position_index);
        v.at(0) = point_state == ForceToPointState(OppositeForce(
              full_board.LastForce()));
        v.at(1) = point_state == ForceToPointState(full_board.LastForce());
        v.at(2) = full_board.LastForce();
        v.at(3) = 0.0f; // not border
        v.at(4) = full_board.KoIndex() == position_index;
      }
    }
  }

  return result;
}

struct GraphBuilder {
  std::vector<BucketNode> input_nodes;
  std::array<std::vector<ConcatNode>, 9> concat_nodes_arr;
  std::array<std::vector<UniNode>, 9> uni_nodes_arr;
  LinearNode output_node;

  Graph *graph = nullptr;

  void CreateNodes() {
    input_nodes.resize(441);

    for (int i = 0; i < 9; ++i) {
      int len = 19 - i * 2;
      concat_nodes_arr.at(i).resize(len * len);
      uni_nodes_arr.at(i).resize(len * len);
    }
  }

  void Init(Graph &graph, const HyperParams &hyper_params,
      ModelParams &model_params) {
    this->graph = &graph;

    for (int i = 0; i < 9; ++i) {
      int len = 19 - i * 2;
      for (int j = 0; j < len * len; ++j) {
        uni_nodes_arr.at(i).at(j).activate = frelu;
        uni_nodes_arr.at(i).at(j).derivate = drelu;
        uni_nodes_arr.at(i).at(j).init(hyper_params.hidden_dim, -1);
        uni_nodes_arr.at(i).at(j).setParam(&model_params.uni_params_arr.at(i));
      }

      output_node.init(362, -1);
      output_node.setParam(&model_params.output_param);
    }
  }

  void forward(const FullBoard<19> &full_board) {
    auto board_feature = GetBoardFreature(full_board);

    int i = 0;
    for (BucketNode &node : input_nodes) {
      node.forwardArr(graph, board_feature.at(i++).data());
    }

    for (int i = 0; i < 9; ++i) {
      int len = 19 - i * 2;
      for (int y = 0; y < len; ++y) {
        for (int x = 0; x < len; ++x) {
          std::vector<Node *> previous_pointers;
          previous_pointers.resize(9);
          int pre_p_index = 0;
          for (int pos_y = -1; pos_y < 2; ++pos_y) {
            for (int pos_x = -1; pos_x < 2; ++pos_x) {
              int pre_x = x + 1 + pos_x;
              int pre_y = y + 1 + pos_y;
              int pre_index = pre_y * (len + 2) + pre_x;
              if (i == 0) {
                previous_pointers.at(pre_p_index++) =
                  &input_nodes.at(pre_index);
              } else {
                previous_pointers.at(pre_p_index++) =
                  &uni_nodes_arr.at(i - 1).at(pre_index);
              }
            }
          }

          concat_nodes_arr.at(i).at(y * len + x).forward(graph,
              previous_pointers);
          uni_nodes_arr.at(i).at(y * len + x).forward(graph,
              &concat_nodes_arr.at(i).at(y * len + x));
        }
      }
    }

    assert(uni_nodes_arr.at(8).size() == 1);
    output_node.forward(graph, &uni_nodes_arr.at(8).at(0));
  }
};

}

#endif
