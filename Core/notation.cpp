#include "notation.h"
#include <cctype>

char pieceSymbols[] = { 'K', 'Q', 'R', 'B', 'N' }; // без пешек

bool isPieceChar(char c) {
    for (char s : pieceSymbols) {
        if (s == c) return true;
    }
    return false;
}

ParsedMove Notation::parseMoveNotation(const Game& game, const std::string& input) 
    {
        ParsedMove result{ -1, -1, -1, -1, false };
        // Поддерживаем только строку ровно из 4 символов: fromFile, fromRank, toFile, toRank
        if (input.length() != 4)
            return result;

        char fFile = input[0], fRank = input[1];
        char tFile = input[2], tRank = input[3];

        // Проверяем, что всё в диапазоне a–h и 1–8
        if (fFile < 'a' || fFile > 'h' ||
            tFile < 'a' || tFile > 'h' ||
            fRank < '1' || fRank > '8' ||
            tRank < '1' || tRank > '8')
        {
            return result;
        }

        int fromCol = fFile - 'a';
        int fromRow = 8 - (fRank - '0');
        int toCol = tFile - 'a';
        int toRow = 8 - (tRank - '0');

        const Board& board = game.getBoard();
        Color turn = game.getCurrentTurn();

        // Проверяем, что на from стоит фигура своего цвета
        auto piece = board.getPiece(fromRow, fromCol);
        if (!piece || piece->getColor() != turn)
            return result;

        // Проверяем легальность хода
        if (piece->isMoveLegal(fromRow, fromCol, toRow, toCol) &&
            MoveHandler::isMoveLegal(board, fromRow, fromCol, toRow, toCol, turn))
        {
            result.fromRow = fromRow;
            result.fromCol = fromCol;
            result.toRow = toRow;
            result.toCol = toCol;
            result.valid = true;
        }

        return result;
}

