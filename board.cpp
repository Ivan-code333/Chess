#include "board.h"
#include "piece.h"

Board::Board() {
    grid.resize(BOARD_SIZE, std::vector<std::shared_ptr<Piece>>(BOARD_SIZE, nullptr));
    reset();
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
    grid[toRow][toCol] = grid[fromRow][fromCol];
    grid[fromRow][fromCol] = nullptr;
}

bool Board::isInsideBoard(int row, int col) const {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}
