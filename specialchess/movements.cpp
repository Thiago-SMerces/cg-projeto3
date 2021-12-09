#include "movements.hpp"

#include <cppitertools/itertools.hpp>
#include <iostream>

std::vector<Piece> Movements::drawMove(Piece& piece, std::vector<Piece> pieces, int playerTurn, int p1PiecesCount) {
    player = playerTurn;
    playersPieces = pieces;
    p1PiecesSize = p1PiecesCount;
    if (player == 1) {
        advance = 0.165f;
    }
    else {
        advance = -0.165f;
    }
    switch (piece.type)
    {
    case 'p':
        return movePawn(piece);
        break;
    case 'r':
        return moveRook(piece);
        break;
    case 'c':
        return moveKnight(piece);
        break;
    case 'b':
        return moveBishop(piece);
        break;
    case 'q':
        return moveQueen(piece);
        break;
    case 'k':
        return moveKing(piece);
        break;
    default:
        std::vector<Piece> empty;
        return empty;
        break;
    }
}

Piece Movements::isBlocked(Piece& p, std::vector<Piece> pieces) {
    for (auto& piece : pieces) {
        if (compareFloat(piece.xPos, p.xPos) && compareFloat(piece.yPos, p.yPos)) {
            // piece.blocked = true;
            p.blocked = true;
            return piece;
        }
    }
    p.blocked = false;
    return p;
}

void Movements::insertElement(Piece& next, char piece_type, std::vector<Piece>& pieceMoves) {
    next.modelMatrix = glm::mat4(1.0);
    next.type = piece_type;
    pieceMoves.push_back(next);
}

bool Movements::insertValidMove(Piece& current, std::vector<Piece>& pieceMoves, int orientation) {
    blockingPiece = isBlocked(next, playersPieces);
    if (next.blocked) {
        if (blockingPiece.color != current.color) {
            insertElement(next, 's', pieceMoves);
        }
		return false;
    }
	if (orientation == 1) {
		if ((player == 1 && next.xPos <= -boardEdge && next.yPos <= -boardEdge) || 
			(player == 2 && next.xPos >= boardEdge && next.yPos >= boardEdge)) {
				insertElement(next, 's', pieceMoves);
		}
		else {
			return false;
		}
	}
	else if (orientation == 2) {
		if ((player == 1 && next.xPos >= boardEdge && next.yPos <= -boardEdge) || 
            (player == 2 && next.xPos <= -boardEdge && next.yPos >= boardEdge)) {
                insertElement(next, 's', pieceMoves);
		}
		else {
			return false;
		}
	}
	else if (orientation == 3) {
		if ((player == 1 && next.xPos >= boardEdge && next.yPos >= boardEdge) || 
			(player == 2 && next.xPos <= -boardEdge && next.yPos <= -boardEdge)) {
				insertElement(next, 's', pieceMoves);
		}
		else {
			return false;
		}
	}
	else if (orientation == 4) {
		if ((player == 1 && next.xPos <= -boardEdge && next.yPos >= boardEdge) || 
            (player == 2 && next.xPos >= boardEdge && next.yPos <= -boardEdge)) {
                insertElement(next, 's', pieceMoves);
        }
	}
	return true;
}

std::vector<Piece> Movements::movePawn(Piece& pawn) {
    std::vector<Piece> pieceMoves;
    nMoves = 1;
    pieceMoves.reserve(nMoves);
	next = {};

    next.xPos = pawn.xPos;
    next.yPos = pawn.yPos + advance;
    next.zPos = z;
    if (pawn.firstMove) {
        nMoves = 2;
    }

	for (int i = 0; i < nMoves; i++) {
		if (!insertValidMove(pawn, pieceMoves, 1)) 
			break;
		next.yPos += advance;
	}
	if (next.blocked && pieceMoves.size() > 0) {
		pieceMoves.pop_back();
	}

    // pawn capture
    next.xPos = pawn.xPos + advance;
    next.yPos = pawn.yPos + advance;

    // possible captures
    Piece piece_capture = isBlocked(next, playersPieces);
    if (next.blocked && piece_capture.color != pawn.color)
        insertElement(next, 's', pieceMoves);
    
    next.xPos = pawn.xPos - advance;
    piece_capture = isBlocked(next, playersPieces);
    if (next.blocked && piece_capture.color != pawn.color)
        insertElement(next, 's', pieceMoves);
    
    return pieceMoves;
}

std::vector<Piece> Movements::moveKnight(Piece& knight) {
    std::vector<Piece> pieceMoves;
    nMoves = 8;
    pieceMoves.reserve(nMoves);
    next = {};
    
    // horizontal right
    next.xPos = knight.xPos + (2 * advance);
    next.yPos = knight.yPos + advance;
    next.zPos = z;

	insertValidMove(knight, pieceMoves, 1);

    next.yPos = knight.yPos - advance;
	insertValidMove(knight, pieceMoves, 4);

    // horizontal left
    next.xPos = knight.xPos - (2 * advance);
    next.yPos = knight.yPos + advance;
	insertValidMove(knight, pieceMoves, 2);

    next.yPos = knight.yPos - advance;
	insertValidMove(knight, pieceMoves, 3);

    // vertical up
    next.xPos = knight.xPos + advance;
    next.yPos = knight.yPos + (2 * advance);
	insertValidMove(knight, pieceMoves, 1);

    next.xPos = knight.xPos - advance;
	insertValidMove(knight, pieceMoves, 2);

    // vertical down
    next.xPos = knight.xPos + advance;
    next.yPos = knight.yPos - (2 * advance);
	insertValidMove(knight, pieceMoves, 4);

    next.xPos = knight.xPos - advance;
	insertValidMove(knight, pieceMoves, 3);
    
    return pieceMoves;
}


std::vector<Piece> Movements::moveRook(Piece& rook) {
    std::vector<Piece> pieceMoves;
    nMoves = 16;
    pieceMoves.reserve(nMoves);
	next = {};

    // right horizontal
    next.xPos = rook.xPos + advance;
    next.yPos = rook.yPos;
    next.zPos = z;

	for (int i = 0; i < nMoves; i++) {
		if (!insertValidMove(rook, pieceMoves, 1)) 
			break;
		next.xPos += advance;
	}

    // left horizontal
    next.xPos = rook.xPos - advance;
    next.yPos = rook.yPos;

	for (int i = 0; i < nMoves; i++) {
    	if (!insertValidMove(rook, pieceMoves, 3)) 
			break;
		next.xPos -= advance;
	}

    // vertical up
    next.xPos = rook.xPos;
    next.yPos = rook.yPos + advance;
    next.zPos = z;

	for (int i = 0; i < nMoves; i++) {
		if (!insertValidMove(rook, pieceMoves, 1)) 
			break;
		next.yPos += advance;
	}

    // vertical down
    next.xPos = rook.xPos;
    next.yPos = rook.yPos - advance;

	for (int i = 0; i < nMoves; i++) {
		if (!insertValidMove(rook, pieceMoves, 3))
			break;
		next.yPos -= advance;
	}

    return pieceMoves;
}

std::vector<Piece> Movements::moveBishop(Piece& bishop) {
    std::vector<Piece> pieceMoves;
    nMoves = 16;
    pieceMoves.reserve(nMoves);
    next = {};

    // right diagonal up
    next.xPos = bishop.xPos + advance;
    next.yPos = bishop.yPos + advance;
    next.zPos = z;
    for (int i = 0; i < nMoves; i++) {
		if (!insertValidMove(bishop, pieceMoves, 1)) 
			break;
		next.xPos += advance;
        next.yPos += advance;
    }

    // left diagonal up
    next.xPos = bishop.xPos - advance;
    next.yPos = bishop.yPos + advance;
	for (int i = 0; i < nMoves; i++) {
		if (!insertValidMove(bishop, pieceMoves, 2)) 
			break;
		next.xPos -= advance;
        next.yPos += advance;
	}

    // right diagonal down
    next.xPos = bishop.xPos + advance;
    next.yPos = bishop.yPos - advance;
    for (int i = 0; i < nMoves; i++) {
		if (!insertValidMove(bishop, pieceMoves, 4)) 
			break;
		next.xPos += advance;
        next.yPos -= advance;
	}

    // left diagonal down
    next.xPos = bishop.xPos - advance;
    next.yPos = bishop.yPos - advance;
    
	for (int i = 0; i < nMoves; i++) {
		if (!insertValidMove(bishop, pieceMoves, 3)) 
			break;
		next.xPos -= advance;
        next.yPos -= advance;
	}

    return pieceMoves;
}

std::vector<Piece> Movements::moveQueen(Piece& queen) {
    std::vector<Piece> pieceMoves_bishop = moveBishop(queen);
    std::vector<Piece> pieceMoves_rook = moveRook(queen);
    std::vector<Piece> pieceMoves;
    pieceMoves.reserve(pieceMoves_bishop.size() + pieceMoves_rook.size());
    pieceMoves.insert(pieceMoves.end(), pieceMoves_bishop.begin(), pieceMoves_bishop.end());
    pieceMoves.insert(pieceMoves.end(), pieceMoves_rook.begin(), pieceMoves_rook.end());
    return pieceMoves;
}

void Movements::cast(Piece& king, Piece& first, int pos, std::vector<Piece>& pieceMoves) {
	if ((king.firstMove && !first.blocked && !next.blocked)) {
		for (auto index : iter::range(static_cast<size_t>(pos), playersPieces.size())) {
			if (playersPieces[index].type == 'r' && playersPieces[index].color == king.color 
				&& playersPieces[index].firstMove) {
				insertElement(next, 's', pieceMoves);
            	king.castlingIndex = index;
				king.castlingX = next.xPos;
				break;
			}
		}
    }
}

std::vector<Piece> Movements::moveKing(Piece& king) {
    std::vector<Piece> pieceMoves;
    int nMoves = 8;
    pieceMoves.reserve(nMoves);
	next = {};
    
    // vertical up
    next.xPos = king.xPos;
    next.yPos = king.yPos + advance;
    next.zPos = z;
	insertValidMove(king, pieceMoves, 1);
    
    // right diagonal up
    next.xPos += advance;
	insertValidMove(king, pieceMoves, 1);
	
    // right horizontal
    next.yPos -= advance;
	insertValidMove(king, pieceMoves, 1);
    
    // right diagonal down
    next.yPos -= advance;
	insertValidMove(king, pieceMoves, 4);
    
    // vertical down
    next.xPos -= advance;
	insertValidMove(king, pieceMoves, 3);

    // left diagonal down
    next.xPos -= advance;
	insertValidMove(king, pieceMoves, 3);
	
    // left horizontal
    next.yPos += advance;
	insertValidMove(king, pieceMoves, 3);
    
    // left diagonal up
    next.yPos += advance;
	insertValidMove(king, pieceMoves, 2);
    
    // castling
    float first_s = king.xPos + (advance);
    float second_s = king.xPos + (2 * advance);

    float first_b = king.xPos - (advance);
    float second_b = king.xPos - (2 * advance);
    
    next.xPos = second_s;
    next.yPos = king.yPos;
    
    Piece first = next;
    first.xPos = first_s;

    blockingPiece = isBlocked(first, playersPieces);
    Piece blockingPiece2 = isBlocked(next, playersPieces);

    if (king.color == 'w')
        cast(king, first, 1, pieceMoves);
    else
        cast(king, first, p1PiecesSize + 1, pieceMoves);

    next.xPos = second_b;
    first.xPos = first_b;

    blockingPiece = isBlocked(first, playersPieces);
    blockingPiece2 = isBlocked(next, playersPieces);

    if (king.color == 'w')
	    cast(king, first, 0, pieceMoves);
    else
	    cast(king, first, p1PiecesSize, pieceMoves);

    return pieceMoves;
}