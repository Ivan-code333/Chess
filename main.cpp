#include <iostream>
#include "board.h"
#include "piece.h"

int main() {
    // Инициализация доски
    Board board;
    board.setupInitialPosition();

    // Отладочный вывод начальной позиции
    std::cout << "Initial board:" << std::endl;
    board.print();

    // Пример 1: Ход белой пешки e2 -> e4
    std::cout << "\nMoving white pawn e2 -> e4..." << std::endl;
    if (board.isMoveLegal({4, 1}, {4, 3}, Color::WHITE)) {
        board.movePiece({4, 1}, {4, 3});
        board.print();
    } else {
        std::cerr << "Invalid move!" << std::endl;
    }

    // Пример 2: Ход черного коня g8 -> f6
    std::cout << "\nMoving black knight g8 -> f6..." << std::endl;
    if (board.isMoveLegal({6, 7}, {5, 5}, Color::BLACK)) {
        board.movePiece({6, 7}, {5, 5});
        board.print();
    } else {
        std::cerr << "Invalid move!" << std::endl;
    }

    // Пример 3: Неверный ход (ладья не может перепрыгивать)
    std::cout << "\nTrying illegal move (a1 -> a3)..." << std::endl;
    if (board.isMoveLegal({0, 0}, {0, 2}, Color::WHITE)) {
        board.movePiece({0, 0}, {0, 2});
    } else {
        std::cerr << "Move rejected (as expected)." << std::endl;
    }

    return 0;
}