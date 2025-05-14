#pragma once

class Cell {
public:
    bool isMine = false;
    bool isRevealed = false;
    bool isFlagged = false;
    int adjacentMines = 0;

    char display() const;
};
