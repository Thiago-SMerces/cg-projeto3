#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>

#include <glm/gtc/matrix_inverse.hpp>
#include "imfilebrowser.h"

// bool compareFloat(float x, float y, float epsilon = 0.001f) {
//    if(fabs(x - y) < epsilon) {
//       return true;
//    }
//    else{
//       return false;
//    }
// }

void OpenGLWindow::handleEvent(SDL_Event& event) {
  glm::ivec2 mousePosition;
  SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

  if (event.type == SDL_MOUSEMOTION) {
    m_trackBallModel.mouseMove(mousePosition);
    m_trackBallLight.mouseMove(mousePosition);
  }
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT) {
      m_trackBallModel.mousePress(mousePosition);
    }
    if (event.button.button == SDL_BUTTON_RIGHT) {
      m_trackBallLight.mousePress(mousePosition);
    }
  }
  if (event.type == SDL_MOUSEBUTTONUP) {
    if (event.button.button == SDL_BUTTON_LEFT) {
      m_trackBallModel.mouseRelease(mousePosition);
    }
    if (event.button.button == SDL_BUTTON_RIGHT) {
      m_trackBallLight.mouseRelease(mousePosition);
    }
  }
  if (event.type == SDL_MOUSEWHEEL) {
    m_zoom += (event.wheel.y > 0 ? 1.0f : -1.0f) / 5.0f;
    m_zoom = glm::clamp(m_zoom, -1.5f, 1.0f);
  }

  // Keyboard events
  	if (event.type == SDL_KEYDOWN) {
    	if (event.key.keysym.sym == SDLK_LEFT) {
        if (player_turn == 1) {
          if (possible_moves[move_id].x_pos > -0.58f)
            possible_moves[move_id].x_pos -= 0.165f;
        }
        else {
          if (possible_moves[move_id].x_pos < 0.55f)
            possible_moves[move_id].x_pos += 0.165f;
        }
		  }
      if (event.key.keysym.sym == SDLK_RIGHT) {
        if (player_turn == 1) {
          if (possible_moves[move_id].x_pos < 0.415f)
            possible_moves[move_id].x_pos += 0.165f;
        }
        else {
          if (possible_moves[move_id].x_pos > -0.415f)
            possible_moves[move_id].x_pos -= 0.165f;
        }
		  }
      if (event.key.keysym.sym == SDLK_UP) {
          if (player_turn == 1) {
            if (possible_moves[move_id].y_pos < 0.55f) 
              possible_moves[move_id].y_pos += 0.165f;
          }
          else {
            if (possible_moves[move_id].y_pos > -0.55f) 
              possible_moves[move_id].y_pos -= 0.165f;
          } 
		    }
      	if (event.key.keysym.sym == SDLK_DOWN) {
          if (player_turn == 1) {
            if (possible_moves[move_id].y_pos > -0.58f)
              possible_moves[move_id].y_pos -= 0.165f;
          }
          else {
            if (possible_moves[move_id].y_pos < 0.55f)
              possible_moves[move_id].y_pos += 0.165f;
          }
		    }
		// if (event.key.keysym.sym == SDLK_a) {
    // 		m_truckSpeed = -1.0f;
		// }
    //   	if (event.key.keysym.sym == SDLK_d) {
    //     	m_truckSpeed = 1.0f;
		// }
    
      	if (event.key.keysym.sym == SDLK_SPACE) {
          if (player_turn == 1) {
            if (!selector) {
            // current_pos.model.render(current_pos.model.getNumTriangles());
            // freopen( "log.txt", "w", stdout );
            //   freopen( "log.txt", "w", stderr );
            //   std::cerr << "MOVES\nmove x: " << possible_moves[0].x_pos << " move y: " << possible_moves[0].y_pos
            //    << "\nPIECES\n" << std::endl;
            int piece_index = 0;
            for (auto& piece : player1_pieces) {
              // std::cerr << "piece x: " << piece.x_pos << " piece y: " << piece.y_pos << std::endl;
              // printf("move x: %f move y: %f piece x: %f piece y: %f", possible_moves[0].x_pos, possible_moves[0].y_pos,
              // piece.x_pos, piece.y_pos);
              if (compareFloat(possible_moves[0].x_pos, piece.x_pos) && compareFloat(possible_moves[0].y_pos, piece.y_pos)) {
                selected_piece = piece_index;
                selector = true;
                move_id = 1;
                // std::cerr << "IT'S OFFICIAL" << std::endl;
              //   freopen( "log.txt", "w", stdout );
              // freopen( "log.txt", "w", stderr );
                piece_moves = mover.drawMove(piece, player1_pieces, player2_pieces, player_turn);
                for (auto & move : piece_moves) {
                  loadModel(getAssetsPath() + "selected/tile.obj", move, "maps/pattern.png");
                  // std::cerr << "pmove x: " << move.x_pos << " pmove y: " << move.y_pos << std::endl;
                }
              }
              piece_index++;
            }
            }
            else {
                selector = false;
                move_id = 0;
                
                for (auto& move : piece_moves) {
                  freopen( "log.txt", "w", stdout );
  freopen( "log.txt", "w", stderr );
  std::cout << "output" << std::endl;
  std::cerr << "Error" << std::endl;
                  std::cerr << "pmove x: " << move.x_pos << " pmove y: " << move.y_pos << std::endl;

              std::cerr << "move x: " << move.x_pos << " move y: " << move.y_pos << std::endl;
              // printf("move x: %f move y: %f piece x: %f piece y: %f", possible_moves[0].x_pos, possible_moves[0].y_pos,
              // piece.x_pos, piece.y_pos);
              if (compareFloat(possible_moves[1].x_pos, move.x_pos) && compareFloat(possible_moves[1].y_pos, move.y_pos)) {
                // std::cerr << "Previous x: " << player1_pieces[selected_piece].x_pos << " previous y: " << player1_pieces[selected_piece].y_pos << std::endl;
                player1_pieces[selected_piece].x_pos = move.x_pos;
                player1_pieces[selected_piece].y_pos = move.y_pos;
                player1_pieces[selected_piece].first_move = false;
                player_turn = 2;
                // velocity = 0.0001f;
                // std::cerr << "x: " << player1_pieces[selected_piece].x_pos << " y: " << player1_pieces[selected_piece].y_pos << std::endl;

                // std::cerr << "IT'S OFFICIAL" << std::endl;
              }
            }
            }
          }
          else {
            if (!selector) {
            int piece_index = 0;
            // freopen( "log.txt", "w", stdout );
          freopen( "log.txt", "w", stderr );
            std::cerr << "pieces2 len " << player2_pieces.size() << " 1 len" << player1_pieces.size() << std::endl;

              std::cerr << "move x: " << possible_moves[0].x_pos << " move y: " << possible_moves[0].y_pos << std::endl;

            for (auto& piece : player2_pieces) {
                          std::cerr << "p2move x: " << piece.x_pos << " p2move y: " << piece.y_pos << std::endl;

              if (compareFloat(possible_moves[0].x_pos, piece.x_pos) && compareFloat(possible_moves[0].y_pos, piece.y_pos)) {
                selected_piece = piece_index;
                selector = true;
                move_id = 1;
                piece_moves = mover.drawMove(piece, player2_pieces, player1_pieces, player_turn);
                for (auto & move : piece_moves) {
                  loadModel(getAssetsPath() + "selected/tile.obj", move, "maps/pattern.png");
                }
                std::cerr << "IT'S A MATCH!!! " << std::endl;
              }
              piece_index++;
            }
            }
            else {
                selector = false;
                move_id = 0;
                
                for (auto& move : piece_moves) {
              if (compareFloat(possible_moves[1].x_pos, move.x_pos) && compareFloat(possible_moves[1].y_pos, move.y_pos)) {
                player2_pieces[selected_piece].x_pos = move.x_pos;
                player2_pieces[selected_piece].y_pos = move.y_pos;
                player2_pieces[selected_piece].first_move = false;
                player_turn = 1;
                // velocity = 0.0001f;
                }
            }
            }
          }
        }
    }
		// 	m_viewMatrix = 
		// 		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f),
		// 		glm::vec3(0.0f, 1.0f, 0.0f));
			
    //     	m_dollySpeed -= 1.0f;
		// }
  	// }
  	if (event.type == SDL_KEYUP) {
		// if (event.key.keysym.sym == SDLK_LEFT && m_panSpeed < 0) {
		// 	m_panSpeed = 0.0f;
		// }
		// if (event.key.keysym.sym == SDLK_RIGHT && m_panSpeed > 0) {
		// 	m_panSpeed = 0.0f;
		// }
		// if (event.key.keysym.sym == SDLK_a && m_truckSpeed < 0) {
    // 		m_truckSpeed = 0.0f;
		// }
    //   	if (event.key.keysym.sym == SDLK_d && m_truckSpeed > 0) {
    //     	m_truckSpeed = 0.0f;
		// }
		// if (event.key.keysym.sym == SDLK_UP && m_dollySpeed > 0) {
		// 	m_dollySpeed -= 0.01f;
		// }
		// if (event.key.keysym.sym == SDLK_DOWN && m_dollySpeed < 0) {
		// 	m_dollySpeed += 0.01f;
		// }
      if (event.key.keysym.sym == SDLK_SPACE) {
        if (selector) {
          Piece select;
          select.model_matrix = glm::mat4(1.0f);
          select.type = 's';
          select.x_pos = possible_moves[0].x_pos;
          select.y_pos = possible_moves[0].y_pos;
          select.z_pos = possible_moves[0].z_pos;
          possible_moves.push_back(select);
          loadModel(getAssetsPath() + "selected/tile.obj", possible_moves[1], "maps/tile.jpg");
        }
      else {
        // selector = true;
        if (possible_moves.size() > 1) {
          possible_moves[1].model.terminateGL();
          possible_moves.pop_back();
        }
        for (auto & move : piece_moves) {
                  move.model.terminateGL();
                }
                piece_moves.clear();
        }
      }
    }
}

void OpenGLWindow::initializeGL() {
  abcg::glClearColor(0, 0, 0, 1);
  abcg::glEnable(GL_DEPTH_TEST);

  // Create programs
  for (const auto& name : m_shaderNames) {
    const auto path{getAssetsPath() + "shaders/" + name};
    const auto program{createProgramFromFile(path + ".vert", path + ".frag")};
    m_programs.push_back(program);
  }

  // Load default model
  loadModel(getAssetsPath() + "board/board.obj", board, mappaths[0].c_str());
  // current_pos.model_matrix = glm::mat4(1.0);
  Piece current_pos;
  current_pos.model_matrix = glm::mat4(1.0f);
  current_pos.type = 's';
  current_pos.x_pos = -0.58f;
  current_pos.y_pos = -0.58f;
  current_pos.z_pos = 0.042f;
	possible_moves.push_back(current_pos);
  loadModel(getAssetsPath() + "selected/tile.obj", possible_moves[0], "maps/tile.jpg");
  int pieces_total = 16;
  player1_pieces.reserve(pieces_total);
  player2_pieces.reserve(pieces_total);
  
  int down = 3;
  for (auto index : iter::range(pieces_total)) {
    Piece p;
    p.index = index;
    if (index < 5) {
      p.model_matrix = glm::mat4(1.0f);
		  player1_pieces.push_back(p);
      player1_pieces[index].type = piece_types[index];
      if (index == 3)
        loadModel(getAssetsPath() + filepaths[index].c_str(), player1_pieces[index], wmappaths[index + 2].c_str());
      else
        loadModel(getAssetsPath() + filepaths[index].c_str(), player1_pieces[index], wmappaths[index + 1].c_str());
    }
    else if (index < 8) {
      player1_pieces.push_back(player1_pieces[index - down]);
      // loadModel(getAssetsPath() + filepaths[index-down].c_str(), player1_pieces[index], wmappaths[index-down + 1].c_str());
      down += 2;
    }
    else if (index == 8){
      p.model_matrix = glm::mat4(1.0f);
		  player1_pieces.push_back(p);
      player1_pieces[index].type = piece_types[piece_types.size() - 1];
      loadModel(getAssetsPath() + filepaths[filepaths.size() - 1].c_str(), player1_pieces[index], wmappaths[filepaths.size() - 1 + 1].c_str());
    }
    else {
      player1_pieces.push_back(player1_pieces[index - 1]);
    }
  }

  // player2_pieces = player1_pieces;
  down = 3;
  for (auto index : iter::range(pieces_total)) {
    Piece p;
    p.index = index;
    if (index < 5) {
      p.model_matrix = glm::mat4(1.0f);
		  player2_pieces.push_back(p);
      player2_pieces[index].type = piece_types[index];

      if (index == 3)
        loadModel(getAssetsPath() + filepaths[index].c_str(), player2_pieces[index], mappaths[index + 2].c_str());
      else
        loadModel(getAssetsPath() + filepaths[index].c_str(), player2_pieces[index], mappaths[index + 1].c_str());
    }
    else if (index < 8) {
      player2_pieces.push_back(player2_pieces[index - down]);
      // loadModel(getAssetsPath() + filepaths[index-down].c_str(), player1_pieces[index], wmappaths[index-down + 1].c_str());
      down += 2;
    }
    else if (index == 8){
      p.model_matrix = glm::mat4(1.0f);
		  player2_pieces.push_back(p);
      player2_pieces[index].type = piece_types[piece_types.size() - 1];
      loadModel(getAssetsPath() + filepaths[filepaths.size() - 1].c_str(), player2_pieces[index], mappaths[filepaths.size() - 1 + 1].c_str());
    }
    else {
      player2_pieces.push_back(player2_pieces[index - 1]);
    }
  }

  // Copying vector by copy function
  // std::copy(player1_pieces.begin(), player1_pieces.end(), std::back_inserter(player2_pieces));
  // for (auto& piece: player2_pieces) {
  //   switch (piece.type)
  //     {
  //     case 'p':
  //         loadModel(getAssetsPath() + filepaths[5].c_str(), piece, mappaths[6].c_str());
  //         break;
  //     case 'r':
  //         loadModel(getAssetsPath() + filepaths[0].c_str(), piece, mappaths[1].c_str());
  //         break;
  //     case 'c':
  //         loadModel(getAssetsPath() + filepaths[1].c_str(), piece, mappaths[2].c_str());
  //         break;
  //     case 'b':
  //         loadModel(getAssetsPath() + filepaths[2].c_str(), piece, mappaths[3].c_str());
  //         break;
  //     case 'q':
  //         loadModel(getAssetsPath() + filepaths[3].c_str(), piece, mappaths[4].c_str());
  //         break;
  //     case 'k':
  //         loadModel(getAssetsPath() + filepaths[4].c_str(), piece, mappaths[5].c_str());
  //         break;
  //     default:
  //         break;
  //     }
  // }
  // player2_pieces = player1_pieces;

  // for (auto index2 : iter::range(5)) {
  //   Piece p;
  //   p.model_matrix = glm::mat4(1.0f);
	// 	player2_pieces.push_back(p);
  //   if (index2 < 5) {
  //     player2_pieces[index2].type = piece_types[index2];
  //     if (index2 == 3)
  //       loadModel(getAssetsPath() + filepaths[index2].c_str(), player2_pieces[index2], mappaths[index2 + 2].c_str());
  //     else
  //       loadModel(getAssetsPath() + filepaths[index2].c_str(), player2_pieces[index2], mappaths[index2 + 1].c_str());
  //   }
  //   else if (index2 < 8) {
  //     player2_pieces[index2].type = piece_types[index2 - down];
  //     loadModel(getAssetsPath() + filepaths[index2-down].c_str(), player2_pieces[index2], mappaths[index2-down + 1].c_str());
  //     down += 2;
  //   }
  //   else {
  //     player2_pieces[index2].type = piece_types[piece_types.size() - 1];
  //     loadModel(getAssetsPath() + filepaths[filepaths.size() - 1].c_str(), player2_pieces[index2], mappaths[filepaths.size() - 1 + 1].c_str());
  //   }
  // }

  m_mappingMode = 3;  // "From mesh" option
  // freopen( "log.txt", "w", stdout );
  // freopen( "log.txt", "w", stderr );
  // std::cout << "output" << std::endl;
  // std::cerr << "Error" << std::endl;

  // Initial trackball spin
  m_trackBallModel.setAxis(glm::normalize(glm::vec3(0.0f, 0.5f, 0.0f)));
  m_trackBallModel.setVelocity(0.0f);
}

void OpenGLWindow::loadModel(std::string_view path, Piece &piece, const char* map) {
  piece.model.terminateGL();

  piece.model.loadDiffuseTexture(getAssetsPath() + map);
  piece.model.loadObj(path);
  piece.model.setupVAO(m_programs.at(m_currentProgramIndex));
  piece.trianglesDraw = piece.model.getNumTriangles();

  // Use material properties from the loaded model
  piece.ka = piece.model.getKa();
  piece.kd = piece.model.getKd();
  piece.ks = piece.model.getKs();
  piece.shininess = piece.model.getShininess();
}

void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  // Use currently selected program
  const auto program{m_programs.at(m_currentProgramIndex)};
  abcg::glUseProgram(program);

  // Get location of uniform variables
  const GLint viewMatrixLoc{abcg::glGetUniformLocation(program, "viewMatrix")};
  const GLint projMatrixLoc{abcg::glGetUniformLocation(program, "projMatrix")};
  modelMatrixLoc = 
      abcg::glGetUniformLocation(program, "modelMatrix");
  normalMatrixLoc =
      abcg::glGetUniformLocation(program, "normalMatrix");
  const GLint lightDirLoc{
      abcg::glGetUniformLocation(program, "lightDirWorldSpace")};
  GLint shininessLoc{abcg::glGetUniformLocation(program, "shininess")};
  GLint IaLoc{abcg::glGetUniformLocation(program, "Ia")};
  GLint IdLoc{abcg::glGetUniformLocation(program, "Id")};
  GLint IsLoc{abcg::glGetUniformLocation(program, "Is")};
  GLint KaLoc{abcg::glGetUniformLocation(program, "Ka")};
  GLint KdLoc{abcg::glGetUniformLocation(program, "Kd")};
  GLint KsLoc{abcg::glGetUniformLocation(program, "Ks")};
  GLint diffuseTexLoc{abcg::glGetUniformLocation(program, "diffuseTex")};
  GLint mappingModeLoc{
      abcg::glGetUniformLocation(program, "mappingMode")};

  // Set uniform variables used by every scene object
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform1i(diffuseTexLoc, 0);
  abcg::glUniform1i(mappingModeLoc, m_mappingMode);

  const auto lightDirRotated{m_trackBallLight.getRotation() * m_lightDir};
  abcg::glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);

  // Set uniform variables of the current object
  abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix[0][0]);

  modelViewMatrix = glm::mat3(m_viewMatrix * m_modelMatrix);
  glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  abcg::glUniform1f(shininessLoc, board.shininess);
  abcg::glUniform4fv(KaLoc, 1, &(board.ka.x));
  abcg::glUniform4fv(KdLoc, 1, &(board.kd.x));
  abcg::glUniform4fv(KsLoc, 1, &(board.ks.x));

  board.model.render(board.trianglesDraw);

  for (auto &move : piece_moves) {
    move.position = glm::vec3(move.x_pos, move.y_pos, move.z_pos);
    move.model_matrix = glm::rotate(move.model_matrix, glm::wrapAngle(glm::radians(180.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
    move.model_matrix = glm::scale(move.model_matrix, glm::vec3(0.10f, 0.10f, 0.165f));
		// Set uniform variable
		abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &(move.model_matrix[0][0]));

    modelViewMatrix = glm::mat3(m_viewMatrix * move.model_matrix);
    glm::mat3 normalMatrix2{glm::inverseTranspose(modelViewMatrix)};
    abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix2[0][0]);

		move.model.render(move.trianglesDraw);
  }

  for (auto &moves : possible_moves) {
    moves.position = glm::vec3(moves.x_pos, moves.y_pos, moves.z_pos);
    moves.model_matrix = glm::rotate(moves.model_matrix, glm::wrapAngle(glm::radians(180.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
    // possible_moves[0].model_matrix = glm::rotate(possible_moves[0].model_matrix, glm::wrapAngle(glm::radians(90.0f)), glm::vec3(1.0f, 0.0f, 0.25f));
    // possible_moves[0].model_matrix = glm::translate(possible_moves[0].model_matrix, possible_moves[0].position);
    moves.model_matrix = glm::scale(moves.model_matrix, glm::vec3(0.10f, 0.10f, 0.165f));
		// piece.model_matrix = glm::rotate(piece.model_matrix, 1.0f, glm::vec3(0.5f, 0.0f, 0.0f));
		// Set uniform variable
		abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &(moves.model_matrix[0][0]));

    modelViewMatrix = glm::mat3(m_viewMatrix * moves.model_matrix);
    glm::mat3 normalMatrix2{glm::inverseTranspose(modelViewMatrix)};
    abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix2[0][0]);

		moves.model.render(moves.trianglesDraw);
  }


  float x_pos = -0.58f;
  float y_pos = -0.58f;
  float p_scale = 0.155f;
  int index = 0;

  for (auto & piece : player1_pieces) {
    // if (piece.render) {
      if (!piece.is_positioned) {
      piece.x_pos = x_pos;
      piece.y_pos = y_pos;
      piece.z_pos = p_scale;
      piece.is_positioned = true;
    }
    piece.position = glm::vec3(piece.x_pos, piece.y_pos, piece.z_pos);
		piece.model_matrix = glm::rotate(piece.model_matrix, glm::wrapAngle(glm::radians(180.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
		// piece.model_matrix = glm::translate(piece.model_matrix, piece.position);
		piece.model_matrix = glm::scale(piece.model_matrix, glm::vec3(p_scale));
    x_pos += 0.165f;
		// piece.model_matrix = glm::rotate(piece.model_matrix, 1.0f, glm::vec3(0.5f, 0.0f, 0.0f));

    index++;
    if (index == 8) {
      x_pos = -0.58f;
      y_pos = -0.415f;
      p_scale = 0.125f;
    }

		// Set uniform variable
		abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &(piece.model_matrix[0][0]));

    modelViewMatrix = glm::mat3(m_viewMatrix * piece.model_matrix);
    glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
    abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

		piece.model.render(piece.trianglesDraw);
    // piece.render = false;
    // }
  }

  x_pos = -0.58f;
  y_pos = 0.58f;
  p_scale = 0.155f;
  index = 0;

  for (auto & piece : player2_pieces) {
    if (!piece.is_positioned) {
      piece.x_pos = x_pos;
      piece.y_pos = y_pos;
      piece.z_pos = p_scale;
      piece.is_positioned = true;

      // switch (piece.type)
      // {
      // case 'p':
      //     piece.model.loadDiffuseTexture(getAssetsPath() + mappaths[6].c_str());
      //     break;
      // case 'r':
      //     piece.model.loadDiffuseTexture(getAssetsPath() + mappaths[1].c_str());
      //     break;
      // case 'c':
      //     piece.model.loadDiffuseTexture(getAssetsPath() + mappaths[2].c_str());
      //     break;
      // case 'b':
      //     piece.model.loadDiffuseTexture(getAssetsPath() + mappaths[3].c_str());
      //     break;
      // case 'q':
      //     piece.model.loadDiffuseTexture(getAssetsPath() + mappaths[4].c_str());
      //     break;
      // case 'k':
      //     piece.model.loadDiffuseTexture(getAssetsPath() + mappaths[5].c_str());
      //     break;
      // default:
      //     break;
      // }
    }
    piece.position = glm::vec3(piece.x_pos, piece.y_pos, piece.z_pos);
		// piece.model_matrix = glm::rotate(piece.model_matrix, glm::wrapAngle(glm::radians(180.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
		// piece.model_matrix = glm::translate(piece.model_matrix, piece.position);
		piece.model_matrix = glm::scale(piece.model_matrix, glm::vec3(p_scale));
    x_pos += 0.165f;
		// piece.model_matrix = glm::rotate(piece.model_matrix, 1.0f, glm::vec3(0.5f, 0.0f, 0.0f));

    index++;
    if (index == 8) {
      x_pos = -0.58f;
      y_pos = 0.415f;
      p_scale = 0.125f;
    }

		// Set uniform variable
		abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &(piece.model_matrix[0][0]));

    modelViewMatrix = glm::mat3(m_viewMatrix * piece.model_matrix);
    glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
    abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

		piece.model.render(piece.trianglesDraw);
  }


  abcg::glUseProgram(0);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  // File browser for models
  static ImGui::FileBrowser fileDialogModel;
  fileDialogModel.SetTitle("Load 3D Model");
  fileDialogModel.SetTypeFilters({".obj"});
  fileDialogModel.SetWindowSize(m_viewportWidth * 0.8f,
                                m_viewportHeight * 0.8f);

  // File browser for textures
  static ImGui::FileBrowser fileDialogTex;
  fileDialogTex.SetTitle("Load Texture");
  fileDialogTex.SetTypeFilters({".jpg", ".png"});
  fileDialogTex.SetWindowSize(m_viewportWidth * 0.8f, m_viewportHeight * 0.8f);

// Only in WebGL
#if defined(__EMSCRIPTEN__)
  fileDialogModel.SetPwd(getAssetsPath());
  fileDialogTex.SetPwd(getAssetsPath() + "/maps");
#endif

  // Create main window widget
  {
    auto widgetSize{ImVec2(222, 190)};

    if (!board.model.isUVMapped()) {
      // Add extra space for static text
      widgetSize.y += 26;
    }

    ImGui::SetNextWindowPos(ImVec2(m_viewportWidth - widgetSize.x - 5, 5));
    ImGui::SetNextWindowSize(widgetSize);
    const auto flags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration};
    ImGui::Begin("Widget window", nullptr, flags);

    // Menu
    {
      bool loadModel{};
      bool loadDiffTex{};
      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
          ImGui::MenuItem("Load 3D Model...", nullptr, &loadModel);
          ImGui::MenuItem("Load Diffuse Texture...", nullptr, &loadDiffTex);
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }
      if (loadModel) fileDialogModel.Open();
      if (loadDiffTex) fileDialogTex.Open();
    }

    // Slider will be stretched horizontally
    ImGui::PushItemWidth(widgetSize.x - 16);
    //  = board.model.getNumTriangles();
    ImGui::SliderInt("", &board.trianglesDraw, 0, board.model.getNumTriangles(),
                     "%d triangles");
    ImGui::PopItemWidth();

    static bool faceCulling{};
    ImGui::Checkbox("Back-face culling", &faceCulling);

    if (faceCulling) {
      abcg::glEnable(GL_CULL_FACE);
    } else {
      abcg::glDisable(GL_CULL_FACE);
    }

    // CW/CCW combo box
    {
      static std::size_t currentIndex{};
      std::vector<std::string> comboItems{"CCW", "CW"};

      ImGui::PushItemWidth(120);
      if (ImGui::BeginCombo("Front face",
                            comboItems.at(currentIndex).c_str())) {
        for (auto index : iter::range(comboItems.size())) {
          const bool isSelected{currentIndex == index};
          if (ImGui::Selectable(comboItems.at(index).c_str(), isSelected))
            currentIndex = index;
          if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();

      if (currentIndex == 0) {
        abcg::glFrontFace(GL_CCW);
      } else {
        abcg::glFrontFace(GL_CW);
      }
    }

    // Projection combo box
    {
      static std::size_t currentIndex{};
      std::vector<std::string> comboItems{"Perspective", "Orthographic"};

      ImGui::PushItemWidth(120);
      if (ImGui::BeginCombo("Projection",
                            comboItems.at(currentIndex).c_str())) {
        for (auto index : iter::range(comboItems.size())) {
          const bool isSelected{currentIndex == index};
          if (ImGui::Selectable(comboItems.at(index).c_str(), isSelected))
            currentIndex = index;
          if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();

      const auto aspect{static_cast<float>(m_viewportWidth) /
                        static_cast<float>(m_viewportHeight)};
      if (currentIndex == 0) {
        m_projMatrix =
            glm::perspective(glm::radians(45.0f), aspect, 0.1f, 5.0f);

      } else {
        m_projMatrix =
            glm::ortho(-1.0f * aspect, 1.0f * aspect, -1.0f, 1.0f, 0.1f, 5.0f);
      }
    }

    // Shader combo box
    {
      static std::size_t currentIndex{};

      ImGui::PushItemWidth(120);
      if (ImGui::BeginCombo("Shader", m_shaderNames.at(currentIndex))) {
        for (auto index : iter::range(m_shaderNames.size())) {
          const bool isSelected{currentIndex == index};
          if (ImGui::Selectable(m_shaderNames.at(index), isSelected))
            currentIndex = index;
          if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();

      // Set up VAO if shader program has changed
      if (static_cast<int>(currentIndex) != m_currentProgramIndex) {
        m_currentProgramIndex = currentIndex;
        board.model.setupVAO(m_programs.at(m_currentProgramIndex));
        board.model.setupVAO(m_programs.at(m_currentProgramIndex));
      }
    }

    if (!board.model.isUVMapped()) {
      ImGui::TextColored(ImVec4(1, 1, 0, 1), "Mesh has no UV coords.");
    }

    // UV mapping box
    {
      std::vector<std::string> comboItems{"Triplanar", "Cylindrical",
                                          "Spherical"};

      if (board.model.isUVMapped()) comboItems.emplace_back("From mesh");

      ImGui::PushItemWidth(120);
      if (ImGui::BeginCombo("UV mapping",
                            comboItems.at(m_mappingMode).c_str())) {
        for (auto index : iter::range(comboItems.size())) {
          const bool isSelected{m_mappingMode == static_cast<int>(index)};
          if (ImGui::Selectable(comboItems.at(index).c_str(), isSelected))
            m_mappingMode = index;
          if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::PopItemWidth();
    }

    ImGui::End();
  }

  // Create window for light sources
  if (m_currentProgramIndex < 4) {
    const auto widgetSize{ImVec2(222, 244)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportWidth - widgetSize.x - 5,
                                   m_viewportHeight - widgetSize.y - 5));
    ImGui::SetNextWindowSize(widgetSize);
    ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoDecoration);

    ImGui::Text("Light properties");

    // Slider to control light properties
    ImGui::PushItemWidth(widgetSize.x - 36);
    ImGui::ColorEdit3("Ia", &m_Ia.x, ImGuiColorEditFlags_Float);
    ImGui::ColorEdit3("Id", &m_Id.x, ImGuiColorEditFlags_Float);
    ImGui::ColorEdit3("Is", &m_Is.x, ImGuiColorEditFlags_Float);
    ImGui::PopItemWidth();

    ImGui::Spacing();

    ImGui::Text("Material properties");

    // Slider to control material properties
    ImGui::PushItemWidth(widgetSize.x - 36);
    ImGui::ColorEdit3("Ka", &(board.ka.x), ImGuiColorEditFlags_Float);
    ImGui::ColorEdit3("Kd", &(board.kd.x), ImGuiColorEditFlags_Float);
    ImGui::ColorEdit3("Ks", &(board.ks.x), ImGuiColorEditFlags_Float);
    ImGui::PopItemWidth();

    // Slider to control the specular shininess
    ImGui::PushItemWidth(widgetSize.x - 16);
    ImGui::SliderFloat("", &(board.shininess), 0.0f, 500.0f, "shininess: %.1f");
    ImGui::PopItemWidth();

    ImGui::End();
  }

  fileDialogModel.Display();
  if (fileDialogModel.HasSelected()) {
    loadModel(fileDialogModel.GetSelected().string(), board, mappaths[0].c_str());
    fileDialogModel.ClearSelected();

    if (board.model.isUVMapped()) {
      // Use mesh texture coordinates if available...
      m_mappingMode = 3;
    } else {
      // ...or triplanar mapping otherwise
      m_mappingMode = 0;
    }
  }

  fileDialogTex.Display();
  if (fileDialogTex.HasSelected()) {
    board.model.loadDiffuseTexture(fileDialogTex.GetSelected().string());
    fileDialogTex.ClearSelected();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_trackBallModel.resizeViewport(width, height);
  m_trackBallLight.resizeViewport(width, height);
}

void OpenGLWindow::terminateGL() {
  // m_model.terminateGL();
  board.model.terminateGL();
  for (auto& move : piece_moves) {
    move.model.terminateGL();
  }
  for (auto& move : possible_moves) {
    move.model.terminateGL();
  }
  for (auto& piece : player1_pieces) {
    piece.model.terminateGL();
  }
  for (auto& piece : player2_pieces) {
    piece.model.terminateGL();
  }
  for (const auto& program : m_programs) {
    abcg::glDeleteProgram(program);
  }
}

void OpenGLWindow::update() {
  m_modelMatrix = m_trackBallModel.getRotation();
  if (player_turn == 1) {
    modifier = 1;
    // m_modelMatrix = glm::rotate(m_modelMatrix, glm::wrapAngle(glm::radians(180.0f)), glm::vec3(0.0f, 0.0f, 0.0f));
  }
  else {
    modifier = -1;
    m_modelMatrix = glm::rotate(m_modelMatrix, glm::wrapAngle(glm::radians(180.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
  }
  for (auto& piece : player1_pieces) {
    piece.model_matrix = m_trackBallModel.getRotation();
    piece.model_matrix = glm::translate(piece.model_matrix, glm::vec3(piece.x_pos * modifier, 
      piece.y_pos * modifier, piece.z_pos));
    
    if (player_turn == 2) {
      piece.model_matrix = glm::rotate(piece.model_matrix, glm::wrapAngle(glm::radians(180.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
    }
  }
  for (auto& piece : player2_pieces) {
    piece.model_matrix = m_trackBallModel.getRotation();
    piece.model_matrix = glm::translate(piece.model_matrix, glm::vec3(piece.x_pos * modifier, 
      piece.y_pos * modifier, piece.z_pos));
    if (player_turn == 2) {
      piece.model_matrix = glm::rotate(piece.model_matrix, glm::wrapAngle(glm::radians(180.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
    }
  }
  for (auto& move : piece_moves) {
    move.model_matrix = m_trackBallModel.getRotation();
    move.model_matrix = glm::translate(move.model_matrix, glm::vec3(move.x_pos * modifier, 
      move.y_pos * modifier, move.z_pos));
    if (player_turn == 2) {
      move.model_matrix = glm::rotate(move.model_matrix, glm::wrapAngle(glm::radians(180.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
    }
  }
  for (auto& move : possible_moves) {
    move.model_matrix = m_trackBallModel.getRotation();
  //   pos.model_matrix = glm::rotate(pos.model_matrix, glm::wrapAngle(glm::radians(180.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
	// // possible_moves[0].model_matrix = glm::rotate(possible_moves[0].model_matrix, glm::wrapAngle(glm::radians(90.0f)), glm::vec3(1.0f, 0.0f, 0.25f));
	// pos.model_matrix = glm::translate(pos.model_matrix, pos.position);
	// pos.model_matrix = glm::scale(pos.model_matrix, glm::vec3(0.10f, 0.10f, 0.165f));
    move.model_matrix = glm::translate(move.model_matrix, glm::vec3(move.x_pos * modifier, 
      move.y_pos * modifier, move.z_pos));
    if (player_turn == 2) {
      move.model_matrix = glm::rotate(move.model_matrix, glm::wrapAngle(glm::radians(180.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
    }
  }

  // if (velocity > 0.0f) {
  //   velocity -= 0.0000001f;
  //   m_trackBallModel.setVelocity(velocity);
  // }
  

  m_viewMatrix =
      glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f + m_zoom),
                  glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}