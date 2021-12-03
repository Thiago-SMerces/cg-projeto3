#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include <glm/gtc/matrix_inverse.hpp>
#include "imfilebrowser.h"

void OpenGLWindow::validatePieceMovement(std::vector<Piece>& currentPieces, int mv) {
	int pieceIndex = 0;
	for (auto& piece : currentPieces) {
		if (compareFloat(movementSelector[0].xPos, piece.xPos) && 
			compareFloat(movementSelector[0].yPos, piece.yPos)) {
			selectedPiece = pieceIndex;
			selector = true;
			moveId = mv;
			std::vector<Piece> pieces;
			pieces.reserve(player1Pieces.size() + player2Pieces.size());
			pieces.insert(pieces.end(), player1Pieces.begin(), player1Pieces.end());
			pieces.insert(pieces.end(), player2Pieces.begin(), player2Pieces.end());
			pieceMoves = mover.drawMove(piece, pieces, playerTurn);
			for (auto & move : pieceMoves) {
				loadModel(getAssetsPath() + filePaths[2].c_str(), move, otherMapPaths[0].c_str());
			}
		}
		pieceIndex++;
	}
}

void OpenGLWindow::placePiece(std::vector<Piece>& currentPieces, std::vector<Piece>& opponentsPieces, int newTurn) {
	selector = false;
	moveId = 0;
	for (auto& move : pieceMoves) {
		if (compareFloat(movementSelector[1].xPos, move.xPos) && 
			compareFloat(movementSelector[1].yPos, move.yPos)) {
			
			currentPieces[selectedPiece].xPos = move.xPos;
			currentPieces[selectedPiece].yPos = move.yPos;
			currentPieces[selectedPiece].first_move = false;
			
			for (auto index : iter::range(opponentsPieces.size())) {
				if (compareFloat(move.xPos, opponentsPieces[index].xPos) && 
					compareFloat(move.yPos, opponentsPieces[index].yPos)) {
					// check if king was captured
					if (opponentsPieces[index].type == 'k') {
						gamedata.m_state = State::Checkmate;
					}
					opponentsPieces.erase(opponentsPieces.begin() + index);
				}
			}
			
			// check if the move is castling
			if (currentPieces[selectedPiece].type == 'k' &&
				currentPieces[selectedPiece].castling_index != -1) {
				if (compareFloat(currentPieces[selectedPiece].castling_x, move.xPos)) {
					int rook_index = currentPieces[selectedPiece].castling_index;
					if (rook_index > selectedPiece) {
						currentPieces[rook_index].xPos = currentPieces[selectedPiece].xPos - advance;
					}
					else {
						currentPieces[rook_index].xPos = currentPieces[selectedPiece].xPos + advance;
					}
				}
				currentPieces[selectedPiece].castling_index = -1;
			}

			// check if promoting pawn
			if (currentPieces[selectedPiece].type == 'p') {
				if ((currentPieces[selectedPiece].yPos > upperLimit && currentPieces[selectedPiece].yPos < -lowerLimit) || 
					(currentPieces[selectedPiece].yPos > lowerLimit && currentPieces[selectedPiece].yPos < -upperLimit)) {
					
					currentPieces[selectedPiece].type = 'q';
					// default to queen promotion
					if (currentPieces[selectedPiece].color == 'w') {
						loadModel(getAssetsPath() + filePaths[6].c_str(), currentPieces[selectedPiece], 
								whiteMapPaths[3].c_str());
					}
					else {
						loadModel(getAssetsPath() + filePaths[6].c_str(), currentPieces[selectedPiece], 
								blackMapPaths[3].c_str());
					}
				}
			}
			playerTurn = newTurn;
		}
	}
}

void OpenGLWindow::handleEvent(SDL_Event& event) {
  	glm::ivec2 mousePosition;
	SDL_GetMouseState(&mousePosition.x, &mousePosition.y);

	if (event.type == SDL_MOUSEMOTION) {
		m_trackBallModel.mouseMove(mousePosition);
		m_trackBallLight.mouseMove(mousePosition);
	}
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT || event.button.button == SDL_BUTTON_RIGHT) {
			m_trackBallModel.mousePress(mousePosition);
		}
	}
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (event.button.button == SDL_BUTTON_LEFT || event.button.button == SDL_BUTTON_RIGHT) {
			m_trackBallModel.mouseRelease(mousePosition);
		}
	}
	if (event.type == SDL_MOUSEWHEEL) {
		m_zoom += (event.wheel.y > 0 ? 1.0f : -1.0f) / 5.0f;
		m_zoom = glm::clamp(m_zoom, -1.5f, 1.0f);
	}

  	// Keyboard events
  	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_a)
			m_panSpeed = -1.0f;
		if (event.key.keysym.sym == SDLK_d)
			m_panSpeed = 1.0f;

    	if (event.key.keysym.sym == SDLK_LEFT && gamedata.m_state != State::Checkmate) {
        	if (playerTurn == 1 && movementSelector[moveId].xPos > lowerLimit) {
            	movementSelector[moveId].xPos -= advance;
        	}
        	else if (playerTurn == 2 && movementSelector[moveId].xPos < upperLimit) {
            	movementSelector[moveId].xPos += advance;
        	}
		}
    	if (event.key.keysym.sym == SDLK_RIGHT && gamedata.m_state != State::Checkmate) {
        	if (playerTurn == 1 && movementSelector[moveId].xPos < upperLimit) {
            	movementSelector[moveId].xPos += advance;
        	}
        	else if (playerTurn == 2 && movementSelector[moveId].xPos > lowerLimit) {
            	movementSelector[moveId].xPos -= advance;
        	}
		}
      	if (event.key.keysym.sym == SDLK_UP && gamedata.m_state != State::Checkmate) {
          	if (playerTurn == 1 && movementSelector[moveId].yPos < upperLimit) {
              	movementSelector[moveId].yPos += advance;
          	}
          	else if (playerTurn == 2 && movementSelector[moveId].yPos > lowerLimit) {
              	movementSelector[moveId].yPos -= advance;
          	}
		}
      	if (event.key.keysym.sym == SDLK_DOWN && gamedata.m_state != State::Checkmate) {
          	if (playerTurn == 1 && movementSelector[moveId].yPos > lowerLimit) {
              	movementSelector[moveId].yPos -= advance;
          	}
          	else if (playerTurn == 2 && movementSelector[moveId].yPos < upperLimit) {
              	movementSelector[moveId].yPos += advance;
          	}
		}
		
      	if (event.key.keysym.sym == SDLK_SPACE && gamedata.m_state != State::Checkmate) {
          	if (playerTurn == 1) {
            	if (!selector) {
					validatePieceMovement(player1Pieces, 1);
            	}
            	else {
					placePiece(player1Pieces, player2Pieces, 2);
            	}
          	}
			else {
				if (!selector) {
					validatePieceMovement(player2Pieces, 1);
				}
				else {
					placePiece(player2Pieces, player1Pieces, 1);
				}
			}
		}
    }

  	if (event.type == SDL_KEYUP) {
		if (event.key.keysym.sym == SDLK_r && gamedata.m_state == State::Checkmate)
			restart();
		if (event.key.keysym.sym == SDLK_a && m_panSpeed < 0)
			m_panSpeed = 0.0f;
		if (event.key.keysym.sym == SDLK_d && m_panSpeed > 0)
			m_panSpeed = 0.0f;
    	if (event.key.keysym.sym == SDLK_SPACE && gamedata.m_state != State::Checkmate) {
        	if (selector) {
				Piece select;
				select.modelMatrix = glm::mat4(1.0f),
				select.type = 's',
				select.xPos = movementSelector[0].xPos,
				select.yPos = movementSelector[0].yPos,
				select.zPos = movementSelector[0].zPos;
				movementSelector.push_back(select);
				loadModel(getAssetsPath() + filePaths[2].c_str(), movementSelector[1], otherMapPaths[2].c_str());
			}
			else {
				if (movementSelector.size() > 1) {
					movementSelector[1].model.terminateGL();
					movementSelector.pop_back();
				}
				for (auto & move : pieceMoves) {
					move.model.terminateGL();
				}
				pieceMoves.clear();
			}
		}
    }
}

void OpenGLWindow::renderPieces(std::vector<Piece>& currentPieces, char color, int reflect,
								const std::vector<std::string>& mapPaths) {
	size_t fileLastIndex = filePaths.size() - 1;
	size_t mapLastIndex = mapPaths.size() - 1;
	for (auto index : iter::range(piecesTotal)) {
		Piece p;
		p.index = index;
		p.color = color;
		if (index < 5) {
			p.modelMatrix = glm::mat4(1.0f);
			currentPieces.push_back(p);
			currentPieces[index].type = pieceTypes[index];
			loadModel(getAssetsPath() + filePaths[index + 3].c_str(), 
						currentPieces[index], mapPaths[index].c_str());
		}
		else if (index < 8) {
			currentPieces.push_back(currentPieces[index - reflect]);
			reflect += 2;
		}
		else {
			p.modelMatrix = glm::mat4(1.0f);
			currentPieces.push_back(p);
			currentPieces[index].type = pieceTypes[mapLastIndex];
			loadModel(getAssetsPath() + filePaths[fileLastIndex].c_str(), 
						currentPieces[index], mapPaths[mapLastIndex].c_str());
		}
	}
}

void OpenGLWindow::initializeGL() {
	// Load a new font
  	ImGuiIO &io{ImGui::GetIO()};
  	auto filename{getAssetsPath() + "AlexBrush-Regular.ttf"};
  	m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  	if (m_font == nullptr) {
    	throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  	}
	
	abcg::glClearColor(5.0f / 255.0f, 16.0f / 255.0f, 109.0f / 255.0f, 1);
	abcg::glEnable(GL_DEPTH_TEST);

  	restart();
}

void OpenGLWindow::restart() {
	// clear everything before starting
	terminateGL();

	// reset player turn, moveid, modifier, zoom
	playerTurn = 1;
	moveId = 0;
	modifier = 1;
	m_zoom = -0.5f;
	velocity = 0.0f;

	// reset mover, board and tube
	mover = {};
	board = {};
	tube = {};
	tubePosition = glm::vec3(0.0f);
	tubeRotation = glm::vec3(0.0f, 0.1f, 0.5f);

	// m_lightDir = glm::vec4(-1.0f, -1.0f, -1.0f, 0.0f);
	// m_Ia = glm::vec4(1.0f);
	// m_Id = glm::vec4(1.0f);
	// m_Is = glm::vec4(1.0f);

	// Create programs
	for (const auto& name : m_shaderNames) {
		const auto path{getAssetsPath() + "shaders/" + name};
		const auto program{createProgramFromFile(path + ".vert", path + ".frag")};
		m_programs.push_back(program);
	}

	// clear vectors too
	movementSelector.clear();
	player1Pieces.clear();
	player2Pieces.clear();

	// set state as playing
	gamedata.m_state = State::Playing;
  	// Load board model
	loadModel(getAssetsPath() + filePaths[1].c_str(), board, otherMapPaths[1].c_str());

	// load tube model
	loadModel(getAssetsPath() + filePaths[0].c_str(), tube, otherMapPaths[0].c_str());

	// load initial movement selector tile
	Piece initialPos;
	initialPos.modelMatrix = glm::mat4(1.0f),
	initialPos.type = 's',
	initialPos.xPos = -0.58f,
	initialPos.yPos = -0.58f,
	initialPos.zPos = 0.042f;
	movementSelector.push_back(initialPos);

	loadModel(getAssetsPath() + filePaths[2].c_str(), movementSelector[0], otherMapPaths[2].c_str());
	
	// render all pieces in the board
	player1Pieces.reserve(piecesTotal);
	player2Pieces.reserve(piecesTotal);

	renderPieces(player1Pieces, 'w', 3, whiteMapPaths);
	renderPieces(player2Pieces, 'b', 3, blackMapPaths);

	m_mappingMode = 3;  // "From mesh" option

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

void OpenGLWindow::bindings(std::vector<Piece>& objects, glm::vec3& rotation, glm::vec3& scale, int index, 
							float angle, float xPos, float yPos, float zPos) {
	
	for (auto &object : objects) {
		if (index > -1 && !object.is_positioned) {
			object.xPos = xPos;
			object.yPos = yPos;
			object.zPos = zPos;
			object.is_positioned = true;
		}
		object.position = glm::vec3(object.xPos, object.yPos, object.zPos);
		object.modelMatrix = glm::rotate(object.modelMatrix, glm::wrapAngle(glm::radians(angle)), rotation);
		object.modelMatrix = glm::scale(object.modelMatrix, scale);

		if (index > -1) {
			xPos += advance;
			index++;
			
			if (index == 8) {
				xPos = -0.58f;
				yPos = -0.415f * modifier;
				zPos = 0.125f;
				scale =  glm::vec3(zPos);
			}
		}

		abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &(object.modelMatrix[0][0]));

		modelViewMatrix = glm::mat3(m_viewMatrix * object.modelMatrix);
		glm::mat3 normalMatrix2{glm::inverseTranspose(modelViewMatrix)};
		abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix2[0][0]);

		object.model.render(object.trianglesDraw);
	}
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
	GLint mappingModeLoc{abcg::glGetUniformLocation(program, "mappingMode")};

	// Set uniform variables used by every scene object
	abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
	abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
	abcg::glUniform1i(diffuseTexLoc, 0);
	abcg::glUniform1i(mappingModeLoc, m_mappingMode);

	// camera
	abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_camera.m_viewMatrix[0][0]);
  	abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_camera.m_projMatrix[0][0]);

	const auto lightDirRotated{m_trackBallLight.getRotation() * m_lightDir};
	abcg::glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);
	abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
	abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
	abcg::glUniform4fv(IsLoc, 1, &m_Is.x);

	// board
	abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix[0][0]);

	modelViewMatrix = glm::mat3(m_viewMatrix * m_modelMatrix);
	glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
	abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

	abcg::glUniform1f(shininessLoc, board.shininess);
	abcg::glUniform4fv(KaLoc, 1, &(board.ka.x));
	abcg::glUniform4fv(KdLoc, 1, &(board.kd.x));
	abcg::glUniform4fv(KsLoc, 1, &(board.ks.x));

	board.model.render(board.trianglesDraw);

	// piece movements
	glm::vec3 rotation{0.0f, 0.0f, 1.0f};
	glm::vec3 scale{0.10f, 0.10f, 0.165f};

	bindings(pieceMoves, rotation, scale, -1);

	// movement selector
	rotation = glm::vec3(0.0f, 0.0f, 1.0f);
	scale = glm::vec3(0.10f, 0.10f, 0.165f);

	bindings(movementSelector, rotation, scale, -1);

	// player 1 pieces
	float xPos = -0.58f;
	float yPos = -0.58f;
	float zPos = 0.155f;

	rotation = glm::vec3(0.0f, 0.0f, 1.0f);
	scale = glm::vec3(zPos);

	bindings(player1Pieces, rotation, scale, 0, 180.0f, xPos, yPos, zPos);

	// player 2 pieces
	xPos = -0.58f;
	yPos = 0.58f;
	zPos = 0.155f;

	rotation = glm::vec3(0.0f, 0.0f, 1.0f);
	scale = glm::vec3(zPos);

	// guarantee that pawns are rendered correctly
	modifier *= -1;
	bindings(player2Pieces, rotation, scale, 0, 0.0f, xPos, yPos, zPos);
	modifier *= -1;

	// tube (stage) does not need loop
	tube.position = tubePosition;

  	tube.modelMatrix = glm::rotate(tube.modelMatrix, glm::wrapAngle(glm::radians(90.0f)), tubeRotation);
	tube.modelMatrix = glm::translate(tube.modelMatrix, tubePosition);
	tube.modelMatrix = glm::scale(tube.modelMatrix, glm::vec3(7.0f));
	
	abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &(tube.modelMatrix[0][0]));
	modelViewMatrix = glm::mat3(m_viewMatrix * tube.modelMatrix);
	glm::mat3 normalMatrix2{glm::inverseTranspose(modelViewMatrix)};
	abcg::glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix2[0][0]);
	
	tube.model.render(tube.trianglesDraw);

  	abcg::glUseProgram(0);
}

void OpenGLWindow::paintUI() {
	abcg::OpenGLWindow::paintUI();

  	// Create main window widget
  	{
		auto widgetSize{ImVec2(222, 190)};

		if (!board.model.isUVMapped()) {
			widgetSize.y += 26;
		}

		ImGui::SetNextWindowPos(ImVec2(m_viewportWidth - widgetSize.x - 5, 5));
		ImGui::SetNextWindowSize(widgetSize);
		const auto flags{ImGuiWindowFlags_NoDecoration};
		ImGui::Begin("Widget window", nullptr, flags);
		ImGui::Text("Game properties");
		if (gamedata.m_state == State::Playing)
			gameMessage = "Playing";
		else if (gamedata.m_state == State::Checkmate) {
			if (playerTurn == 1)
				gameMessage = "Checkmate";
			else 
				gameMessage = "Checkmate";
		}
		ImGui::Text("Status - %s", gameMessage.c_str());

		// Slider will be stretched horizontally
		ImGui::PushItemWidth(widgetSize.x - 16);
		ImGui::SliderInt("", &board.trianglesDraw, 0, board.model.getNumTriangles(),
						"%d triangles");
		ImGui::PopItemWidth();

		static bool faceCulling{};
		ImGui::Checkbox("Back-face culling", &faceCulling);

		if (faceCulling) {
			abcg::glEnable(GL_CULL_FACE);
		}
		else {
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
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();

			if (currentIndex == 0)
				abcg::glFrontFace(GL_CCW);
			else
				abcg::glFrontFace(GL_CW);
		}

		// Projection combo box
		{
			static std::size_t currentIndex{};
			std::vector<std::string> comboItems{"Perspective", "Orthographic"};

			ImGui::PushItemWidth(120);
			if (ImGui::BeginCombo("Projection", comboItems.at(currentIndex).c_str())) {
				for (auto index : iter::range(comboItems.size())) {
					const bool isSelected{currentIndex == index};
					if (ImGui::Selectable(comboItems.at(index).c_str(), isSelected))
						currentIndex = index;
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();

			const auto aspect{static_cast<float>(m_viewportWidth) /
								static_cast<float>(m_viewportHeight)};
			if (currentIndex == 0) {
				m_projMatrix = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 5.0f);

			}
			else {
				m_projMatrix = glm::ortho(-1.0f * aspect, 1.0f * aspect, -1.0f, 1.0f, 0.1f, 5.0f);
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
					if (isSelected)
						ImGui::SetItemDefaultFocus();
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
			std::vector<std::string> comboItems{"Triplanar", "Cylindrical", "Spherical"};

			if (board.model.isUVMapped())
				comboItems.emplace_back("From mesh");

			ImGui::PushItemWidth(120);
			if (ImGui::BeginCombo("UV mapping", comboItems.at(m_mappingMode).c_str())) {
				for (auto index : iter::range(comboItems.size())) {
					const bool isSelected{m_mappingMode == static_cast<int>(index)};
					if (ImGui::Selectable(comboItems.at(index).c_str(), isSelected))
						m_mappingMode = index;
					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
		}

		ImGui::End();
	}

	if (gamedata.m_state == State::Checkmate) {
		const auto size{ImVec2(400, 85)};
		const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f, 
									(m_viewportHeight - size.y) / 2.0f)};
		
		ImGui::SetNextWindowPos(position);
		ImGui::SetNextWindowSize(size);
		ImGuiWindowFlags restartFlags{ImGuiWindowFlags_NoBackground | 
									ImGuiWindowFlags_NoTitleBar |
									ImGuiWindowFlags_NoInputs};
		ImGui::Begin(" ", nullptr, restartFlags);
		ImGui::PushFont(m_font);

		if (playerTurn == 1)
			ImGui::Text("Player 2 Won");
		else
			ImGui::Text("Player 1 Won");
			
		ImGui::PopFont();
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
}

void OpenGLWindow::resizeGL(int width, int height) {
	m_viewportWidth = width;
	m_viewportHeight = height;

	m_trackBallModel.resizeViewport(width, height);
	m_trackBallLight.resizeViewport(width, height);

	m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateVector(std::vector<Piece>& objects) {
	for (auto& object : objects)
		object.model.terminateGL();
	
	objects.clear();
}

void OpenGLWindow::terminateGL() {
	// terminate board
	board.model.terminateGL();

	// terminate tube
	tube.model.terminateGL();
	
	// terminate piece movements
	terminateVector(pieceMoves);

	// terminate movement selector
	terminateVector(movementSelector);

	// terminate player 1 pieces
	terminateVector(player1Pieces);

	// terminate player 2 pieces
	terminateVector(player2Pieces);
	
	// terminate programs
	for (const auto& program : m_programs)
		abcg::glDeleteProgram(program);
	
	m_programs.clear();
}

void OpenGLWindow::updatePieces(std::vector<Piece>& currentPieces) {
	for (auto& piece : currentPieces) {
		piece.modelMatrix = m_trackBallModel.getRotation();
		piece.modelMatrix = glm::translate(piece.modelMatrix, glm::vec3(piece.xPos * modifier, 
		piece.yPos * modifier, piece.zPos));
		
		if (playerTurn == 2) {
			piece.modelMatrix = glm::rotate(piece.modelMatrix, glm::wrapAngle(glm::radians(180.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
		}
	}
}

void OpenGLWindow::update() {
	m_modelMatrix = m_trackBallModel.getRotation();
	if (playerTurn == 1) {
		modifier = 1;
	}
	else {
		modifier = -1;
		m_modelMatrix = glm::rotate(m_modelMatrix, glm::wrapAngle(glm::radians(180.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	// update player 1 pieces
	updatePieces(player1Pieces);
	// update player 2 pieces
	updatePieces(player2Pieces);
	// update piece moves vector 
	updatePieces(pieceMoves);
	// update movement selector vector
	updatePieces(movementSelector);

	// create illusion that tube is constantly moving 
	tube.modelMatrix = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0f, 0.1f, 0.5f)) * tubeMatrix;
	const float deltaTime{static_cast<float>(getDeltaTime())};
	tubePosition.z += deltaTime * 0.5f;
	tubeRotation.z += deltaTime * 0.5f;

	if (tubePosition.z > 3.0f) {
		tubePosition.z = -1.0f;
	}
	// limit look at to sides of the tube
	if (m_panSpeedTotal > -150.0f && m_panSpeedTotal < 150.0f) {
		m_panSpeedTotal += m_panSpeed;
		m_camera.pan(m_panSpeed * deltaTime);
	}
	else if (m_panSpeedTotal + m_panSpeed > -150.0f && m_panSpeedTotal + m_panSpeed < 150.0f) {
		m_panSpeedTotal += m_panSpeed;
	}

	m_viewMatrix =	glm::lookAt(m_camera.getEye(m_zoom), m_camera.getAt(), m_camera.getUp());
}