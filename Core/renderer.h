#ifndef RENDERER_H
#define RENDERER_H

#include <SFML/Graphics.hpp>
#include "board.h"
#include <string>
#include <memory>
#include <unordered_map>

class ChessRenderer {
public:
    ChessRenderer(sf::RenderWindow& window, const sf::Texture& spritesheet, const sf::Vector2i& pieceSize);
    
    void drawBoard();
    void drawPieces(const Board& board);
    void drawPanel();
    
    // Получить позицию клетки по координатам мыши
    bool getSquareFromMouse(int mouseX, int mouseY, int& row, int& col) const;
    
    // Получить размер клетки
    int getCellSize() const { return cellSize; }
    
    // Получить размер доски в пикселях
    int getBoardSize() const { return boardPx; }

private:
    sf::RenderWindow& window;
    int cellSize;
    int boardPx;
    int panelWidth;
    sf::Vector2i pieceSize;
    
    // Цвета
    sf::Color lightColor;
    sf::Color darkColor;
    sf::Color panelColor;
    
    // Текстура спрайтшита и спрайт для отрисовки
    sf::Texture spritesheetTexture;
    sf::Sprite pieceSprite;
    
    sf::IntRect getPieceTextureRect(Color color, char symbol) const;
};

#endif // RENDERER_H 