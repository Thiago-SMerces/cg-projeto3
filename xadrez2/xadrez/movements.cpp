#include "movements.hpp"

#include <cppitertools/itertools.hpp>

std::vector<Piece> Movements::drawMove(Piece piece, std::vector<Piece> all_pieces, int player_turn) {
    this->all_pieces = all_pieces;
    player = player_turn;
    if (player == 1) {
        // lim_x = -0.58f;
        // lim_y = -0.58f;
        advance = 0.165f;
    }
    else {
        // lim_x = -0.58f;
        // lim_y = 0.58f;
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

bool isBlocked(Piece p, std::vector<Piece> all_pieces) {
    for (auto& piece : all_pieces) {
        if (compareFloat(piece.x_pos, p.x_pos) && compareFloat(piece.y_pos, p.y_pos)) {
            return true;
        }
    }
    return false;
}

bool isBlocked(float x, float y, std::vector<Piece> all_pieces) {
    for (auto& piece : all_pieces) {
        if (compareFloat(piece.x_pos, x) && compareFloat(piece.y_pos, y)) {
            return true;
        }
    }
    return false;
}

std::vector<Piece> Movements::movePawn(Piece pawn) {
    std::vector<Piece> piece_moves;
    int n_moves = 2;
    piece_moves.reserve(n_moves);
    Piece next;
    next.x_pos = pawn.x_pos;
    next.y_pos = pawn.y_pos + advance;
    next.z_pos = 0.04f;
    if (pawn.first_move) {
        for (int i = 0; i < n_moves; i++) {
            if (isBlocked(next, all_pieces))
                break;
            if (next.y_pos <= -lim_y) {
                next.model_matrix = glm::mat4(1.0);
                next.type = 's';
                piece_moves.push_back(next);
                next.x_pos = next.x_pos;
                next.y_pos = next.y_pos + advance;
            }
            else {
                break;
            }
        }
    }
    else {
        if (next.y_pos <= 0.65f) {
            if (!isBlocked(next, all_pieces)) {
                next.model_matrix = glm::mat4(1.0);
                next.type = 's';
                piece_moves.push_back(next);
            }
        }
    }
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
    next.z_pos = 0.04f;
    for (int i = 0; i < n_moves; i++) {
        if (isBlocked(next, all_pieces))
            break;
        if (next.x_pos <= -lim_x) {
            next.model_matrix = glm::mat4(1.0);
            next.type = 's';
            piece_moves.push_back(next);
            next.x_pos = next.x_pos + advance;
            next.y_pos = next.y_pos;
        }
        else {
            break;
        }
    }

    // left horizontal
    next = {};
    next.x_pos = rook.x_pos - advance;
    next.y_pos = rook.y_pos;
    next.z_pos = 0.04f;
    for (int i = 0; i < n_moves; i++) {
        if (isBlocked(next, all_pieces))
            break;
        if (next.x_pos >= lim_x) {
            next.model_matrix = glm::mat4(1.0);
            next.type = 's';
            piece_moves.push_back(next);
            next.x_pos = next.x_pos - advance;
            next.y_pos = next.y_pos;
        }
        else {
            break;
        }
    }

    // vertical up
    next = {};
    next.x_pos = rook.x_pos;
    next.y_pos = rook.y_pos + advance;
    next.z_pos = 0.04f;
    for (int i = 0; i < n_moves; i++) {
        if (isBlocked(next, all_pieces))
            break;
        if (next.y_pos <= -lim_x) {
            next.model_matrix = glm::mat4(1.0);
            next.type = 's';
            piece_moves.push_back(next);
            next.x_pos = next.x_pos;
            next.y_pos = next.y_pos + advance;
        }
        else {
            break;
        }
    }
    // vertical down
    next = {};
    next.x_pos = rook.x_pos;
    next.y_pos = rook.y_pos - advance;
    next.z_pos = 0.04f;
    for (int i = 0; i < n_moves; i++) {
        if (isBlocked(next, all_pieces))
            break;
        if (next.y_pos >= lim_x) {
            next.model_matrix = glm::mat4(1.0);
            next.type = 's';
            piece_moves.push_back(next);
            next.x_pos = next.x_pos;
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
    next.z_pos = 0.04f;
    if (!isBlocked(next.x_pos, next.y_pos, all_pieces) && next.x_pos <= -lim_x && next.y_pos <= -lim_x) {
        next.model_matrix = glm::mat4(1.0);
        next.type = 's';
        piece_moves.push_back(next);
    }

    next.y_pos = knight.y_pos - advance;
    if (!isBlocked(next.x_pos, next.y_pos, all_pieces) && next.x_pos <= -lim_x && next.y_pos >= lim_x) {
        next.model_matrix = glm::mat4(1.0);
        next.type = 's';
        piece_moves.push_back(next);
    }

    // horizontal left
    next.x_pos = knight.x_pos - (2 * advance);
    next.y_pos = knight.y_pos + advance;
    if (!isBlocked(next.x_pos, next.y_pos, all_pieces) && next.x_pos >=- -lim_x && next.y_pos <= -lim_x) {
        next.model_matrix = glm::mat4(1.0);
        next.type = 's';
        piece_moves.push_back(next);
    }

    next.y_pos = knight.y_pos - advance;
    if (!isBlocked(next.x_pos, next.y_pos, all_pieces) && next.x_pos >= lim_x && next.y_pos >= lim_x) {
        next.model_matrix = glm::mat4(1.0);
        next.type = 's';
        piece_moves.push_back(next);
    }

    // vertical up
    next.x_pos = knight.x_pos + advance;
    next.y_pos = knight.y_pos + (2 * advance);
    if (!isBlocked(next.x_pos, next.y_pos, all_pieces) && next.x_pos <= -lim_x && next.y_pos <= -lim_x) {
        next.model_matrix = glm::mat4(1.0);
        next.type = 's';
        piece_moves.push_back(next);
    }

    next.x_pos = knight.x_pos - advance;
    if (!isBlocked(next.x_pos, next.y_pos, all_pieces) && next.x_pos >= lim_x && next.y_pos <= -lim_x) {
        next.model_matrix = glm::mat4(1.0);
        next.type = 's';
        piece_moves.push_back(next);
    }

    // vertical down
    next.x_pos = knight.x_pos + advance;
    next.y_pos = knight.y_pos - (2 * advance);
    if (!isBlocked(next.x_pos, next.y_pos, all_pieces) && next.x_pos <= -lim_x && next.y_pos >= lim_x) {
        next.model_matrix = glm::mat4(1.0);
        next.type = 's';
        piece_moves.push_back(next);
    }

    next.x_pos = knight.x_pos - advance;
    if (!isBlocked(next.x_pos, next.y_pos, all_pieces) && next.x_pos >= lim_x && next.y_pos >= lim_x) {
        next.model_matrix = glm::mat4(1.0);
        next.type = 's';
        piece_moves.push_back(next);
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
    next.z_pos = 0.04f;
    for (int i = 0; i < n_moves; i++) {
        if (isBlocked(next, all_pieces))
            break;
        if (next.x_pos <= -lim_x && next.y_pos <= -lim_x) {
            next.model_matrix = glm::mat4(1.0);
            next.type = 's';
            piece_moves.push_back(next);
            next.x_pos = next.x_pos + advance;
            next.y_pos = next.y_pos + advance;
        }
        else {
            break;
        }
    }

    // left diagonal up
    next = {};
    next.x_pos = bishop.x_pos - advance;
    next.y_pos = bishop.y_pos + advance;
    next.z_pos = 0.04f;
    for (int i = 0; i < n_moves; i++) {
        if (isBlocked(next, all_pieces))
            break;
        if (next.x_pos >= lim_x && next.y_pos <= -lim_x) {
            next.model_matrix = glm::mat4(1.0);
            next.type = 's';
            piece_moves.push_back(next);
            next.x_pos = next.x_pos - advance;
            next.y_pos = next.y_pos + advance;
        }
        else {
            break;
        }
    }

    // right diagonal down
    next = {};
    next.x_pos = bishop.x_pos + advance;
    next.y_pos = bishop.y_pos - advance;
    next.z_pos = 0.04f;
    for (int i = 0; i < n_moves; i++) {
        if (isBlocked(next, all_pieces))
            break;
        if (next.x_pos <= -lim_x && next.y_pos >= lim_x) {
            next.model_matrix = glm::mat4(1.0);
            next.type = 's';
            piece_moves.push_back(next);
            next.x_pos = next.x_pos + advance;
            next.y_pos = next.y_pos - advance;
        }
        else {
            break;
        }
    }
    // left diagonal down
    next = {};
    next.x_pos = bishop.x_pos - advance;
    next.y_pos = bishop.y_pos - advance;
    next.z_pos = 0.04f;
    for (int i = 0; i < n_moves; i++) {
        if (isBlocked(next, all_pieces))
            break;
        if (next.x_pos >= lim_x && next.y_pos >= lim_x) {
            next.model_matrix = glm::mat4(1.0);
            next.type = 's';
            piece_moves.push_back(next);
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
    next.z_pos = 0.04f;
    if (!isBlocked(next, all_pieces) && next.y_pos <= -lim_x) {
        next.model_matrix = glm::mat4(1.0);
        next.type = 's';
        piece_moves.push_back(next);
    }
    
    // right diagonal up
    next.x_pos += advance;
    if (!isBlocked(next, all_pieces) && next.x_pos <= -lim_x && next.y_pos <= -lim_x) {
        next.model_matrix = glm::mat4(1.0);
        next.type = 's';
        piece_moves.push_back(next);
    }

    // right horizontal
    next.y_pos -= advance;
    if (!isBlocked(next, all_pieces) && next.y_pos >= lim_x && next.x_pos <= -lim_x) {
        next.model_matrix = glm::mat4(1.0);
        next.type = 's';
        piece_moves.push_back(next);
    }
    
    // right diagonal down
    next.y_pos -= advance;
    if (!isBlocked(next, all_pieces) && next.y_pos >= lim_x && next.x_pos <= -lim_x) {
        next.model_matrix = glm::mat4(1.0);
        next.type = 's';
        piece_moves.push_back(next);
    }

    // vertical down
    next.x_pos -= advance;
    if (!isBlocked(next, all_pieces) && next.x_pos >= lim_x && next.y_pos >= lim_x) {
        next.model_matrix = glm::mat4(1.0);
        next.type = 's';
        piece_moves.push_back(next);
    }

    // left diagonal down
    next.x_pos -= advance;
    if (!isBlocked(next, all_pieces) && next.x_pos >= lim_x && next.y_pos >= lim_x) {
        next.model_matrix = glm::mat4(1.0);
        next.type = 's';
        piece_moves.push_back(next);
    }

    // left horizontal
    next.y_pos += advance;
    if (!isBlocked(next, all_pieces) && next.y_pos <= -lim_x && next.x_pos >= lim_x) {
        next.model_matrix = glm::mat4(1.0);
        next.type = 's';
        piece_moves.push_back(next);
    }

    // left diagonal up
    next.y_pos += advance;
    if (!isBlocked(next, all_pieces) && next.y_pos <= -lim_x && next.x_pos >= lim_x) {
        next.model_matrix = glm::mat4(1.0);
        next.type = 's';
        piece_moves.push_back(next);
    }

    // castling
    float first = king.x_pos + (advance);
    float second = king.x_pos + (2 * advance);
    
    next.x_pos = second;
    next.y_pos = king.y_pos;
    if (king.first_move && !isBlocked(first, king.y_pos, all_pieces) && !isBlocked(second, king.y_pos, all_pieces)) {
        for (auto& piece : all_pieces) {
            if (piece.type == 'r' && piece.first_move) {
                next.model_matrix = glm::mat4(1.0);
                next.type = 's';
                piece_moves.push_back(next);
            }
        }
    }
    return piece_moves;
}