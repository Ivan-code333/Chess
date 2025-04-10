// board.cpp
#include "board.h"
#include <sstream>

Board::Board() {
    setupNewGame();
}

void Board::setupNewGame() {
    clear();

    // Установка пешек
    for (int x = 0; x < 8; x++) {
        setPiece(Position(x, 1), Piece(PieceType::PAWN, Color::BLACK));
        setPiece(Position(x, 6), Piece(PieceType::PAWN, Color::WHITE));
    }

    // Установка ладей
    setPiece(Position(0, 0), Piece(PieceType::ROOK, Color::BLACK));
    setPiece(Position(7, 0), Piece(PieceType::ROOK, Color::BLACK));
    setPiece(Position(0, 7), Piece(PieceType::ROOK, Color::WHITE));
    setPiece(Position(7, 7), Piece(PieceType::ROOK, Color::WHITE));

    // Установка коней
    setPiece(Position(1, 0), Piece(PieceType::KNIGHT, Color::BLACK));
    setPiece(Position(6, 0), Piece(PieceType::KNIGHT, Color::BLACK));
    setPiece(Position(1, 7), Piece(PieceType::KNIGHT, Color::WHITE));
    setPiece(Position(6, 7), Piece(PieceType::KNIGHT, Color::WHITE));

    // Установка слонов
    setPiece(Position(2, 0), Piece(PieceType::BISHOP, Color::BLACK));
    setPiece(Position(5, 0), Piece(PieceType::BISHOP, Color::BLACK));
    setPiece(Position(2, 7), Piece(PieceType::BISHOP, Color::WHITE));
    setPiece(Position(5, 7), Piece(PieceType::BISHOP, Color::WHITE));

    // Установка ферзей
    setPiece(Position(3, 0), Piece(PieceType::QUEEN, Color::BLACK));
    setPiece(Position(3, 7), Piece(PieceType::QUEEN, Color::WHITE));

    // Установка королей
    setPiece(Position(4, 0), Piece(PieceType::KING, Color::BLACK));
    setPiece(Position(4, 7), Piece(PieceType::KING, Color::WHITE));

    // Сброс флагов рокировки
    whiteKingMoved = false;
    blackKingMoved = false;
    whiteKingsideRookMoved = false;
    whiteQueensideRookMoved = false;
    blackKingsideRookMoved = false;
    blackQueensideRookMoved = false;

    // Сброс target для взятия на проходе
    enPassantTarget = Position();

    // Очистка истории ходов
    moveHistory.clear();
}

void Board::clear() {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            pieces[y][x] = Piece();
        }
    }
}

Piece Board::getPiece(const Position& pos) const {
    if (!pos.isValid()) return Piece();
    return pieces[pos.y][pos.x];
}

void Board::setPiece(const Position& pos, const Piece& piece) {
    if (pos.isValid()) {
        pieces[pos.y][pos.x] = piece;
    }
}

void Board::removePiece(const Position& pos) {
    if (pos.isValid()) {
        pieces[pos.y][pos.x] = Piece();
    }
}

bool Board::makeMove(const Move& move) {
    if (!move.isValid()) return false;

    Position from = move.getFrom();
    Position to = move.getTo();
    Piece movingPiece = getPiece(from);

    // Проверяем наличие фигуры
    if (movingPiece.isEmpty()) return false;

    // Обновляем флаги для рокировки
    if (movingPiece.getType() == PieceType::KING) {
        if (movingPiece.getColor() == Color::WHITE) {
            whiteKingMoved = true;
        } else {
            blackKingMoved = true;
        }
    } else if (movingPiece.getType() == PieceType::ROOK) {
        if (from.x == 0 && from.y == 7) whiteQueensideRookMoved = true;
        if (from.x == 7 && from.y == 7) whiteKingsideRookMoved = true;
        if (from.x == 0 && from.y == 0) blackQueensideRookMoved = true;
        if (from.x == 7 && from.y == 0) blackKingsideRookMoved = true;
    }

    // Обработка специальных ходов
    switch (move.getMoveType()) {
        case MoveType::NORMAL:
        case MoveType::CAPTURE:
            // Стандартное перемещение
            setPiece(to, movingPiece);
            removePiece(from);
            break;

        case MoveType::PROMOTION:
            // Повышение пешки
            setPiece(to, Piece(move.getPromotionType(), movingPiece.getColor()));
            removePiece(from);
            break;

        case MoveType::CASTLE_KINGSIDE:
            // Короткая рокировка
            setPiece(to, movingPiece);
            removePiece(from);
            // Перемещаем ладью
            if (movingPiece.getColor() == Color::WHITE) {
                setPiece(Position(5, 7), getPiece(Position(7, 7)));
                removePiece(Position(7, 7));
            } else {
                setPiece(Position(5, 0), getPiece(Position(7, 0)));
                removePiece(Position(7, 0));
            }
            break;

        case MoveType::CASTLE_QUEENSIDE:
            // Длинная рокировка
            setPiece(to, movingPiece);
            removePiece(from);
            // Перемещаем ладью
            if (movingPiece.getColor() == Color::WHITE) {
                setPiece(Position(3, 7), getPiece(Position(0, 7)));
                removePiece(Position(0, 7));
            } else {
                setPiece(Position(3, 0), getPiece(Position(0, 0)));
                removePiece(Position(0, 0));
            }
            break;

        case MoveType::EN_PASSANT:
            // Взятие на проходе
            setPiece(to, movingPiece);
            removePiece(from);
            // Убираем взятую пешку
            removePiece(Position(to.x, from.y));
            break;

        default:
            return false;
    }

    // Отмечаем, что фигура двигалась
    getPiece(to).setHasMoved(true);

    // Обновляем target для взятия на проходе
    enPassantTarget = Position();
    if (movingPiece.getType() == PieceType::PAWN) {
        int dy = to.y - from.y;
        if (abs(dy) == 2) {  // Ход на две клетки
            enPassantTarget = Position(to.x, from.y + dy/2);
        }
    }

    // Сохраняем ход в истории
    moveHistory.push_back(move);

    return true;
}

void Board::undoLastMove() {
    // Реализация отмены последнего хода (для простоты опустим)
    // В полной реализации потребуется сохранять больше информации о ходе
}

bool Board::isValidMove(const Move& move, Color currentPlayer) const {
    // Базовые проверки
    if (!move.isValid()) return false;

    Position from = move.getFrom();
    Position to = move.getTo();

    // Проверка наличия фигуры и принадлежности к текущему игроку
    Piece piece = getPiece(from);
    if (piece.isEmpty() || piece.getColor() != currentPlayer) return false;

    // Получение всех возможных ходов для этой фигуры
    std::vector<Move> possibleMoves = generatePieceMoves(from);

    // Проверка наличия хода в списке возможных
    for (const auto& possibleMove : possibleMoves) {
        if (move == possibleMove) {
            // Проверка, не приводит ли ход к шаху своему королю
            return !moveResultsInCheck(move, currentPlayer);
        }
    }

    return false;
}

std::vector<Move> Board::getAllLegalMoves(Color currentPlayer) const {
    std::vector<Move> legalMoves;

    // Перебираем все клетки доски
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Position pos(x, y);
            Piece piece = getPiece(pos);

            // Если нашли фигуру текущего игрока
            if (!piece.isEmpty() && piece.getColor() == currentPlayer) {
                // Получаем все возможные ходы для этой фигуры
                std::vector<Move> pieceMoves = generatePieceMoves(pos);

                // Проверяем каждый ход на легальность (не приводит ли к шаху)
                for (const auto& move : pieceMoves) {
                    if (!moveResultsInCheck(move, currentPlayer)) {
                        legalMoves.push_back(move);
                    }
                }
            }
        }
    }

    return legalMoves;
}

bool Board::isCheck(Color color) const {
    // Находим положение короля
    Position kingPos = findKing(color);
    if (!kingPos.isValid()) return false;

    // Проверяем, атакована ли эта позиция фигурами противоположного цвета
    Color opponentColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    return isUnderAttack(kingPos, opponentColor);
}

bool Board::isCheckmate(Color color) const {
    // Если не шах, то не мат
    if (!isCheck(color)) return false;

    // Проверяем, есть ли ходы, спасающие от шаха
    return getAllLegalMoves(color).empty();
}

bool Board::isStalemate(Color color) const {
    // Если шах, то не пат
    if (isCheck(color)) return false;

    // Пат - когда нет возможных ходов, но король не под шахом
    return getAllLegalMoves(color).empty();
}

bool Board::isGameOver() const {
    // Игра окончена, если мат или пат любой из сторон
    return isCheckmate(Color::WHITE) || isCheckmate(Color::BLACK) ||
           isStalemate(Color::WHITE) || isStalemate(Color::BLACK);
}

std::string Board::toFEN() const {
    // Реализация генерации FEN-нотации
    std::stringstream ss;

    // Позиции фигур
    for (int y = 0; y < 8; y++) {
        int emptyCount = 0;
        for (int x = 0; x < 8; x++) {
            Piece piece = getPiece(Position(x, y));
            if (piece.isEmpty()) {
                emptyCount++;
            } else {
                if (emptyCount > 0) {
                    ss << emptyCount;
                    emptyCount = 0;
                }
                ss << piece.getSymbol();
            }
        }
        if (emptyCount > 0) {
            ss << emptyCount;
        }
        if (y < 7) {
            ss << "/";
        }
    }

    // Остальные части FEN (активный цвет, возможности рокировки, и т.д.)
    // Для краткости опущены

    return ss.str();
}

Move Board::getLastMove() const {
    if (moveHistory.empty()) return Move();
    return moveHistory.back();
}

const std::vector<Move>& Board::getMoveHistory() const {
    return moveHistory;
}

Position Board::findKing(Color color) const {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Position pos(x, y);
            Piece piece = getPiece(pos);
            if (piece.getType() == PieceType::KING && piece.getColor() == color) {
                return pos;
            }
        }
    }
    return Position(); // Invalid position if not found
}

std::vector<Move> Board::generatePieceMoves(const Position& pos) const {
    Piece piece = getPiece(pos);
    if (piece.isEmpty()) return {};

    switch (piece.getType()) {
        case PieceType::PAWN: return generatePawnMoves(pos);
        case PieceType::KNIGHT: return generateKnightMoves(pos);
        case PieceType::BISHOP: return generateBishopMoves(pos);
        case PieceType::ROOK: return generateRookMoves(pos);
        case PieceType::QUEEN: return generateQueenMoves(pos);
        case PieceType::KING: return generateKingMoves(pos);
        default: return {};
    }
}

bool Board::isUnderAttack(const Position& pos, Color attackerColor) const {
    // Проверка для каждой фигуры атакующего цвета, может ли она атаковать позицию
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            Position attackerPos(x, y);
            Piece attacker = getPiece(attackerPos);

            if (!attacker.isEmpty() && attacker.getColor() == attackerColor) {
                std::vector<Move> moves = generatePieceMoves(attackerPos);

                for (const auto& move : moves) {
                    if (move.getTo() == pos) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Board::moveResultsInCheck(const Move& move, Color currentPlayer) const {
    // Создаем временную копию доски
    Board tempBoard = *this;

    // Выполняем ход на временной доске
    tempBoard.makeMove(move);

    // Проверяем, не находится ли король под шахом после хода
    return tempBoard.isCheck(currentPlayer);
}

// Реализация генераторов ходов для каждого типа фигур
// Для краткости, реализуем только некоторые базовые правила

std::vector<Move> Board::generatePawnMoves(const Position& pos) const {
    std::vector<Move> moves;
    Piece pawn = getPiece(pos);
    if (pawn.isEmpty() || pawn.getType() != PieceType::PAWN) return moves;

    int direction = (pawn.getColor() == Color::WHITE) ? -1 : 1;
    int startRow = (pawn.getColor() == Color::WHITE) ? 6 : 1;
    int promotionRow = (pawn.getColor() == Color::WHITE) ? 0 : 7;

    // Ход вперед на одну клетку
    Position oneStep(pos.x, pos.y + direction);
    if (oneStep.isValid() && getPiece(oneStep).isEmpty()) {
        // Проверка на повышение
        if (oneStep.y == promotionRow) {
            moves.push_back(Move(pos, oneStep, MoveType::PROMOTION, PieceType::QUEEN));
            moves.push_back(Move(pos, oneStep, MoveType::PROMOTION, PieceType::ROOK));
            moves.push_back(Move(pos, oneStep, MoveType::PROMOTION, PieceType::BISHOP));
            moves.push_back(Move(pos, oneStep, MoveType::PROMOTION, PieceType::KNIGHT));
        } else {
            moves.push_back(Move(pos, oneStep));
        }

        // Ход вперед на две клетки с начальной позиции
        if (!pawn.getHasMoved() && pos.y == startRow) {
            Position twoStep(pos.x, pos.y + 2 * direction);
            if (twoStep.isValid() && getPiece(twoStep).isEmpty()) {
                moves.push_back(Move(pos, twoStep));
            }
        }
    }

    // Взятие по диагонали
    for (int dx = -1; dx <= 1; dx += 2) {
        Position capture(pos.x + dx, pos.y + direction);
        if (capture.isValid()) {
            Piece targetPiece = getPiece(capture);

            // Обычное взятие
            if (!targetPiece.isEmpty() && targetPiece.getColor() != pawn.getColor()) {
                if (capture.y == promotionRow) {
                    // Повышение при взятии
                    moves.push_back(Move(pos, capture, MoveType::PROMOTION, PieceType::QUEEN));
                    moves.push_back(Move(pos, capture, MoveType::PROMOTION, PieceType::ROOK));
                    moves.push_back(Move(pos, capture, MoveType::PROMOTION, PieceType::BISHOP));
                    moves.push_back(Move(pos, capture, MoveType::PROMOTION, PieceType::KNIGHT));
                } else {
                    moves.push_back(Move(pos, capture, MoveType::CAPTURE));
                }
            }

            // Взятие на проходе
            if (targetPiece.isEmpty() && capture == enPassantTarget) {
                moves.push_back(Move(pos, capture, MoveType::EN_PASSANT));
            }
        }
    }

    return moves;
}

std::vector<Move> Board::generateKnightMoves(const Position& pos) const {
    std::vector<Move> moves;
    Piece knight = getPiece(pos);
    if (knight.isEmpty() || knight.getType() != PieceType::KNIGHT) return moves;

    // Все возможные ходы коня
    const int knightMoves[8][2] = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };

    for (const auto& offset : knightMoves) {
        Position newPos(pos.x + offset[0], pos.y + offset[1]);
        if (newPos.isValid()) {
            Piece targetPiece = getPiece(newPos);
            if (targetPiece.isEmpty()) {
                moves.push_back(Move(pos, newPos));
            } else if (targetPiece.getColor() != knight.getColor()) {
                moves.push_back(Move(pos, newPos, MoveType::CAPTURE));
            }
        }
    }

    return moves;
}

std::vector<Move> Board::generateBishopMoves(const Position& pos) const {
    std::vector<Move> moves;
    Piece bishop = getPiece(pos);
    if (bishop.isEmpty() || bishop.getType() != PieceType::BISHOP) return moves;

    // Все 4 диагональных направления
    const int bishopDirections[4][2] = {
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    for (const auto& dir : bishopDirections) {
        for (int step = 1; step < 8; step++) {
            Position newPos(pos.x + dir[0] * step, pos.y + dir[1] * step);
            if (!newPos.isValid()) break;

            Piece targetPiece = getPiece(newPos);
            if (targetPiece.isEmpty()) {
                moves.push_back(Move(pos, newPos));
            } else {
                if (targetPiece.getColor() != bishop.getColor()) {
                    moves.push_back(Move(pos, newPos, MoveType::CAPTURE));
                }
                break; // Прекращаем движение в этом направлении
            }
        }
    }

    return moves;
}

std::vector<Move> Board::generateRookMoves(const Position& pos) const {
    std::vector<Move> moves;
    Piece rook = getPiece(pos);
    if (rook.isEmpty() || rook.getType() != PieceType::ROOK) return moves;

    // Все 4 ортогональных направления
    const int rookDirections[4][2] = {
        {0, -1}, {0, 1}, {-1, 0}, {1, 0}
    };

    for (const auto& dir : rookDirections) {
        for (int step = 1; step < 8; step++) {
            Position newPos(pos.x + dir[0] * step, pos.y + dir[1] * step);
            if (!newPos.isValid()) break;

            Piece targetPiece = getPiece(newPos);
            if (targetPiece.isEmpty()) {
                moves.push_back(Move(pos, newPos));
            } else {
                if (targetPiece.getColor() != rook.getColor()) {
                    moves.push_back(Move(pos, newPos, MoveType::CAPTURE));
                }
                break; // Прекращаем движение в этом направлении
            }
        }
    }

    return moves;
}

std::vector<Move> Board::generateQueenMoves(const Position& pos) const {
    std::vector<Move> moves;
    Piece queen = getPiece(pos);
    if (queen.isEmpty() || queen.getType() != PieceType::QUEEN) return moves;

    // Ферзь сочетает возможности ладьи и слона
    auto rookMoves = generateRookMoves(pos);
    auto bishopMoves = generateBishopMoves(pos);

    moves.insert(moves.end(), rookMoves.begin(), rookMoves.end());
    moves.insert(moves.end(), bishopMoves.begin(), bishopMoves.end());

    return moves;
}

std::vector<Move> Board::generateKingMoves(const Position& pos) const {
    std::vector<Move> moves;
    Piece king = getPiece(pos);
    if (king.isEmpty() || king.getType() != PieceType::KING) return moves;

    // Все 8 возможных направлений для короля
    const int kingDirections[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };

    for (const auto& dir : kingDirections) {
        Position newPos(pos.x + dir[0], pos.y + dir[1]);
        if (!newPos.isValid()) continue;

        Piece targetPiece = getPiece(newPos);
        if (targetPiece.isEmpty()) {
            moves.push_back(Move(pos, newPos));
        } else if (targetPiece.getColor() != king.getColor()) {
            moves.push_back(Move(pos, newPos, MoveType::CAPTURE));
        }
    }

    // Проверка возможности рокировки
    if (!king.getHasMoved()) {
        // Короткая рокировка (kingside)
        if ((king.getColor() == Color::WHITE && !whiteKingMoved && !whiteKingsideRookMoved) ||
            (king.getColor() == Color::BLACK && !blackKingMoved && !blackKingsideRookMoved)) {

            // Проверяем, свободны ли клетки между королем и ладьей
            bool canCastle = true;
            for (int x = pos.x + 1; x < 7; x++) {
                if (!getPiece(Position(x, pos.y)).isEmpty()) {
                    canCastle = false;
                    break;
                }
            }

            // Проверяем, не проходят ли клетки под атакой
            if (canCastle) {
                for (int x = pos.x; x <= pos.x + 2; x++) {
                    if (isUnderAttack(Position(x, pos.y), (king.getColor() == Color::WHITE) ? Color::BLACK : Color::WHITE)) {
                        canCastle = false;
                        break;
                    }
                }
            }

            if (canCastle) {
                moves.push_back(Move(pos, Position(pos.x + 2, pos.y), MoveType::CASTLE_KINGSIDE));
            }
        }

        // Длинная рокировка (queenside)
        if ((king.getColor() == Color::WHITE && !whiteKingMoved && !whiteQueensideRookMoved) ||
            (king.getColor() == Color::BLACK && !blackKingMoved && !blackQueensideRookMoved)) {

            // Проверяем, свободны ли клетки между королем и ладьей
            bool canCastle = true;
            for (int x = pos.x - 1; x > 0; x--) {
                if (!getPiece(Position(x, pos.y)).isEmpty()) {
                    canCastle = false;
                    break;
                }
            }

            // Проверяем, не проходят ли клетки под атакой
            if (canCastle) {
                for (int x = pos.x; x >= pos.x - 2; x--) {
                    if (isUnderAttack(Position(x, pos.y), (king.getColor() == Color::WHITE) ? Color::BLACK : Color::WHITE)) {
                        canCastle = false;
                        break;
                    }
                }
            }

            if (canCastle) {
                moves.push_back(Move(pos, Position(pos.x - 2, pos.y), MoveType::CASTLE_QUEENSIDE));
            }
        }
    }

    return moves;
}