/*Это заголовочный файл класса Board, который описывает шахматную доску,
 то есть хранит фигуры, предоставляет доступ к ним, и реализует перемещения.*/


#ifndef BOARD_H

#define BOARD_H

#include <vector>
#include <memory>
#include "piece.h"

const int BOARD_SIZE = 8;

class Board {
public:
    Board();
    void reset();  // Установить начальное положение фигур

    std::shared_ptr<Piece> getPiece(int row, int col) const;
    void setPiece(int row, int col, std::shared_ptr<Piece> piece);
    void movePiece(int fromRow, int fromCol, int toRow, int toCol);
    bool isEmpty(int row, int col) const;

    bool isInsideBoard(int row, int col) const;

private:
    std::vector<std::vector<std::shared_ptr<Piece>>> grid;
};

#endif // BOARD_H
