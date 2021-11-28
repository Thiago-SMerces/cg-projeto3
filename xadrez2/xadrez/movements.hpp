#ifndef MOVEMENTS_HPP_
#define MOVEMENTS_HPP_

#include <vector>

#include "abcg.hpp"
#include "piece.hpp"

class Movements {
 public:
  std::vector<Piece> drawMove(Piece piece, std::vector<Piece> all_pieces, int player_turn);

 private:
  int player{};
  float lim_x = -0.58f;
  float lim_y = -0.58f;
  float advance = 0.165f;
  std::vector<Piece> all_pieces;

  std::vector<Piece> movePawn(Piece pawn);
  std::vector<Piece> moveRook(Piece rook);
  std::vector<Piece> moveKnight(Piece knight);
  std::vector<Piece> moveBishop(Piece bishop);
  std::vector<Piece> moveQueen(Piece queen);
  std::vector<Piece> moveKing(Piece king);
};

#endif