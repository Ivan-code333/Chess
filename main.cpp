#include <iostream>
#include <string>
#include "Core/board.h"
#include "Core/move.h"
#include "Core/game.h"
#include "Core/notation.h"
#include "Core/renderer.h"
#include "Font.h"
#include "Texture.h"
#include <SFML/Graphics.hpp>
#include <vector>

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

    // Инициализация шрифта
    sf::Font font;
    if (!font.loadFromMemory(font_data, font_data_size)) {
        std::cerr << "Ошибка загрузки шрифта из памяти!\n";
        return -1;
    }

    // Создаем рендерер
    sf::Texture texture;
    if (!texture.loadFromMemory(texture_data, texture_data_size)) {
        std::cerr << "Ошибка загрузки текстуры из памяти!\n";
        return -1;
    }
    ChessRenderer renderer(window, texture, sf::Vector2i(80, 80));
    
    // Создаем игру
    Game game;
    game.reset();
    const Board& board = game.getBoard();

    // История ходов
    std::vector<std::string> moveHistory;
    std::string currentInput;
    bool isInputActive = false;
    bool windowHasFocus = true;
    const int MAX_INPUT_LENGTH = 4; // Максимальная длина хода (например, e2e4)

    // Область истории ходов
    sf::RectangleShape historyBox(sf::Vector2f(panelWidth-40, 200));
    historyBox.setPosition(boardPx+20, 60);
    historyBox.setFillColor(sf::Color(240,240,240));
    historyBox.setOutlineColor(sf::Color(200,200,200));
    historyBox.setOutlineThickness(2);

    // Текстовое поле для ввода
    sf::RectangleShape inputBox(sf::Vector2f(panelWidth-40, 30));
    inputBox.setPosition(boardPx+20, 270);
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color(100,100,100));
    inputBox.setOutlineThickness(2);

    // Текст ввода
    sf::Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(20);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(boardPx+25, 275);
    inputText.setString("");

    // Текст подсказки
    sf::Text inputHint;
    inputHint.setFont(font);
    inputHint.setCharacterSize(16);
    inputHint.setFillColor(sf::Color(150,150,150));
    inputHint.setPosition(boardPx+25, 275);
    inputHint.setString("Enter move (e2e4)");

    // Кнопки
    sf::RectangleShape flipBtn(sf::Vector2f(panelWidth-40, 40));
    flipBtn.setPosition(boardPx+20, 310);
    flipBtn.setFillColor(sf::Color(245, 241, 235));
    flipBtn.setOutlineColor(sf::Color(220, 210, 200));
    flipBtn.setOutlineThickness(2);

    sf::RectangleShape resignBtn = flipBtn;
    resignBtn.setPosition(boardPx+20, 360);
    sf::RectangleShape drawBtn = flipBtn;
    drawBtn.setPosition(boardPx+20, 410);

    sf::Text flipText("Flip board", font, 22);
    flipText.setFillColor(sf::Color(80,80,80));
    flipText.setPosition(boardPx+40, 318);
    sf::Text resignText("Resign", font, 22);
    resignText.setFillColor(sf::Color(80,80,80));
    resignText.setPosition(boardPx+40, 368);
    sf::Text drawText("Draw", font, 22);
    drawText.setFillColor(sf::Color(80,80,80));
    drawText.setPosition(boardPx+40, 418);

    sf::Text movesTitle("Moves", font, 28);
    movesTitle.setFillColor(sf::Color::Black);
    movesTitle.setPosition(boardPx+20, 20);

    // Текст истории ходов
    sf::Text historyText;
    historyText.setFont(font);
    historyText.setCharacterSize(18);
    historyText.setFillColor(sf::Color::Black);
    historyText.setPosition(boardPx+25, 65);

    sf::String sfCurrentInput;
    sf::String sfHistoryString;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::LostFocus) {
                windowHasFocus = false;
                std::cout << "Window lost focus" << std::endl;
            }
            if (event.type == sf::Event::GainedFocus) {
                windowHasFocus = true;
                std::cout << "Window gained focus" << std::endl;
            }
                
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int mouseX = event.mouseButton.x;
                    int mouseY = event.mouseButton.y;
                    
                    // Проверяем клик по полю ввода
                    if (mouseX >= boardPx+20 && mouseX <= boardPx+panelWidth-20 &&
                        mouseY >= 270 && mouseY <= 300) {
                        isInputActive = true;
                        window.requestFocus();
                        inputBox.setOutlineColor(sf::Color(0,120,215));
                        inputBox.setOutlineThickness(2);
                        std::cout << "Input field activated" << std::endl;
                    } else {
                        isInputActive = false;
                        inputBox.setOutlineColor(sf::Color(100,100,100));
                        inputBox.setOutlineThickness(2);
                        std::cout << "Input field deactivated" << std::endl;
                    }
                    
                    // Проверяем клик по кнопкам
                    if (mouseX >= boardPx+20 && mouseX <= boardPx+panelWidth-20) {
                        if (mouseY >= 310 && mouseY <= 350) {
                            // TODO: Реализовать переворот доски
                        }
                        else if (mouseY >= 360 && mouseY <= 400) {
                            // TODO: Реализовать сдачу
                        }
                        else if (mouseY >= 410 && mouseY <= 450) {
                            // TODO: Реализовать предложение ничьей
                        }
                    }
                }
            }

            // Обработка ввода текста
            if (event.type == sf::Event::TextEntered) {
                std::cout << "Text entered event: " << event.text.unicode << std::endl;
                if (isInputActive) {
                    if (event.text.unicode == '\b') { // Backspace
                        if (!currentInput.empty()) {
                            currentInput.pop_back();
                            std::cout << "Backspace pressed, new input: " << currentInput << std::endl;
                        }
                    }
                    else if (event.text.unicode == '\r' || event.text.unicode == '\n') { // Enter
                        if (!currentInput.empty()) {
                            moveHistory.push_back(currentInput);
                            std::cout << "Enter pressed, added move: " << currentInput << std::endl;
                            currentInput.clear();
                        }
                    }
                    else if (event.text.unicode < 128 && currentInput.length() < MAX_INPUT_LENGTH) {
                        char ch = static_cast<char>(event.text.unicode);
                        if ((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')) {
                            currentInput += ch;
                            std::cout << "Character added, new input: " << currentInput << std::endl;
                        }
                    }
                    inputText.setString(currentInput);
                }
            }

            // Обработка клавиш
            if (event.type == sf::Event::KeyPressed) {
                std::cout << "Key pressed: " << event.key.code << std::endl;
                if (isInputActive) {
                    if (event.key.code == sf::Keyboard::BackSpace) {
                        if (!currentInput.empty()) {
                            currentInput.pop_back();
                            inputText.setString(currentInput);
                            std::cout << "Backspace key pressed, new input: " << currentInput << std::endl;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Return) {
                        if (!currentInput.empty()) {
                            moveHistory.push_back(currentInput);
                            std::cout << "Return key pressed, added move: " << currentInput << std::endl;
                            currentInput.clear();
                            inputText.setString(currentInput);
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Escape) {
                        isInputActive = false;
                        inputBox.setOutlineColor(sf::Color(100,100,100));
                        inputBox.setOutlineThickness(2);
                        std::cout << "Escape pressed, input field deactivated" << std::endl;
                    }
                }
            }
        }

        window.clear(sf::Color(245, 241, 235));

        // Рисуем доску и фигуры
        renderer.drawBoard();
        renderer.drawPieces(board);
        
        // Рисуем панель
        renderer.drawPanel();
        window.draw(movesTitle);
        
        // Рисуем историю ходов
        window.draw(historyBox);
        
        // Отображаем историю ходов
        sfCurrentInput = currentInput;
        inputText.setString(sfCurrentInput);

        sfHistoryString.clear();
        for (const auto& move : moveHistory) {
            sfHistoryString += move;
            sfHistoryString += '\n';
        }
        historyText.setString(sfHistoryString);
        window.draw(historyText);

        // Рисуем поле ввода
        window.draw(inputBox);
        if (currentInput.empty() && !isInputActive) {
            window.draw(inputHint);
        } else {
            window.draw(inputText);
        }

        // Рисуем кнопки
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
