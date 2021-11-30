#include "movements.hpp"

#include <cppitertools/itertools.hpp>

std::vector<Piece> Movements::drawMove(Piece piece, std::vector<Piece> pieces, int player_turn) {
    player = player_turn;
    players_pieces = pieces;
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
        if (compareFloat(piece.x_pos, p.x_pos) && compareFloat(piece.y_pos, p.y_pos)) {
            // piece.blocked = true;
            p.blocked = true;
            return piece;
        }
    }
    p.blocked = false;
    return p;
}

void Movements::insertElement(Piece& next, char piece_type, std::vector<Piece>& piece_moves) {
    next.model_matrix = glm::mat4(1.0);
    next.type = piece_type;
    piece_moves.push_back(next);
}

std::vector<Piece> Movements::movePawn(Piece pawn) {
    std::vector<Piece> piece_moves;
    int n_moves = 1;
    piece_moves.reserve(n_moves);
    Piece next;
    next.x_pos = pawn.x_pos;
    next.y_pos = pawn.y_pos + advance;
    next.z_pos = z;
    if (pawn.first_move) {
        n_moves = 2;
    }

    for (int i = 0; i < n_moves; i++) {
        blocking = isBlocked(next, players_pieces);
        if (next.blocked)
            break;
        if ((player == 1 && next.y_pos <= -lim_y) || 
            (player == 2 && next.y_pos >= lim_y)) {
                insertElement(next, 's', piece_moves);
                next.y_pos = next.y_pos + advance;
        }
        else {
            break;
        }
    }

    // pawm capture
    next.x_pos = pawn.x_pos + advance;
    next.y_pos = pawn.y_pos + advance;
    // possible captures
    Piece piece_capture = isBlocked(next, players_pieces);
    if (next.blocked && piece_capture.color != pawn.color)
        insertElement(next, 's', piece_moves);
    
    next.x_pos = pawn.x_pos - advance;
    piece_capture = isBlocked(next, players_pieces);
    if (next.blocked && piece_capture.color != pawn.color)
        insertElement(next, 's', piece_moves);
    
    return piece_moves;
}

std::vector<Piece> Movements::moveRook(Piece rook) {
    std::vector<Piece> piece_moves;
    int n_moves = 16;
    piece_moves.reserve(n_moves);
    Piece next = {};

    // right horizontal
    next.x_pos = rook.x_pos + advance;
    next.y_pos = rook.y_pos;
    next.z_pos = z;

    for (int i = 0; i < n_moves; i++) {
        blocking = isBlocked(next, players_pieces);
        if (next.blocked) {
            if (blocking.color != rook.color) {
                insertElement(next, 's', piece_moves);
            }
            break;
        }
        if ((player == 1 && next.x_pos <= -lim_x) || 
            (player == 2 && next.x_pos >= lim_x)) {
                insertElement(next, 's', piece_moves);
                next.x_pos = next.x_pos + advance;
        }
        else {
            break;
        }
    }

    // left horizontal
    next.x_pos = rook.x_pos - advance;
    next.y_pos = rook.y_pos;
    for (int i = 0; i < n_moves; i++) {
        blocking = isBlocked(next, players_pieces);
        if (next.blocked) {
            if (blocking.color != rook.color) {
                insertElement(next, 's', piece_moves);
            }
            break;
        }
        if ((player == 1 && next.x_pos >= lim_x) || 
            (player == 2 && next.x_pos <= -lim_x)) {
                insertElement(next, 's', piece_moves);
                next.x_pos = next.x_pos - advance;
        }
        else {
            break;
        }
    }

    // vertical up
    next = {};
    next.x_pos = rook.x_pos;
    next.y_pos = rook.y_pos + advance;
    next.z_pos = z;
    for (int i = 0; i < n_moves; i++) {
        blocking = isBlocked(next, players_pieces);
        if (next.blocked) {
            if (blocking.color != rook.color) {
                insertElement(next, 's', piece_moves);
            }
            break;
        }
        if ((player == 1 && next.y_pos <= -lim_x) || 
            (player == 2 && next.y_pos >= lim_x)) {
                insertElement(next, 's', piece_moves);
                next.y_pos = next.y_pos + advance;
        }
        else {
            break;
        }
    }

    next.x_pos = rook.x_pos;
    next.y_pos = rook.y_pos - advance;

    for (int i = 0; i < n_moves; i++) {
        blocking = isBlocked(next, players_pieces);
        if (next.blocked) {
            if (blocking.color != rook.color) {
                insertElement(next, 's', piece_moves);
            }
            break;
        }
        if ((player == 1 && next.y_pos >= lim_x) || 
            (player == 2 && next.y_pos <= -lim_x)) {
                insertElement(next, 's', piece_moves);
                next.y_pos = next.y_pos - advance;
        }
        else {
            break;
        }
    }
    return piece_moves;
}

std::vector<Piece> Movements::moveKnight(Piece knight) {
    std::vector<Piece> piece_moves;
    int n_moves = 8;
    piece_moves.reserve(n_moves);
    Piece next = {};
    
    // horizontal right
    next.x_pos = knight.x_pos + (2 * advance);
    next.y_pos = knight.y_pos + advance;
    next.z_pos = z;
    blocking = isBlocked(next, players_pieces);

    if ((player == 1 && next.x_pos <= -lim_x && next.y_pos <= -lim_x) || 
        (player == 2 && next.x_pos >= lim_x && next.y_pos >= lim_x)) {
            if (next.blocked) {
                if (blocking.color != knight.color) {
                    insertElement(next, 's', piece_moves);
                }
            }
            else {
                insertElement(next, 's', piece_moves);
            }
    }

    next.y_pos = knight.y_pos - advance;
    blocking = isBlocked(next, players_pieces);

    if ((player == 1 && next.x_pos <= -lim_x && next.y_pos >= lim_x) || 
        (player == 2 && next.x_pos >= lim_x && next.y_pos <= -lim_x)) {
            if (next.blocked) {
                if (blocking.color != knight.color) {
                    insertElement(next, 's', piece_moves);
                }
            }
            else {
                insertElement(next, 's', piece_moves);
            }
    }

    // horizontal left
    next.x_pos = knight.x_pos - (2 * advance);
    next.y_pos = knight.y_pos + advance;
    blocking = isBlocked(next, players_pieces);

    if ((player == 1 && next.x_pos >= lim_x && next.y_pos <= -lim_x) || 
        (player == 2 && next.x_pos <= -lim_x && next.y_pos >= lim_x)) {
            if (next.blocked) {
                if (blocking.color != knight.color) {
                    insertElement(next, 's', piece_moves);
                }
            }
            else {
                insertElement(next, 's', piece_moves);
            }
    }

    next.y_pos = knight.y_pos - advance;
    blocking = isBlocked(next, players_pieces);
    if ((player == 1 && next.x_pos >= lim_x && next.y_pos >= lim_x) || 
        (player == 2 && next.x_pos <= -lim_x && next.y_pos <= -lim_x)) {
            if (next.blocked) {
                if (blocking.color != knight.color) {
                    insertElement(next, 's', piece_moves);
                }
            }
            else {
                insertElement(next, 's', piece_moves);
            }
    }
    
    // vertical up
    next.x_pos = knight.x_pos + advance;
    next.y_pos = knight.y_pos + (2 * advance);
    blocking = isBlocked(next, players_pieces);
    if ((player == 1 && next.x_pos <= -lim_x && next.y_pos <= -lim_x) || 
        (player == 2 && next.x_pos >= lim_x && next.y_pos >= lim_x)) {
            if (next.blocked) {
                if (blocking.color != knight.color) {
                    insertElement(next, 's', piece_moves);
                }
            }
            else {
                insertElement(next, 's', piece_moves);
            }
    }

    next.x_pos = knight.x_pos - advance;
    blocking = isBlocked(next, players_pieces);
    if ((player == 1 && next.x_pos >= lim_x && next.y_pos <= -lim_x) || 
        (player == 2 && next.x_pos <= -lim_x && next.y_pos >= lim_x)) {
            if (next.blocked) {
                if (blocking.color != knight.color) {
                    insertElement(next, 's', piece_moves);
                }
            }
            else {
                insertElement(next, 's', piece_moves);
            }
    }

    // vertical down
    next.x_pos = knight.x_pos + advance;
    next.y_pos = knight.y_pos - (2 * advance);
    blocking = isBlocked(next, players_pieces);

    if ((player == 1 && next.x_pos <= -lim_x && next.y_pos >= lim_x) || 
        (player == 2 && next.x_pos >= lim_x && next.y_pos <= -lim_x)) {
            if (next.blocked) {
                if (blocking.color != knight.color) {
                    insertElement(next, 's', piece_moves);
                }
            }
            else {
                insertElement(next, 's', piece_moves);
            }
    }

    next.x_pos = knight.x_pos - advance;
    blocking = isBlocked(next, players_pieces);
    if ((player == 1 && next.x_pos >= lim_x && next.y_pos >= lim_x) || 
        (player == 2 && next.x_pos <= -lim_x && next.y_pos <= -lim_x)) {
            if (next.blocked) {
                if (blocking.color != knight.color) {
                    insertElement(next, 's', piece_moves);
                }
            }
            else {
                insertElement(next, 's', piece_moves);
            }
    }
    
    return piece_moves;
}

std::vector<Piece> Movements::moveBishop(Piece bishop) {
    std::vector<Piece> piece_moves;
    int n_moves = 16;
    piece_moves.reserve(n_moves);
    Piece next = {};

    // right diagonal up
    next.x_pos = bishop.x_pos + advance;
    next.y_pos = bishop.y_pos + advance;
    next.z_pos = z;
    for (int i = 0; i < n_moves; i++) {
        blocking = isBlocked(next, players_pieces);
        if (next.blocked) {
            if (blocking.color != bishop.color) {
                insertElement(next, 's', piece_moves);
            }
            break;
        }
        if ((player == 1 && next.x_pos <= -lim_x && next.y_pos <= -lim_x) || 
            (player == 2 && next.x_pos >= lim_x && next.y_pos >= lim_x)) {
                insertElement(next, 's', piece_moves);
                next.x_pos = next.x_pos + advance;
                next.y_pos = next.y_pos + advance;
        }
        else {
            break;
        }
    }

    // left diagonal up
    next.x_pos = bishop.x_pos - advance;
    next.y_pos = bishop.y_pos + advance;
    for (int i = 0; i < n_moves; i++) {
        blocking = isBlocked(next, players_pieces);
        if (next.blocked) {
            if (blocking.color != bishop.color) {
                insertElement(next, 's', piece_moves);
            }
            break;
        }
        if ((player == 1 && next.x_pos >= lim_x && next.y_pos <= -lim_x) || 
            (player == 2 && next.x_pos <= -lim_x && next.y_pos >= lim_x)) {
                insertElement(next, 's', piece_moves);
                next.x_pos = next.x_pos - advance;
                next.y_pos = next.y_pos + advance;
        }
        else {
            break;
        }
    }

    // right diagonal down
    next.x_pos = bishop.x_pos + advance;
    next.y_pos = bishop.y_pos - advance;
    for (int i = 0; i < n_moves; i++) {
        blocking = isBlocked(next, players_pieces);
        if (next.blocked) {
            if (blocking.color != bishop.color) {
                insertElement(next, 's', piece_moves);
            }
            break;
        }
        if ((player == 1 && next.x_pos <= -lim_x && next.y_pos >= lim_x) || 
            (player == 2 && next.x_pos >= lim_x && next.y_pos <= -lim_x)) {
                insertElement(next, 's', piece_moves);
                next.x_pos = next.x_pos + advance;
                next.y_pos = next.y_pos - advance;
        }
        else {
            break;
        }
    }
    // left diagonal down
    next.x_pos = bishop.x_pos - advance;
    next.y_pos = bishop.y_pos - advance;
    next.z_pos = z;

    for (int i = 0; i < n_moves; i++) {
        blocking = isBlocked(next, players_pieces);
        if (next.blocked) {
            if (blocking.color != bishop.color) {
                insertElement(next, 's', piece_moves);
            }
            break;
        }
        if ((player == 1 && next.x_pos >= lim_x && next.y_pos >= lim_x) || 
            (player == 2 && next.x_pos <= -lim_x && next.y_pos <= -lim_x)) {
                insertElement(next, 's', piece_moves);
                next.x_pos = next.x_pos - advance;
                next.y_pos = next.y_pos - advance;
        }
        else {
            break;
        }
    }

    return piece_moves;
}

std::vector<Piece> Movements::moveQueen(Piece queen) {
    std::vector<Piece> piece_moves_bishop = moveBishop(queen);
    std::vector<Piece> piece_moves_rook = moveRook(queen);
    std::vector<Piece> piece_moves;
    piece_moves.reserve(piece_moves_bishop.size() + piece_moves_rook.size());
    piece_moves.insert(piece_moves.end(), piece_moves_bishop.begin(), piece_moves_bishop.end());
    piece_moves.insert(piece_moves.end(), piece_moves_rook.begin(), piece_moves_rook.end());
    return piece_moves;
}

std::vector<Piece> Movements::moveKing(Piece king) {
    std::vector<Piece> piece_moves;
    int n_moves = 8;
    piece_moves.reserve(n_moves);
    Piece next = {};
    
    // vertical up
    next.x_pos = king.x_pos;
    next.y_pos = king.y_pos + advance;
    next.z_pos = z;
    blocking = isBlocked(next, players_pieces);
    if ((player == 1 && next.y_pos <= -lim_x) || 
        (player == 2 && next.y_pos >= lim_x)) {
            if (next.blocked) {
                if (blocking.color != king.color) {
                    insertElement(next, 's', piece_moves);
                }
            }
            else {
                insertElement(next, 's', piece_moves);
            }
    }
    
    // right diagonal up
    next.x_pos += advance;
    blocking = isBlocked(next, players_pieces);
    if ((player == 1 && next.x_pos >= lim_x && next.y_pos >= lim_x) || 
        (player == 2 && next.x_pos >= lim_x && next.y_pos >= lim_x)) {
            if (next.blocked) {
                if (blocking.color != king.color) {
                    insertElement(next, 's', piece_moves);
                }
            }
            else {
                insertElement(next, 's', piece_moves);
            }
    }

    // right horizontal
    next.y_pos -= advance;
    blocking = isBlocked(next, players_pieces);
    if ((player == 1 && next.y_pos >= lim_x && next.x_pos <= -lim_x) || 
        (player == 2 && next.y_pos <= -lim_x && next.x_pos >= lim_x)) {
            if (next.blocked) {
                if (blocking.color != king.color) {
                    insertElement(next, 's', piece_moves);
                }
            }
            else {
                insertElement(next, 's', piece_moves);
            }
    }
    
    // right diagonal down
    next.y_pos -= advance;
    blocking = isBlocked(next, players_pieces);

    if ((player == 1 && next.y_pos >= lim_x && next.x_pos <= -lim_x) || 
        (player == 2 && next.y_pos <= -lim_x && next.x_pos >= lim_x)) {
            if (next.blocked) {
                if (blocking.color != king.color) {
                    insertElement(next, 's', piece_moves);
                }
            }
            else {
                insertElement(next, 's', piece_moves);
            }
    }

    // vertical down
    next.x_pos -= advance;
    blocking = isBlocked(next, players_pieces);

    if ((player == 1 && next.y_pos >= lim_x && next.x_pos >= lim_x) || 
        (player == 2 && next.y_pos <= -lim_x && next.x_pos <= -lim_x)) {
            if (next.blocked) {
                if (blocking.color != king.color) {
                    insertElement(next, 's', piece_moves);
                }
            }
            else {
                insertElement(next, 's', piece_moves);
            }
    }

    // left diagonal down
    next.x_pos -= advance;
    blocking = isBlocked(next, players_pieces);

    if ((player == 1 && next.y_pos >= lim_x && next.x_pos >= lim_x) || 
        (player == 2 && next.y_pos <= -lim_x && next.x_pos <= -lim_x)) {
            if (next.blocked) {
                if (blocking.color != king.color) {
                    insertElement(next, 's', piece_moves);
                }
            }
            else {
                insertElement(next, 's', piece_moves);
            }
    }

    // left horizontal
    next.y_pos += advance;
    blocking = isBlocked(next, players_pieces);

    if ((player == 1 && next.y_pos <= -lim_x && next.x_pos >= lim_x) || 
        (player == 2 && next.y_pos >= lim_x && next.x_pos <= -lim_x)) {
            if (next.blocked) {
                if (blocking.color != king.color) {
                    insertElement(next, 's', piece_moves);
                }
            }
            else {
                insertElement(next, 's', piece_moves);
            }
    }

    // left diagonal up
    next.y_pos += advance;
    blocking = isBlocked(next, players_pieces);

    if ((player == 1 && next.y_pos <= -lim_x && next.x_pos >= lim_x) || 
        (player == 2 && next.y_pos >= lim_x && next.x_pos <= -lim_x)) {
            if (next.blocked) {
                if (blocking.color != king.color) {
                    insertElement(next, 's', piece_moves);
                }
            }
            else {
                insertElement(next, 's', piece_moves);
            }
    }

    // castling
    float first_s = king.x_pos + (advance);
    float second_s = king.x_pos + (2 * advance);

    float first_b = king.x_pos - (advance);
    float second_b = king.x_pos - (2 * advance);
    
    next = {};
    next.x_pos = second_s;
    next.y_pos = king.y_pos;
    next.z_pos = z;

    Piece next1 = {};
    next1.x_pos = first_s;
    next1.y_pos = king.y_pos;
    next1.z_pos = z;
    blocking = isBlocked(next1, players_pieces);
    Piece blocking2 = isBlocked(next, players_pieces);

    if ((king.first_move && !king.blocked)) {
        if ((players_pieces)[king.index + 3].type == 'r' && (players_pieces)[king.index + 3].first_move) {
            next.model_matrix = glm::mat4(1.0);
            next.type = 's';
            piece_moves.push_back(next);
            (players_pieces)[king.index].castling_index = king.index + 3;
        }
    }
    next.x_pos = second_b;
    next1.x_pos = first_b;
    blocking = isBlocked(next1, players_pieces);
    blocking2 = isBlocked(next, players_pieces);
    if ((king.first_move && king.blocked)) {
        if ((players_pieces)[king.index - 4].type == 'r' && (players_pieces)[king.index - 4].first_move) {
            next.model_matrix = glm::mat4(1.0);
            next.type = 's';
            piece_moves.push_back(next);
            (players_pieces)[king.index].castling_index = king.index - 4;
        }
    }
    return piece_moves;
}