#include <iostream>
#include <string>
#include "Core/board.h"
#include "Core/move.h"
#include "Core/game.h"
#include "Core/notation.h"
#include "Font.h"
#include <SFML/Graphics.hpp>

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
    const int cellSize = 80;
    const int boardSize = 8;
    const int boardPx = cellSize * boardSize;
    const int panelWidth = 320;
    const int windowWidth = boardPx + panelWidth;
    const int windowHeight = boardPx;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Chess");
    window.setFramerateLimit(60);

    // Цвета клеток
    sf::Color lightColor(240, 217, 181);
    sf::Color darkColor(181, 136, 99);
    sf::Color panelColor(245, 241, 235);
    sf::Color buttonColor(245, 241, 235);
    sf::Color buttonOutline(220, 210, 200);

    // Шрифт для текста
    sf::Font font;
    if (!font.loadFromMemory(font_data, font_data_size)) {
        std::cerr << "Ошибка загрузки шрифта из памяти!\n";
        return -1; // Выход при ошибке загрузки шрифта
    }
    
    // Кнопки
    sf::RectangleShape flipBtn(sf::Vector2f(panelWidth-40, 40));
    flipBtn.setPosition(boardPx+20, 300);
    flipBtn.setFillColor(buttonColor);
    flipBtn.setOutlineColor(buttonOutline);
    flipBtn.setOutlineThickness(2);

    sf::RectangleShape resignBtn = flipBtn;
    resignBtn.setPosition(boardPx+20, 350);
    sf::RectangleShape drawBtn = flipBtn;
    drawBtn.setPosition(boardPx+20, 400);

    sf::Text flipText("Flip Board", font, 22);
    flipText.setFillColor(sf::Color(80,80,80));
    flipText.setPosition(boardPx+40, 308);
    sf::Text resignText("Resign", font, 22);
    resignText.setFillColor(sf::Color(80,80,80));
    resignText.setPosition(boardPx+40, 358);
    sf::Text drawText("Draw", font, 22);
    drawText.setFillColor(sf::Color(80,80,80));
    drawText.setPosition(boardPx+40, 408);

    sf::RectangleShape panelBg(sf::Vector2f(panelWidth, windowHeight));
    panelBg.setPosition(boardPx, 0);
    panelBg.setFillColor(panelColor);
    sf::Text movesTitle("Moves", font, 28);
    movesTitle.setFillColor(sf::Color::Black);
    movesTitle.setPosition(boardPx+20, 20);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(panelColor);

        // Рисуем доску
        for (int row = 0; row < boardSize; ++row) {
            for (int col = 0; col < boardSize; ++col) {
                sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
                cell.setPosition(col*cellSize, row*cellSize);
                if ((row+col)%2==0)
                    cell.setFillColor(lightColor);
                else
                    cell.setFillColor(darkColor);
                window.draw(cell);
            }
        }
        // Рисуем панель
        window.draw(panelBg);
        window.draw(movesTitle);
        window.draw(flipBtn);
        window.draw(resignBtn);
        window.draw(drawBtn);
        window.draw(flipText);
        window.draw(resignText);
        window.draw(drawText);

        window.display();
    }
    return 0;
}
