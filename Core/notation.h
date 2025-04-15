#ifndef NOTATION_H
#define NOTATION_H

#include <string>
#include "game.h"

struct ParsedMove {
    int fromRow, fromCol;
    int toRow, toCol;
    bool valid;
};

class Notation {
public:
    static ParsedMove parseMoveNotation(const Game& game, const std::string& input);
};

#endif // NOTATION_H
