#include "move.h"
#include "piece.h"
#include <cmath>

bool MoveHandler::isMoveLegal(const Board& board, int fromRow, int fromCol,int toRow, int toCol,Color currentTurn)
{
    auto piece = board.getPiece(fromRow, fromCol);
    if (!piece || piece->getColor() != currentTurn)
        return false;

    if (!piece->isMoveLegal(fromRow, fromCol, toRow, toCol))
        return false;

    auto target = board.getPiece(toRow, toCol);
    if (target && target->getColor() == currentTurn)
        return false;

    // Логика пешки:
    char symbol = piece->getSymbol();
    if (symbol == 'P' || symbol == 'p') {
        int direction = (symbol == 'P') ? -1 : 1;
        int rowDiff = toRow - fromRow;
        int colDiff = toCol - fromCol;
        auto destPiece = board.getPiece(toRow, toCol);
        // диагональное взятие
        if (std::abs(colDiff) == 1 && rowDiff == direction) {
            if (!destPiece || destPiece->getColor() == currentTurn)
                return false;
        }
        // прямой ход
        if (colDiff == 0) {
            if (!board.isEmpty(toRow, toCol))
                return false;
            if (std::abs(rowDiff) == 2) {
                int startRow = (symbol == 'P') ? 6 : 1;
                if (fromRow != startRow || !board.isEmpty(fromRow + direction, fromCol))
                    return false;
            }
            else if (std::abs(rowDiff) != 1) {
                return false;
            }
        }
    }

    // Проверка, что путь свободен (для всех фигур, кроме коня и пешки)
    if (symbol != 'N' && symbol != 'n' && symbol != 'P' && symbol != 'p') {
        if (!isPathClear(board, fromRow, fromCol, toRow, toCol))
            return false;
    }

    // Симулируем ход и проверяем, не остаётся ли король под шахом
    Board temp = board;
    temp.movePiece(fromRow, fromCol, toRow, toCol);
    if (isKingInCheck(temp, currentTurn))
        return false;

    return true;
}

bool MoveHandler::tryMove(Board& board, int fromRow, int fromCol, int toRow, int toCol, Color currentTurn) {
    if (!isMoveLegal(board, fromRow, fromCol, toRow, toCol, currentTurn))
        return false;

    auto piece = board.getPiece(fromRow, fromCol);
    if (!piece) return false;

    char symbol = piece->getSymbol();

    // === Обработка логики пешки ===
    if (symbol == 'P' || symbol == 'p') {
        int direction = (currentTurn == Color::White) ? -1 : 1;
        int startRow = (currentTurn == Color::White) ? 6 : 1;
        int rowDiff = toRow - fromRow;
        int colDiff = toCol - fromCol;
        auto destPiece = board.getPiece(toRow, toCol);
// Пешка бьёт по диагонали — проверка есть ли вражеская фигура
        if (std::abs(colDiff) == 1 && rowDiff == direction) {
            if (!destPiece || destPiece->getColor() == currentTurn) {
                return false; // нельзя бить пустое поле или союзника
            }
        }


        // Пешка ходит вперёд — только если клетка пуста
        if (colDiff == 0) {
            if (!board.isEmpty(toRow, toCol))
                return false;

            if (rowDiff == direction) {
                // один шаг вперёд — всё ок
            }
            else if (rowDiff == 2 * direction) {
                // два шага — только с начальной позиции и обе клетки пусты
                if (fromRow != startRow || !board.isEmpty(fromRow + direction, fromCol))
                    return false;
            }
            else {
                // любой другой вертикальный ход — нелегален
                return false;
            }
        }
    }

    // === Общая проверка для остальных фигур (кроме коня) ===
    if (symbol != 'N' && symbol != 'n' && symbol != 'P' && symbol != 'p') {
        if (!isPathClear(board, fromRow, fromCol, toRow, toCol))
            return false;
    }

    board.movePiece(fromRow, fromCol, toRow, toCol);
    return true;
}




// Проверка, что путь от from до to чист (по горизонтали, вертикали или диагонали)
bool MoveHandler::isPathClear(const Board& board, int fromRow, int fromCol, int toRow, int toCol) {
    int dRow = (toRow - fromRow) == 0 ? 0 : (toRow - fromRow) / std::abs(toRow - fromRow);
    int dCol = (toCol - fromCol) == 0 ? 0 : (toCol - fromCol) / std::abs(toCol - fromCol);

    int r = fromRow + dRow;
    int c = fromCol + dCol;
    while (r != toRow || c != toCol) {
        if (board.getPiece(r, c))
            return false;
        r += dRow;
        c += dCol;
    }
    return true;
}

bool MoveHandler::isKingInCheck(const Board& board, Color kingColor) {
    int kingRow = -1, kingCol = -1;

    // Найти короля
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            auto piece = board.getPiece(row, col);
            if (piece && piece->getColor() == kingColor) {
                char symbol = piece->getSymbol();
                if ((kingColor == Color::White && symbol == 'K') ||
                    (kingColor == Color::Black && symbol == 'k')) {
                    kingRow = row;
                    kingCol = col;
                    break;
                    }
            }
        }
    }

    if (kingRow == -1 || kingCol == -1)
        return false; // Король не найден

    // Проверить, атакует ли кто-то короля
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            auto piece = board.getPiece(row, col);
            if (piece && piece->getColor() != kingColor) {
                if (piece->isMoveLegal(row, col, kingRow, kingCol) &&
                    isPathClear(board, row, col, kingRow, kingCol)) {
                    return true;
                    }
            }
        }
    }

    return false;
}

bool MoveHandler::hasLegalMoves(const Board& board, Color turn) {
    for (int fromRow = 0; fromRow < BOARD_SIZE; ++fromRow) {
        for (int fromCol = 0; fromCol < BOARD_SIZE; ++fromCol) {
            auto piece = board.getPiece(fromRow, fromCol);
            if (!piece || piece->getColor() != turn)
                continue;

            for (int toRow = 0; toRow < BOARD_SIZE; ++toRow) {
                for (int toCol = 0; toCol < BOARD_SIZE; ++toCol) {
                    if (fromRow == toRow && fromCol == toCol)
                        continue;

                    if (!MoveHandler::isMoveLegal(board, fromRow, fromCol, toRow, toCol, turn))
                        continue;

                    // Создаём временный ход на доске
                    auto tempBoard = board; // создаем копию доски
                    tempBoard.movePiece(fromRow, fromCol, toRow, toCol); // выполняем ход

                    // Проверяем, не оказался ли король под шахом
                    if (!isKingInCheck(tempBoard, turn)) {
                        return true; // Есть хотя бы один легальный ход
                    }
                }
            }
        }
    }
    return false; // Нет легальных ходов
}


