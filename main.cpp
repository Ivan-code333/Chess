// Обновлённый main.cpp с работающим отображением ввода и истории
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

    sf::Font font;
    if (!font.loadFromMemory(font_data, font_data_size)) {
        std::cerr << "Ошибка загрузки шрифта из памяти!\n";
        return -1;
    }

    sf::Texture texture;
    if (!texture.loadFromMemory(texture_data, texture_data_size)) {
        std::cerr << "Ошибка загрузки текстуры из памяти!\n";
        return -1;
    }
    ChessRenderer renderer(window, texture, sf::Vector2i(80, 80));
    
    Game game;
    game.reset();
    const Board& board = game.getBoard();

    std::vector<sf::String> moveHistory;
    sf::String currentInput;
    bool isInputActive = false;
    const int MAX_INPUT_LENGTH = 4;

    sf::RectangleShape historyBox(sf::Vector2f(panelWidth - 40, 200));
    historyBox.setPosition(boardPx + 20, 60);
    historyBox.setFillColor(sf::Color(240, 240, 240));
    historyBox.setOutlineColor(sf::Color(200, 200, 200));
    historyBox.setOutlineThickness(2);

    sf::RectangleShape inputBox(sf::Vector2f(panelWidth - 40, 30));
    inputBox.setPosition(boardPx + 20, 270);
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color(100, 100, 100));
    inputBox.setOutlineThickness(2);

    sf::Text inputText("", font, 20);
    inputText.setFillColor(sf::Color::Black);
    inputText.setPosition(boardPx + 25, 275);

    sf::Text inputHint("Enter move (e2e4)", font, 16);
    inputHint.setFillColor(sf::Color(150, 150, 150));
    inputHint.setPosition(boardPx + 25, 275);

    sf::RectangleShape flipBtn(sf::Vector2f(panelWidth - 40, 40));
    flipBtn.setPosition(boardPx + 20, 310);
    flipBtn.setFillColor(sf::Color(245, 241, 235));
    flipBtn.setOutlineColor(sf::Color(220, 210, 200));
    flipBtn.setOutlineThickness(2);

    sf::RectangleShape resignBtn = flipBtn;
    resignBtn.setPosition(boardPx + 20, 360);
    sf::RectangleShape drawBtn = flipBtn;
    drawBtn.setPosition(boardPx + 20, 410);

    sf::Text flipText("Flip board", font, 22);
    flipText.setFillColor(sf::Color(80, 80, 80));
    flipText.setPosition(boardPx + 40, 318);
    sf::Text resignText("Resign", font, 22);
    resignText.setFillColor(sf::Color(80, 80, 80));
    resignText.setPosition(boardPx + 40, 368);
    sf::Text drawText("Draw", font, 22);
    drawText.setFillColor(sf::Color(80, 80, 80));
    drawText.setPosition(boardPx + 40, 418);

    sf::Text movesTitle("Moves", font, 28);
    movesTitle.setFillColor(sf::Color::Black);
    movesTitle.setPosition(boardPx + 20, 20);

    sf::Text historyText("", font, 18);
    historyText.setFillColor(sf::Color::Black);
    historyText.setPosition(boardPx + 25, 65);

    // Добавляем переменные для выбора фигуры
    int selectedRow = -1;
    int selectedCol = -1;
    bool isPieceSelected = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
                
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    int mouseX = event.mouseButton.x;
                    int mouseY = event.mouseButton.y;
                    
                if (!game.isGameOver()) {
                    // Обработка клика по доске
                    if (mouseX < boardPx) {
                        int row, col;
                        if (renderer.getSquareFromMouse(mouseX, mouseY, row, col)) {
                            if (!isPieceSelected) {
                                // Выбор фигуры
                                auto piece = board.getPiece(row, col);
                                if (piece && piece->getColor() == game.getCurrentTurn()) {
                                    selectedRow = row;
                                    selectedCol = col;
                                    isPieceSelected = true;
                                }
                            } else {
                                // Попытка сделать ход
                                if (game.makeMove(selectedRow, selectedCol, row, col)) {
                                    // Ход успешен
                                    // Добавляем нотацию хода в историю
                                    std::string moveNotation = 
                                        std::string(1, 'a' + selectedCol) + 
                                        std::to_string(8 - selectedRow) +
                                        std::string(1, 'a' + col) + 
                                        std::to_string(8 - row);
                                    moveHistory.push_back(moveNotation);

                                    // Проверяем состояние игры для предыдущего хода
                                    Color previousTurn = (game.getCurrentTurn() == Color::White) ? Color::Black : Color::White;
                                    if (MoveHandler::isKingInCheck(board, previousTurn)) {
                                        if (!MoveHandler::hasLegalMoves(board, previousTurn)) {
                                            moveHistory.push_back("Checkmate! " + 
                                                std::string(previousTurn == Color::White ? "Black" : "White") + 
                                                " wins.");
                                            game.endGame();
                                        } else {
                                            moveHistory.push_back("Check!");
                                        }
                                    } else if (!MoveHandler::hasLegalMoves(board, previousTurn)) {
                                                moveHistory.push_back("Stalemate! It's a draw.");
                                                game.endGame();
                                            }
                                } else {
                                    // Если ход невозможен, проверяем, не выбрана ли другая фигура
                                    auto piece = board.getPiece(row, col);
                                    if (piece && piece->getColor() == game.getCurrentTurn()) {
                                        selectedRow = row;
                                        selectedCol = col;
                                    } else {
                                        isPieceSelected = false;
                                        selectedRow = -1;
                                        selectedCol = -1;
                                    }
                                }
                            }
                        }
                    }

                    // Обработка клика по полю ввода
                    if (mouseX >= boardPx + 20 && mouseX <= boardPx + panelWidth - 20 &&
                        mouseY >= 270 && mouseY <= 300) {
                        isInputActive = true;
                        inputBox.setOutlineColor(sf::Color(0, 120, 215));
                    } else {
                        isInputActive = false;
                        inputBox.setOutlineColor(sf::Color(100, 100, 100));
                    }

                    // Проверка нажатия кнопки Resign
                    if (mouseX >= boardPx + 20 && mouseX <= boardPx + panelWidth - 20 &&
                        mouseY >= 360 && mouseY <= 400) {
                        game.endGame();
                        moveHistory.push_back(game.getCurrentTurn() == Color::White ? "Black wins by resignation" : "White wins by resignation");
                    }

                    // Проверка нажатия кнопки Draw
                    if (mouseX >= boardPx + 20 && mouseX <= boardPx + panelWidth - 20 &&
                        mouseY >= 410 && mouseY <= 450) {
                        game.endGame();
                        moveHistory.push_back("Game drawn by agreement");
                    }
                }
            }

            if (event.type == sf::Event::TextEntered && isInputActive && !game.isGameOver()) {
                if (event.text.unicode == 8) { // backspace
                    if (!currentInput.isEmpty())
                        currentInput.erase(currentInput.getSize() - 1, 1);
                } else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
                    if (!currentInput.isEmpty()) {
                        try {
                            std::string input = currentInput.toAnsiString();
                            
                            // Проверяем, не ожидается ли выбор фигуры для превращения пешки
                            if (game.isWaitingForPromotion()) {
                                if (input.length() == 1 && (input[0] == 'Q' || input[0] == 'R' || 
                                    input[0] == 'B' || input[0] == 'N')) {
                                    game.promotePawn(input[0]);
                                    moveHistory.push_back("Pawn promoted to " + input);
                                } else {
                                    moveHistory.push_back("Invalid promotion piece! Choose Q, R, B, or N");
                                }
                                currentInput.clear();
                                continue;
                            }

                            if (input.length() != 4) {
                                moveHistory.push_back("Invalid move notation! Use format 'e2e4'");
                                currentInput.clear();
                                continue;
                            }

                            ParsedMove move = Notation::parseMoveNotation(game, input);
                            if (move.valid) {
                                if (game.makeMove(move.fromRow, move.fromCol, move.toRow, move.toCol)) {
                                    moveHistory.push_back(currentInput);

                                    // Проверяем состояние игры для предыдущего хода
                                    Color previousTurn = (game.getCurrentTurn() == Color::White) ? Color::Black : Color::White;
                                    if (MoveHandler::isKingInCheck(board, previousTurn)) {
                                        if (!MoveHandler::hasLegalMoves(board, previousTurn)) {
                                            moveHistory.push_back("Checkmate! " + 
                                                std::string(previousTurn == Color::White ? "Black" : "White") + 
                                                " wins.");
                                            game.endGame();
                                        } else {
                                            moveHistory.push_back("Check!");
                                        }
                                    } else if (!MoveHandler::hasLegalMoves(board, previousTurn)) {
                                                moveHistory.push_back("Stalemate! It's a draw.");
                                                game.endGame();
                                            }
                                } else {
                                    moveHistory.push_back("Invalid move!");
                                }
                            } else {
                                moveHistory.push_back("Invalid move notation!");
                            }
                        } catch (const std::exception& e) {
                            moveHistory.push_back("Error: Invalid input format");
                        }
                        currentInput.clear();
                        isPieceSelected = false;
                        selectedRow = -1;
                        selectedCol = -1;
                    }
                } else if (event.text.unicode < 128 && currentInput.getSize() < MAX_INPUT_LENGTH) {
                    char ch = static_cast<char>(event.text.unicode);
                    if ((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9'))
                        currentInput += ch;
                }
                inputText.setString(currentInput);
            }
        }

        // Обновление цвета кнопок при наведении
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (!game.isGameOver()) {
            if (mousePos.x >= boardPx + 20 && mousePos.x <= boardPx + panelWidth - 20) {
                if (mousePos.y >= 360 && mousePos.y <= 400) {
                    resignBtn.setFillColor(sf::Color(235, 231, 225));
                } else {
                    resignBtn.setFillColor(sf::Color(245, 241, 235));
                }
                if (mousePos.y >= 410 && mousePos.y <= 450) {
                    drawBtn.setFillColor(sf::Color(235, 231, 225));
                } else {
                    drawBtn.setFillColor(sf::Color(245, 241, 235));
                }
            } else {
                resignBtn.setFillColor(sf::Color(245, 241, 235));
                drawBtn.setFillColor(sf::Color(245, 241, 235));
            }
        } else {
            resignBtn.setFillColor(sf::Color(220, 220, 220));
            drawBtn.setFillColor(sf::Color(220, 220, 220));
            resignText.setFillColor(sf::Color(150, 150, 150));
            drawText.setFillColor(sf::Color(150, 150, 150));
        }

        sf::String sfHistoryString;
        for (const auto& move : moveHistory) {
            sfHistoryString += move + "\n";
        }
        historyText.setString(sfHistoryString);

        window.clear(sf::Color(245, 241, 235));

        renderer.drawBoard();
        
        // Подсветка выбранной фигуры
        if (isPieceSelected) {
            sf::RectangleShape highlight(sf::Vector2f(cellSize, cellSize));
            highlight.setPosition(sf::Vector2f(selectedCol * cellSize, selectedRow * cellSize));
            highlight.setFillColor(sf::Color(255, 255, 0, 100));
            window.draw(highlight);
        }
        
        renderer.drawPieces(board);
        renderer.drawPanel();

        window.draw(movesTitle);
        window.draw(historyBox);
        window.draw(historyText);

        window.draw(inputBox);
        if (currentInput.isEmpty() && !isInputActive && !game.isGameOver())
            window.draw(inputHint);
        else
            window.draw(inputText);

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
