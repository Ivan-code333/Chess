#include "notation.h"
#include <cctype>

char pieceSymbols[] = { 'K', 'Q', 'R', 'B', 'N' }; // без пешек

bool isPieceChar(char c) {
    for (char s : pieceSymbols) {
        if (s == c) return true;
    }
    return false;
}

ParsedMove Notation::parseMoveNotation(const Game& game, const std::string& input) {
    ParsedMove result{ -1, -1, -1, -1, false };

    if (input.length() < 2 || input.length() > 3)
        return result;

    char fileChar = input[input.length() - 2];
    char rankChar = input[input.length() - 1];
    if (fileChar < 'a' || fileChar > 'h' || rankChar < '1' || rankChar > '8')
        return result;

    int toCol = fileChar - 'a';
    int toRow = 8 - (rankChar - '0');

    const Board& board = game.getBoard();
    Color turn = game.getCurrentTurn();

    char pieceChar = (input.length() == 3) ? input[0] : 'P'; // Пешка по умолчанию

    // Ищем фигуру игрока, которая может пойти туда
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            auto piece = board.getPiece(row, col);
            if (!piece || piece->getColor() != turn) continue;

            char symbol = piece->getSymbol();
            if (symbol >= 'a' && symbol <= 'z') symbol -= 32; // к верхнему регистру

            if ((pieceChar == 'P' && symbol == 'P') || (symbol == pieceChar)) {
                if (piece->isMoveLegal(row, col, toRow, toCol) &&
                    MoveHandler::isMoveLegal(board, row, col, toRow, toCol, turn))
                    // пробный ход
                {
                    result.fromRow = row;
                    result.fromCol = col;
                    result.toRow = toRow;
                    result.toCol = toCol;
                    result.valid = true;
                    return result;
                }
            }
        }
    }

    return result; // ничего не нашли
}
