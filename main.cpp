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
#include "Favicon.h"
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
    const int coordSize = 30; // Размер области для координат
    const int windowWidth = boardPx + panelWidth + coordSize; // Добавляем место для вертикальных координат
    const int windowHeight = boardPx + coordSize; // Добавляем место для горизонтальных координат

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Chess");
    window.setFramerateLimit(60);

    // Загружаем фавикон
    sf::Image icon;
    if (!icon.loadFromMemory(favicon_data, favicon_data_size)) {
        std::cerr << "Ошибка загрузки фавикона!\n";
    } else {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

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
    bool isInputActive = false;
    const int MAX_INPUT_LENGTH = 4;

    sf::RectangleShape historyBox(sf::Vector2f(panelWidth - 40, 400));
    historyBox.setPosition(boardPx + coordSize + 20, 60); // Сдвигаем с учетом координат
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
    flipBtn.setPosition(boardPx + coordSize + 20, 470); // Сдвигаем с учетом координат
    flipBtn.setFillColor(sf::Color(245, 241, 235));
    flipBtn.setOutlineColor(sf::Color(220, 210, 200));
    flipBtn.setOutlineThickness(2);

    sf::RectangleShape resignBtn = flipBtn;
    resignBtn.setPosition(boardPx + coordSize + 20, 520); // Сдвигаем с учетом координат
    sf::RectangleShape drawBtn = flipBtn;
    drawBtn.setPosition(boardPx + coordSize + 20, 570); // Сдвигаем с учетом координат

    sf::Text flipText("Flip board", font, 22);
    flipText.setFillColor(sf::Color(80, 80, 80));
    flipText.setPosition(boardPx + coordSize + 40, 478); // Сдвигаем с учетом координат
    sf::Text resignText("Resign", font, 22);
    resignText.setFillColor(sf::Color(80, 80, 80));
    resignText.setPosition(boardPx + coordSize + 40, 528); // Сдвигаем с учетом координат
    sf::Text drawText("Draw", font, 22);
    drawText.setFillColor(sf::Color(80, 80, 80));
    drawText.setPosition(boardPx + coordSize + 40, 578); // Сдвигаем с учетом координат

    sf::Text movesTitle("Moves", font, 28);
    movesTitle.setFillColor(sf::Color::Black);
    movesTitle.setPosition(boardPx + coordSize + 20, 20);

    // Добавляем статические координаты
    sf::Text coord1("1", font, 28);
    sf::Text coord2("2", font, 28);
    sf::Text coord3("3", font, 28);
    sf::Text coord4("4", font, 28);
    sf::Text coord5("5", font, 28);
    sf::Text coord6("6", font, 28);
    sf::Text coord7("7", font, 28);
    sf::Text coord8("8", font, 28);

    sf::Text coordA("a", font, 28);
    sf::Text coordB("b", font, 28);
    sf::Text coordC("c", font, 28);
    sf::Text coordD("d", font, 28);
    sf::Text coordE("e", font, 28);
    sf::Text coordF("f", font, 28);
    sf::Text coordG("g", font, 28);
    sf::Text coordH("h", font, 28);

    // Устанавливаем позиции для горизонтальных координат (сверху вниз: 8-1)
    coord8.setPosition(boardPx+5, cellSize*0 + cellSize/2 - 15);
    coord7.setPosition(boardPx+5, cellSize*1 + cellSize/2 - 15);
    coord6.setPosition(boardPx+5, cellSize*2 + cellSize/2 - 15);
    coord5.setPosition(boardPx+5, cellSize*3 + cellSize/2 - 15);
    coord4.setPosition(boardPx+5, cellSize*4 + cellSize/2 - 15);
    coord3.setPosition(boardPx+5, cellSize*5 + cellSize/2 - 15);
    coord2.setPosition(boardPx+5, cellSize*6 + cellSize/2 - 15);
    coord1.setPosition(boardPx+5, cellSize*7 + cellSize/2 - 15);

    // Устанавливаем позиции для вертикальных координат (слева направо: a-h)
    coordA.setPosition(cellSize*0 + cellSize/2 - 10, boardPx + coordSize/2 - 20);
    coordB.setPosition(cellSize*1 + cellSize/2 - 10, boardPx + coordSize/2 - 20);
    coordC.setPosition(cellSize*2 + cellSize/2 - 10, boardPx + coordSize/2 - 20);
    coordD.setPosition(cellSize*3 + cellSize/2 - 10, boardPx + coordSize/2 - 20);
    coordE.setPosition(cellSize*4 + cellSize/2 - 10, boardPx + coordSize/2 - 20);
    coordF.setPosition(cellSize*5 + cellSize/2 - 10, boardPx + coordSize/2 - 20);
    coordG.setPosition(cellSize*6 + cellSize/2 - 10, boardPx + coordSize/2 - 20);
    coordH.setPosition(cellSize*7 + cellSize/2 - 10, boardPx + coordSize/2 - 20);

    // Устанавливаем цвет для всех координат
    coord1.setFillColor(sf::Color(0, 0, 0));
    coord2.setFillColor(sf::Color(0, 0, 0));
    coord3.setFillColor(sf::Color(0, 0, 0));
    coord4.setFillColor(sf::Color(0, 0, 0));
    coord5.setFillColor(sf::Color(0, 0, 0));
    coord6.setFillColor(sf::Color(0, 0, 0));
    coord7.setFillColor(sf::Color(0, 0, 0));
    coord8.setFillColor(sf::Color(0, 0, 0));
    coordA.setFillColor(sf::Color(0, 0, 0));
    coordB.setFillColor(sf::Color(0, 0, 0));
    coordC.setFillColor(sf::Color(0, 0, 0));
    coordD.setFillColor(sf::Color(0, 0, 0));
    coordE.setFillColor(sf::Color(0, 0, 0));
    coordF.setFillColor(sf::Color(0, 0, 0));
    coordG.setFillColor(sf::Color(0, 0, 0));
    coordH.setFillColor(sf::Color(0, 0, 0));

    

    sf::Text historyText;
    historyText.setFont(font);
    historyText.setCharacterSize(18);
    historyText.setFillColor(sf::Color::Black);
    historyText.setPosition(boardPx + coordSize + 25, 65);
    historyText.setLineSpacing(1.5f);

    // Добавляем переменные для выбора фигуры
    int selectedRow = -1;
    int selectedCol = -1;
    bool isPieceSelected = false;

    // Добавляем переменные для прокрутки
    float scrollOffset = 0;
    const float SCROLL_SPEED = 20.0f;
    const float MAX_SCROLL = 0; // Максимальное смещение вверх
    float minScroll = 0; // Минимальное смещение вниз будет рассчитано позже

    // Создаем view для области истории
    sf::View historyView(sf::FloatRect(
        //boardPx + coordSize + 20, 60,
        0, 0,   // Позиция
        panelWidth - 40, 400           // Размер (такой же как у historyBox)
    ));
    historyView.setViewport(sf::FloatRect(
        (boardPx + coordSize + 20.0f) / windowWidth, 60.0f / windowHeight,
        (panelWidth - 40.0f) / windowWidth, 400.0f / windowHeight
    ));

    // Добавляем переменные для окна выбора фигуры
    bool isPromotionWindowOpen = false;
    int promotionRow = -1;
    int promotionCol = -1;
    sf::RectangleShape promotionWindow(sf::Vector2f(200, 100));
    promotionWindow.setFillColor(sf::Color(245, 241, 235));
    promotionWindow.setOutlineColor(sf::Color(200, 200, 200));
    promotionWindow.setOutlineThickness(2);

    // Создаем кнопки для выбора фигуры
    const int PROMOTION_BUTTON_SIZE = 40;
    std::vector<sf::RectangleShape> promotionButtons;
    std::vector<sf::Text> promotionTexts;
    std::vector<char> promotionPieces = {'Q', 'R', 'N', 'B'}; // Ферзь, Ладья, Конь, Слон

    for (int i = 0; i < 4; ++i) {
        sf::RectangleShape button(sf::Vector2f(PROMOTION_BUTTON_SIZE, PROMOTION_BUTTON_SIZE));
        button.setFillColor(sf::Color(255, 255, 255));
        button.setOutlineColor(sf::Color(200, 200, 200));
        button.setOutlineThickness(1);
        promotionButtons.push_back(button);

        sf::Text text;
        text.setFont(font);
        text.setString(sf::String(promotionPieces[i]));
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Black);
        promotionTexts.push_back(text);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
                
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    int mouseX = event.mouseButton.x;
                    int mouseY = event.mouseButton.y;
                    
                if (isPromotionWindowOpen) {
                    // Проверяем клик по кнопкам выбора фигуры
                    for (int i = 0; i < 4; ++i) {
                        if (promotionButtons[i].getGlobalBounds().contains(mouseX, mouseY)) {
                            // Превращаем пешку в выбранную фигуру
                            game.promotePawn(promotionPieces[i]);
                            isPromotionWindowOpen = false;
                            
                            // Добавляем нотацию хода в историю
                            sf::String moveNotation;
                            moveNotation += sf::String(static_cast<char>('a' + selectedCol));
                            moveNotation += sf::String(static_cast<char>('8' - selectedRow));
                            moveNotation += sf::String(static_cast<char>('a' + promotionCol));
                            moveNotation += sf::String(static_cast<char>('8' - promotionRow));
                            moveNotation += sf::String(promotionPieces[i]);
                            moveHistory.push_back(moveNotation);

                            // Проверяем состояние игры
                            Color currentTurn = game.getCurrentTurn();
                            if (MoveHandler::isKingInCheck(board, currentTurn)) {
                                if (!MoveHandler::hasLegalMoves(board, currentTurn)) {
                                    sf::String checkmateMsg = sf::String("Checkmate! ");
                                    checkmateMsg += sf::String(currentTurn == Color::White ? "Black" : "White");
                                    checkmateMsg += sf::String(" wins.");
                                    moveHistory.push_back(checkmateMsg);
                                    game.endGame();
                                } else {
                                    moveHistory.push_back(sf::String("Check!"));
                                }
                            } else if (!MoveHandler::hasLegalMoves(board, currentTurn)) {
                                moveHistory.push_back(sf::String("Stalemate! It's a draw."));
                                game.endGame();
                            }

                            isPieceSelected = false;
                            selectedRow = -1;
                            selectedCol = -1;
                            break;
                        }
                    }
                } else if (!game.isGameOver()) {
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
                                    // Проверяем, нужно ли превращение пешки
                                    auto piece = board.getPiece(row, col);
                                    if (piece && (piece->getSymbol() == 'P' || piece->getSymbol() == 'p') && 
                                        ((piece->getColor() == Color::White && row == 0) || 
                                         (piece->getColor() == Color::Black && row == 7))) {
                                        // Открываем окно выбора фигуры
                                        isPromotionWindowOpen = true;
                                        promotionRow = row;
                                        promotionCol = col;
                                        
                                        // Позиционируем окно выбора
                                        float windowX = col * cellSize + (cellSize - 200) / 2;
                                        float windowY = row * cellSize + (cellSize - 100) / 2;
                                        promotionWindow.setPosition(windowX, windowY);
                                        
                                        // Позиционируем кнопки
                                        for (int i = 0; i < 4; ++i) {
                                            promotionButtons[i].setPosition(
                                                windowX + 10 + i * (PROMOTION_BUTTON_SIZE + 10),
                                                windowY + 30
                                            );
                                            promotionTexts[i].setPosition(
                                                windowX + 20 + i * (PROMOTION_BUTTON_SIZE + 10),
                                                windowY + 35
                                            );
                                        }
                                    } else {
                                        // Ход успешен
                                        // Добавляем нотацию хода в историю
                                        sf::String moveNotation;
                                        moveNotation += sf::String(static_cast<char>('a' + selectedCol));
                                        moveNotation += sf::String(static_cast<char>('8' - selectedRow));
                                        moveNotation += sf::String(static_cast<char>('a' + col));
                                        moveNotation += sf::String(static_cast<char>('8' - row));
                                        moveHistory.push_back(moveNotation);

                                        // Проверяем состояние игры для текущего хода
                                        Color currentTurn = game.getCurrentTurn();
                                        if (MoveHandler::isKingInCheck(board, currentTurn)) {
                                            if (!MoveHandler::hasLegalMoves(board, currentTurn)) {
                                                sf::String checkmateMsg = sf::String("Checkmate! ");
                                                checkmateMsg += sf::String(currentTurn == Color::White ? "Black" : "White");
                                                checkmateMsg += sf::String(" wins.");
                                                moveHistory.push_back(checkmateMsg);
                                                game.endGame();
                                                isPieceSelected = false;
                                                selectedRow = -1;
                                                selectedCol = -1;
                                            } else {
                                                moveHistory.push_back(sf::String("Check!"));
                                            }
                                        } else if (!MoveHandler::hasLegalMoves(board, currentTurn)) {
                                            moveHistory.push_back(sf::String("Stalemate! It's a draw."));
                                            game.endGame();
                                            isPieceSelected = false;
                                            selectedRow = -1;
                                            selectedCol = -1;
                                        }
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

                    // Проверка нажатия кнопки Flip board
                    if (mouseX >= boardPx + coordSize + 20 && mouseX <= boardPx + coordSize + panelWidth - 20 &&
                        mouseY >= 470 && mouseY <= 510) {
                        renderer.flipBoard();
                        
                        // Обновляем позиции цифровых координат
                        if (renderer.isFlipped()) {
                            // Для черных: снизу вверх (1-8)
                            coord1.setPosition(boardPx+5, cellSize*0 + cellSize/2 - 15);
                            coord2.setPosition(boardPx+5, cellSize*1 + cellSize/2 - 15);
                            coord3.setPosition(boardPx+5, cellSize*2 + cellSize/2 - 15);
                            coord4.setPosition(boardPx+5, cellSize*3 + cellSize/2 - 15);
                            coord5.setPosition(boardPx+5, cellSize*4 + cellSize/2 - 15);
                            coord6.setPosition(boardPx+5, cellSize*5 + cellSize/2 - 15);
                            coord7.setPosition(boardPx+5, cellSize*6 + cellSize/2 - 15);
                            coord8.setPosition(boardPx+5, cellSize*7 + cellSize/2 - 15);
                        } else {
                            // Для белых: сверху вниз (8-1)
                            coord8.setPosition(boardPx+5, cellSize*0 + cellSize/2 - 15);
                            coord7.setPosition(boardPx+5, cellSize*1 + cellSize/2 - 15);
                            coord6.setPosition(boardPx+5, cellSize*2 + cellSize/2 - 15);
                            coord5.setPosition(boardPx+5, cellSize*3 + cellSize/2 - 15);
                            coord4.setPosition(boardPx+5, cellSize*4 + cellSize/2 - 15);
                            coord3.setPosition(boardPx+5, cellSize*5 + cellSize/2 - 15);
                            coord2.setPosition(boardPx+5, cellSize*6 + cellSize/2 - 15);
                            coord1.setPosition(boardPx+5, cellSize*7 + cellSize/2 - 15);
                        }
                        
                        // Обновляем позиции буквенных координат
                        coordA.setPosition(cellSize*(renderer.isFlipped() ? 7 : 0) + cellSize/2 - 10, boardPx + coordSize/2 - 20);
                        coordB.setPosition(cellSize*(renderer.isFlipped() ? 6 : 1) + cellSize/2 - 10, boardPx + coordSize/2 - 20);
                        coordC.setPosition(cellSize*(renderer.isFlipped() ? 5 : 2) + cellSize/2 - 10, boardPx + coordSize/2 - 20);
                        coordD.setPosition(cellSize*(renderer.isFlipped() ? 4 : 3) + cellSize/2 - 10, boardPx + coordSize/2 - 20);
                        coordE.setPosition(cellSize*(renderer.isFlipped() ? 3 : 4) + cellSize/2 - 10, boardPx + coordSize/2 - 20);
                        coordF.setPosition(cellSize*(renderer.isFlipped() ? 2 : 5) + cellSize/2 - 10, boardPx + coordSize/2 - 20);
                        coordG.setPosition(cellSize*(renderer.isFlipped() ? 1 : 6) + cellSize/2 - 10, boardPx + coordSize/2 - 20);
                        coordH.setPosition(cellSize*(renderer.isFlipped() ? 0 : 7) + cellSize/2 - 10, boardPx + coordSize/2 - 20);
                    }

                    // Проверка нажатия кнопки Resign
                    if (mouseX >= boardPx + coordSize + 20 && mouseX <= boardPx + coordSize + panelWidth - 20 &&
                        mouseY >= 520 && mouseY <= 570) {
                        game.endGame();
                        moveHistory.push_back(game.getCurrentTurn() == Color::White ? "Black wins by resignation" : "White wins by resignation");
                    }

                    // Проверка нажатия кнопки Draw
                    if (mouseX >= boardPx + coordSize + 20 && mouseX <= boardPx + coordSize + panelWidth - 20 &&
                        mouseY >= 570 && mouseY <= 610) {
                        game.endGame();
                        moveHistory.push_back("Game drawn by agreement");
                    }
                }
            }

            if (event.type == sf::Event::TextEntered && isInputActive && !game.isGameOver()) {
                // Удаляем весь блок обработки текстового ввода
            }

            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) {
                    // Прокрутка вверх
                    scrollOffset = std::min(MAX_SCROLL, scrollOffset + SCROLL_SPEED);
                } else {
                    // Прокрутка вниз
                    scrollOffset = std::max(minScroll, scrollOffset - SCROLL_SPEED);
                }
            }
        }

        // Обновление цвета кнопок при наведении
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (!game.isGameOver()) {
            if (mousePos.x >= boardPx + coordSize + 20 && mousePos.x <= boardPx + coordSize + panelWidth - 20) {
                if (mousePos.y >= 470 && mousePos.y <= 510) {
                    flipBtn.setFillColor(sf::Color(235, 231, 225));
                } else {
                    flipBtn.setFillColor(sf::Color(245, 241, 235));
                }
                if (mousePos.y >= 520 && mousePos.y <= 570) {
                    resignBtn.setFillColor(sf::Color(235, 231, 225));
                } else {
                    resignBtn.setFillColor(sf::Color(245, 241, 235));
                }
                if (mousePos.y >= 570 && mousePos.y <= 610) {
                    drawBtn.setFillColor(sf::Color(235, 231, 225));
                } else {
                    drawBtn.setFillColor(sf::Color(245, 241, 235));
                }
            } else {
                flipBtn.setFillColor(sf::Color(245, 241, 235));
                resignBtn.setFillColor(sf::Color(245, 241, 235));
                drawBtn.setFillColor(sf::Color(245, 241, 235));
            }
        } else {
            flipBtn.setFillColor(sf::Color(220, 220, 220));
            resignBtn.setFillColor(sf::Color(220, 220, 220));
            drawBtn.setFillColor(sf::Color(220, 220, 220));
            resignText.setFillColor(sf::Color(150, 150, 150));
            drawText.setFillColor(sf::Color(150, 150, 150));
        }

        

        window.clear(sf::Color(245, 241, 235));

        
        

        renderer.drawBoard();
        
        // Подсветка выбранной фигуры
        if (isPieceSelected) {
            sf::RectangleShape highlight(sf::Vector2f(cellSize, cellSize));
            int displayCol = renderer.isFlipped() ? 7 - selectedCol : selectedCol;
            int displayRow = renderer.isFlipped() ? 7 - selectedRow : selectedRow;
            highlight.setPosition(sf::Vector2f(displayCol * cellSize, displayRow * cellSize));
            highlight.setFillColor(sf::Color(255, 255, 0, 100));
            window.draw(highlight);
        }
        
        renderer.drawPieces(board);
        renderer.drawPanel();

        window.draw(movesTitle);
        window.draw(historyBox);

        // Сохраняем текущий view
        sf::View defaultView = window.getView();
        
        // Устанавливаем view для истории
        window.setView(historyView);

        // Формируем текст истории
        sf::String historyString;
        for (const auto& move : moveHistory) {
            if (!move.isEmpty()) {
                historyString += move + sf::String("\n");
            }
        }
        historyText.setString(historyString);

        // Рассчитываем минимальное смещение для прокрутки
        sf::FloatRect textBounds = historyText.getGlobalBounds();
        float textHeight = textBounds.height;
        float boxHeight = historyBox.getSize().y - 10; // Оставляем небольшой отступ
        minScroll = std::min(0.0f, boxHeight - textHeight);

        // Применяем смещение
        //historyText.setPosition(boardPx + coordSize + 25, 65 + scrollOffset);
        historyText.setPosition(5, scrollOffset);
        window.draw(historyText);

        // Возвращаем стандартный view
        window.setView(defaultView);

        window.draw(flipBtn);
        window.draw(resignBtn);
        window.draw(drawBtn);
        window.draw(flipText);
        window.draw(resignText);
        window.draw(drawText);

        window.draw(coord1);
        window.draw(coord2);
        window.draw(coord3);
        window.draw(coord4);
        window.draw(coord5);
        window.draw(coord6);
        window.draw(coord7);
        window.draw(coord8);
        window.draw(coordA);
        window.draw(coordB);
        window.draw(coordC);
        window.draw(coordD);
        window.draw(coordE);
        window.draw(coordF);
        window.draw(coordG);
        window.draw(coordH);

        // Отрисовка окна выбора фигуры
        if (isPromotionWindowOpen) {
            window.draw(promotionWindow);
            for (int i = 0; i < 4; ++i) {
                window.draw(promotionButtons[i]);
                window.draw(promotionTexts[i]);
            }
        }

        window.display();
    }
    return 0;
}



