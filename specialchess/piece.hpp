#ifndef PIECE_HPP_
#define PIECE_HPP_

#include "abcg.hpp"
#include "model.hpp"

struct Piece {
	char type{};
	char color{};

	bool firstMove{true};
	bool isPositioned{false};
	bool blocked{false};
	bool promoted{false};
	
	int castlingIndex{-1};
	int trianglesDraw{};
	int index{};

	float castlingX{};
	float xPos{0.58f};
	float yPos{0.58f};
	float zPos{0.18f};
	float shininess{};
	
	Model model{};
	
	glm::mat4 modelMatrix{1.0f};
	
	glm::vec3 position{xPos, yPos, zPos};

	glm::vec4 ka{0.0f};
	glm::vec4 kd{0.0f};
	glm::vec4 ks{0.0f};
};

inline bool compareFloat(float x, float y, float epsilon = 0.006f) {
	return (fabs(x - y) < epsilon) ? true : false;
}

#endif