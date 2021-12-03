#ifndef MOVEMENTS_HPP_
#define MOVEMENTS_HPP_

#include <vector>

#include "abcg.hpp"
#include "gamedata.hpp"
#include "piece.hpp"

class Movements {
	public:
		std::vector<Piece> drawMove(Piece& piece, std::vector<Piece> pieces, int player_turn);

 	private:
  		int player{};
		int nMoves{};
		float boardEdge{-0.58f};
		float advance{0.165f};
		float z{0.04f};

		Piece blockingPiece{};
		Piece next{};

		std::vector<Piece> playersPieces{};

		std::vector<Piece> movePawn(Piece& pawn);
		std::vector<Piece> moveRook(Piece& rook);
		std::vector<Piece> moveKnight(Piece& knight);
		std::vector<Piece> moveBishop(Piece& bishop);
		std::vector<Piece> moveQueen(Piece& queen);
		std::vector<Piece> moveKing(Piece& king);

		void cast(Piece& king, Piece& firstTile, int pos, std::vector<Piece>& pieceMoves);
		void insertElement(Piece& next, char piece_type, std::vector<Piece>& pieceMoves);
		// orientation 1 = right/up 2 = left/up 3 = left/down 4 = right/down
		// axis = x or axis = y
		bool insertValidMove(Piece& current, std::vector<Piece>& pieceMoves, int orientation);
		Piece isBlocked(Piece& p, std::vector<Piece> pieces);
};

#endif