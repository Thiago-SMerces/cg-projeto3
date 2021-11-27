#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include "abcg.hpp"
#include "model.hpp"
#include "movements.hpp"
#include "piece.hpp"
#include "trackball.hpp"

#include <string>

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& ev) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  int m_viewportWidth{};
  int m_viewportHeight{};
  int player_turn = 1;

  Piece board;
  
  std::vector<Piece> possible_moves;
  std::vector<Piece> piece_moves;

  int move_id = 0;
  bool selector = false;
  int selected_piece;

  Movements mover;

  std::vector<Piece> player1_pieces;
  glm::vec3 player1_position{};
  std::vector<Piece> player2_pieces;
  glm::vec3 player2_position{};

  std::vector<std::string> filepaths{
                                "pieces/rook.obj",
                                "pieces/knight.obj",
                                "pieces/bishop.obj",
                                "pieces/queen.obj",
                                "pieces/king.obj",
                                "pieces/pawn.obj"
                                };

  std::vector<std::string> wmappaths{
                                "maps/board.JPG",
                                "maps/wrook.jpg",
                                "maps/wknight.jpg",
                                "maps/wbishop.jpg",
                                "maps/queen.jpg",
                                "maps/wking.jpg",
                                "maps/wpawn.jpg"
                                };

  std::vector<std::string> mappaths{
                                "maps/board.JPG",
                                "maps/rook.jpg",
                                "maps/knight.jpg",
                                "maps/bishop.jpg",
                                "maps/queen.jpg",
                                "maps/king.jpg",
                                "maps/pawn.jpg"
                                };
  std::vector<char> piece_types{
                                'r',
                                'c',
                                'b',
                                'q',
                                'k',
                                'p'
                                };

  int vector_cont{0};

  TrackBall m_trackBallModel;
  TrackBall m_trackBallLight;
  float m_zoom{};

  glm::mat4 m_modelMatrix{1.0f};
  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};

  // Shaders
  std::vector<const char*> m_shaderNames{"texture", "blinnphong", "phong",
                                         "gouraud", "normal",     "depth"};
  std::vector<GLuint> m_programs;
  int m_currentProgramIndex{};

  // Mapping mode
  // 0: triplanar; 1: cylindrical; 2: spherical; 3: from mesh
  int m_mappingMode{};

  // Light and material properties (for board)
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};

  GLint modelMatrixLoc{};
  GLint normalMatrixLoc{};

  glm::mat3 modelViewMatrix{};

  void loadModel(std::string_view path, Piece &piece, const char* map);
  void update();
};

#endif