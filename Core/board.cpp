#include "board.h"
#include "piece.h"
#include <cmath>

Board::Board() {
    grid.resize(BOARD_SIZE, std::vector<std::shared_ptr<Piece>>(BOARD_SIZE, nullptr));
    reset();
    // Инициализируем lastMove нулевыми значениями
    lastMove = { -1, -1, -1, -1 };
    kingMoved[0] = kingMoved[1] = false;
    rookMoved[0][0] = rookMoved[0][1] = false;
    rookMoved[1][0] = rookMoved[1][1] = false;
}

void Board::reset() {
    // Очистить доску
    for (int row = 0; row < BOARD_SIZE; ++row)
        for (int col = 0; col < BOARD_SIZE; ++col)
            grid[row][col] = nullptr;

    // Расстановка фигур — белые снизу, чёрные сверху
    // Чёрные
    grid[0][0] = std::make_shared<Rook>(Color::Black);
    grid[0][1] = std::make_shared<Knight>(Color::Black);
    grid[0][2] = std::make_shared<Bishop>(Color::Black);
    grid[0][3] = std::make_shared<Queen>(Color::Black);
    grid[0][4] = std::make_shared<King>(Color::Black);
    grid[0][5] = std::make_shared<Bishop>(Color::Black);
    grid[0][6] = std::make_shared<Knight>(Color::Black);
    grid[0][7] = std::make_shared<Rook>(Color::Black);
    for (int col = 0; col < BOARD_SIZE; ++col)
        grid[1][col] = std::make_shared<Pawn>(Color::Black);

    // Белые
    grid[7][0] = std::make_shared<Rook>(Color::White);
    grid[7][1] = std::make_shared<Knight>(Color::White);
    grid[7][2] = std::make_shared<Bishop>(Color::White);
    grid[7][3] = std::make_shared<Queen>(Color::White);
    grid[7][4] = std::make_shared<King>(Color::White);
    grid[7][5] = std::make_shared<Bishop>(Color::White);
    grid[7][6] = std::make_shared<Knight>(Color::White);
    grid[7][7] = std::make_shared<Rook>(Color::White);
    for (int col = 0; col < BOARD_SIZE; ++col)
        grid[6][col] = std::make_shared<Pawn>(Color::White);

    // Сброс lastMove при новой игре
    lastMove = { -1, -1, -1, -1 };
    kingMoved[0] = kingMoved[1] = false;
    rookMoved[0][0] = rookMoved[0][1] = false;
    rookMoved[1][0] = rookMoved[1][1] = false;
}

std::shared_ptr<Piece> Board::getPiece(int row, int col) const {
    if (!isInsideBoard(row, col)) return nullptr;
    return grid[row][col];
}

void Board::setPiece(int row, int col, std::shared_ptr<Piece> piece) {
    if (isInsideBoard(row, col))
        grid[row][col] = piece;
}

void Board::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    if (!isInsideBoard(fromRow, fromCol) || !isInsideBoard(toRow, toCol))
        return;
    // Сохраняем информацию о ходе
    auto piece = grid[fromRow][fromCol];
    if (!piece) return;

    int ci = (piece->getColor() == Color::White) ? 0 : 1;
    // Рокировка
    if ((piece->getSymbol() == 'K' || piece->getSymbol() == 'k') && std::abs(toCol - fromCol) == 2) {
        kingMoved[ci] = true;
        bool kingSide = (toCol > fromCol);
        grid[toRow][toCol] = piece;
        grid[fromRow][fromCol] = nullptr;
        int rookFromCol = kingSide ? BOARD_SIZE - 1 : 0;
        int rookToCol = kingSide ? toCol - 1 : toCol + 1;
        auto rook = grid[fromRow][rookFromCol];
        grid[fromRow][rookFromCol] = nullptr;
        grid[fromRow][rookToCol] = rook;
        rookMoved[ci][kingSide ? 1 : 0] = true;
        lastMove = { fromRow, fromCol, toRow, toCol };
        return;
    }

    if (piece->getSymbol() == 'K' || piece->getSymbol() == 'k') kingMoved[ci] = true;
    if (piece->getSymbol() == 'R' || piece->getSymbol() == 'r') {
        int side = (fromCol == 0) ? 0 : ((fromCol == BOARD_SIZE - 1) ? 1 : -1);
        if (side != -1) rookMoved[ci][side] = true;
    }

    lastMove = { fromRow, fromCol, toRow, toCol };
    // Перемещение
    grid[toRow][toCol] = piece;
    grid[fromRow][fromCol] = nullptr;
}

bool Board::isInsideBoard(int row, int col) const {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}
bool Board::isEmpty(int row, int col) const {
    return !grid[row][col];  // если nullptr — значит клетка пуста
}

const LastMove& Board::getLastMove() const {
    return lastMove;
}

bool Board::canCastleKingSide(Color color) const {
    int ci = (color == Color::White) ? 0 : 1;
    if (kingMoved[ci] || rookMoved[ci][1]) return false;
    int row = (color == Color::White) ? 7 : 0;
    if (!isEmpty(row, 5) || !isEmpty(row, 6)) return false;
    return true;
}

bool Board::canCastleQueenSide(Color color) const {
    int ci = (color == Color::White) ? 0 : 1;
    if (kingMoved[ci] || rookMoved[ci][0]) return false;
    int row = (color == Color::White) ? 7 : 0;
    if (!isEmpty(row, 1) || !isEmpty(row, 2) || !isEmpty(row, 3)) return false;
    return true;
}
