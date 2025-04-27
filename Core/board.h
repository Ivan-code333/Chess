/*Это заголовочный файл класса Board, который описывает шахматную доску,
 то есть хранит фигуры, предоставляет доступ к ним, и реализует перемещения.*/


#ifndef BOARD_H

#define BOARD_H

#include <vector>
#include <memory>
#include "piece.h"

const int BOARD_SIZE = 8;

// Хранит информацию о последнем ходе
struct LastMove {
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;
};

class Board {
public:
    Board();
    void reset();  // Установить начальное положение фигур

    std::shared_ptr<Piece> getPiece(int row, int col) const;
    void setPiece(int row, int col, std::shared_ptr<Piece> piece);
    void movePiece(int fromRow, int fromCol, int toRow, int toCol);
    bool isEmpty(int row, int col) const;

    bool isInsideBoard(int row, int col) const;

    // Получить информацию о последнем ходе
    const LastMove& getLastMove() const;

private:
    std::vector<std::vector<std::shared_ptr<Piece>>> grid;

    // Информация о последнем ходе для реализации взятия на проходе
    LastMove lastMove;
};

#endif // BOARD_H
