#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>
#include <string>

#include "abcg.hpp"
#include "camera.hpp"
#include "gamedata.hpp"
#include "model.hpp"
#include "movements.hpp"
#include "piece.hpp"
#include "trackball.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 	protected:
		void handleEvent(SDL_Event& event) override;
		void initializeGL() override;
		void paintGL() override;
		void paintUI() override;
		void resizeGL(int width, int height) override;
		void terminateGL() override;

	private:
		int m_viewportWidth{};
		int m_viewportHeight{};
		int playerTurn{1};
		int moveId{0};
		int selectedPiece{};
		int modifier{1};
		int m_currentProgramIndex{};
		int m_mappingMode{};
		const int piecesTotal{16};

		bool selector{false};
		
		float m_zoom{-0.5f};
		float velocity{0.0f};
		float m_panSpeed{0.0f};
		float m_panSpeedTotal{0.0f};
		const float lowerLimit{-0.58f};
		const float upperLimit{0.55f};
		const float advance{0.165f};

		Camera m_camera;

		Piece board{};
		Piece tube{};

		std::string gameMessage{};
		
		std::vector<Piece> movementSelector{};
		std::vector<Piece> pieceMoves{};
		std::vector<Piece> player1Pieces{};
		std::vector<Piece> player2Pieces{};
		
		Movements mover{};

		GameData gamedata{};

		ImFont* m_font{};

		glm::vec3 tubePosition{0.0f};
		glm::vec3 tubeRotation{glm::vec3(0.0f, 0.1f, 0.5f)};

		const std::vector<char> pieceTypes{
                                'r',
                                'c',
                                'b',
                                'q',
                                'k',
                                'p'
                                };

		const std::vector<std::string> filePaths{
								"tube/tube.obj",
								"board/board.obj",
								"selected/tile.obj",
                                "pieces/rook4.obj",
                                "pieces/knight4.obj",
                                "pieces/bishop4.obj",
                                "pieces/queen.obj",
                                "pieces/king4.obj",
                                "pieces/pawn4.obj"
                                };

  		const std::vector<std::string> otherMapPaths{
			  					"maps/tube_ss_map_st_gl00.png",
                                "maps/board.JPG",
								"maps/tile.jpg",
                                };
		
		const std::vector<std::string> whiteMapPaths{
								"maps/wrook2.jpg",
                                "maps/wknight2.jpg",
                                "maps/wbishop2.jpg",
                                "maps/wqueen2.jpg",
                                "maps/wking2.jpg",
                                "maps/wpawn2.jpg"
							};

		const std::vector<std::string> blackMapPaths{
								"maps/rook4_rook.jpg",
								"maps/knight4_knight.jpg",
                                "maps/bishop4_bishop.jpg",
                                "maps/queen4_queen.jpg",
                                "maps/king4_king.jpg",
                                "maps/pawn4_pawn.jpg"
							};

		TrackBall m_trackBallModel{};
		TrackBall m_trackBallLight{};

		glm::mat3 modelViewMatrix{};

		glm::mat4 m_modelMatrix{1.0f};
		glm::mat4 m_viewMatrix{1.0f};
		glm::mat4 m_projMatrix{1.0f};
		glm::mat4 tubeMatrix{1.0f};

		// Shaders
		std::vector<const char*> m_shaderNames{"texture", "blinnphong", "phong",
												"gouraud", "normal", "depth"};
		std::vector<GLuint> m_programs;

		// Light properties (for board)
		glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
		glm::vec4 m_Ia{1.0f};
		glm::vec4 m_Id{1.0f};
		glm::vec4 m_Is{1.0f};

		GLint modelMatrixLoc{};
		GLint normalMatrixLoc{};

		void update();
		void restart();
		void terminateVector(std::vector<Piece>& objects);
		void updatePieces(std::vector<Piece>& currentPieces);
		void loadModel(std::string_view path, Piece& piece, const char* map);
		void validatePieceMovement(std::vector<Piece>& currentPieces, int mv);
		void placePiece(std::vector<Piece>& currentPieces, std::vector<Piece>& opponentsPieces, int newTurn);
		void renderPieces(std::vector<Piece>& currentPieces, char color, int reflect, 
							const std::vector<std::string>& mapPaths);
		void bindings(std::vector<Piece>& objects, glm::vec3& rotation, glm::vec3& scale, int index, 
							float angle = 180.0f, float xPos = 0.0f, float yPos = 0.0f, float zPos = 0.0f);
};

#endif