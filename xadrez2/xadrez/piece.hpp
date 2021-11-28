#ifndef PIECE_HPP_
#define PIECE_HPP_

#include <vector>

#include "abcg.hpp"
#include "model.hpp"

struct Piece {
  char type;
  bool first_move = true;
  bool is_positioned = false;
  bool is_loaded = false;
  bool render = true;
  int castling_index = -1;
  float x_pos = 0.58f;
  float y_pos = 0.58f;
  float z_pos = 0.18f;
  float shininess{};
  int trianglesDraw{};
  Model model{};
  glm::mat4 model_matrix{1.0f};
  glm::vec3 position{x_pos, y_pos, z_pos};

  glm::vec4 ka{0.0f};
  glm::vec4 kd{0.0f};
  glm::vec4 ks{0.0f};
};

inline bool compareFloat(float x, float y, float epsilon = 0.006f) {
   if(fabs(x - y) < epsilon) {
      return true;
   }
   else{
      return false;
   }
}

#endif