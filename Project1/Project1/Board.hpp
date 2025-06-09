#pragma once
#include <vector>
#include "Cell.hpp"

class Board {
public:
    Board(int rows, int cols, int mines);
    void display(bool revealAll = false) const;
    bool reveal(int row, int col);
    void toggleFlag(int row, int col);
    bool isCleared() const;
    int getRow()const;
    int getCol()const;
    Cell getCell(int r, int c) const;
    bool inBounds(int r, int c) const;
private:
    int rows, cols, totalMines;
    std::vector<std::vector<Cell>> grid;
    void placeMines();
    void calculateAdjacency();
    void revealRecursive(int r, int c);
};
