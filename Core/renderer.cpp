#include "renderer.h"
#include <iostream>

ChessRenderer::ChessRenderer(sf::RenderWindow& window, const std::string& spritesheetPath, const sf::Vector2i& pieceSize)
    : window(window)
    , cellSize(80)
    , boardPx(cellSize * 8)
    , panelWidth(320)
    , pieceSize(pieceSize)
    , lightColor(240, 217, 181)
    , darkColor(181, 136, 99)
    , panelColor(245, 241, 235)
{
    loadSpritesheet(spritesheetPath);
    pieceSprite.setTexture(spritesheetTexture);
    float scale = static_cast<float>(cellSize) / pieceSize.x;
    pieceSprite.setScale(sf::Vector2f(scale, scale));
}

void ChessRenderer::loadSpritesheet(const std::string& spritesheetPath) {
    if (!spritesheetTexture.loadFromFile(spritesheetPath)) {
        std::cerr << "Ошибка загрузки спрайтшита: " << spritesheetPath << std::endl;
    }
}

sf::IntRect ChessRenderer::getPieceTextureRect(Color color, char symbol) const {
    int row = (color == Color::White) ? 0 : 1;
    int col = -1;
    
    switch (symbol) {
        case 'K': case 'k': col = 0; break;
        case 'Q': case 'q': col = 1; break;
        case 'R': case 'r': col = 2; break;
        case 'B': case 'b': col = 3; break;
        case 'N': case 'n': col = 4; break;
        case 'P': case 'p': col = 5; break;
        default: return sf::IntRect();
    }
    
    if (col != -1) {
        return sf::IntRect(col * pieceSize.x, row * pieceSize.y, pieceSize.x, pieceSize.y);
    }
    return sf::IntRect();
}

void ChessRenderer::drawBoard() {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
            cell.setPosition(sf::Vector2f(col * cellSize, row * cellSize));
            cell.setFillColor((row + col) % 2 == 0 ? lightColor : darkColor);
            window.draw(cell);
        }
    }
}

void ChessRenderer::drawPieces(const Board& board) {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            auto piece = board.getPiece(row, col);
            if (piece) {
                sf::IntRect textureRect = getPieceTextureRect(piece->getColor(), piece->getSymbol());
                
                if (textureRect != sf::IntRect()) {
                    pieceSprite.setTextureRect(textureRect);
                    pieceSprite.setPosition(sf::Vector2f(col * cellSize, row * cellSize));
                    window.draw(pieceSprite);
                }
            }
        }
    }
}

void ChessRenderer::drawPanel() {
    sf::RectangleShape panelBg(sf::Vector2f(panelWidth, window.getSize().y));
    panelBg.setPosition(sf::Vector2f(boardPx, 0));
    panelBg.setFillColor(panelColor);
    window.draw(panelBg);
}

bool ChessRenderer::getSquareFromMouse(int mouseX, int mouseY, int& row, int& col) const {
    if (mouseX < 0 || mouseX >= boardPx || mouseY < 0 || mouseY >= boardPx) {
        return false;
    }
    
    col = mouseX / cellSize;
    row = mouseY / cellSize;
    return true;
} 