#include <iostream>
#include <string>
#include "board.h"
#include "move.h"

void printBoard(const Board& board) {
    std::cout << "  a b c d e f g h\n";
    for (int row = 0; row < BOARD_SIZE; ++row) {
        std::cout << 8 - row << " ";
        for (int col = 0; col < BOARD_SIZE; ++col) {
            auto piece = board.getPiece(row, col);
            if (piece)
                std::cout << piece->getSymbol();
            else
                std::cout << '.';
            std::cout << ' ';
        }
        std::cout << 8 - row << "\n";
    }
    std::cout << "  a b c d e f g h\n";
}

bool parseMove(const std::string& input, int& fromRow, int& fromCol, int& toRow, int& toCol) {
    if (input.size() != 4)
        return false;

    fromCol = input[0] - 'a';
    fromRow = 8 - (input[1] - '0');
    toCol   = input[2] - 'a';
    toRow   = 8 - (input[3] - '0');

    return fromRow >= 0 && fromRow < 8 && fromCol >= 0 && fromCol < 8 &&
           toRow >= 0 && toRow < 8 && toCol >= 0 && toCol < 8;
}

int main() {
    Board board;
    Color currentTurn = Color::White;
    std::string input;

    while (true) {
        printBoard(board);
        std::cout << ((currentTurn == Color::White) ? "White" : "Black") << " to move (e.g. e2e4), or 'q' to quit: ";
        std::cin >> input;

        if (input == "q" || input == "quit")
            break;

        int fromRow, fromCol, toRow, toCol;
        if (!parseMove(input, fromRow, fromCol, toRow, toCol)) {
            std::cout << "Invalid input format. Use format like e2e4.\n";
            continue;
        }

        if (MoveHandler::tryMove(board, fromRow, fromCol, toRow, toCol, currentTurn)) {
            currentTurn = (currentTurn == Color::White) ? Color::Black : Color::White;
        } else {
            std::cout << "Illegal move. Try again.\n";
        }
    }

    std::cout << "Game over.\n";
    return 0;
}
