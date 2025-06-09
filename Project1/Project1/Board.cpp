#include "Board.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>

Board::Board(int r, int c, int m) : rows(r), cols(c), totalMines(m) {
    grid.resize(rows, std::vector<Cell>(cols));
    placeMines();
    calculateAdjacency();
}

void Board::placeMines() {
    srand(time(0));
    int placed = 0;
    while (placed < totalMines) {
        int r = rand() % rows;
        int c = rand() % cols;
        if (!grid[r][c].isMine) {
            grid[r][c].isMine = true;
            placed++;
        }
    }
}

void Board::calculateAdjacency() {
    int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c].isMine) continue;
            int count = 0;
            for (int i = 0; i < 8; ++i) {
                int nr = r + dx[i], nc = c + dy[i];
                if (inBounds(nr, nc) && grid[nr][nc].isMine) {
                    count++;
                }
            }
            grid[r][c].adjacentMines = count;
        }
    }
}

bool Board::inBounds(int r, int c) const {
    return r >= 0 && r < rows && c >= 0 && c < cols;
}

void Board::display(bool revealAll) const {
    std::cout << "   ";
    for (int c = 0; c < cols; ++c)
        std::cout << c << " ";
    std::cout << "\n";

    for (int r = 0; r < rows; ++r) {
        std::cout << r << " ";
        if (r < 10) std::cout << " ";
        for (int c = 0; c < cols; ++c) {
            const Cell& cell = grid[r][c];
            if (revealAll) {
                if (cell.isMine)
                    std::cout << "* ";
                else
                    std::cout << cell.display() << " ";
            }
            else {
                std::cout << cell.display() << " ";
            }
        }
        std::cout << "\n";
    }
}


bool Board::reveal(int r, int c) {
    if (!inBounds(r, c) || grid[r][c].isRevealed || grid[r][c].isFlagged)
        return false;
    if (grid[r][c].isMine) {
        grid[r][c].isRevealed = true;
        return true;
    }
    revealRecursive(r, c);
    return false;
}

void Board::revealRecursive(int r, int c) {
    if (!inBounds(r, c) || grid[r][c].isRevealed || grid[r][c].isFlagged)
        return;
    grid[r][c].isRevealed = true;
    if (grid[r][c].adjacentMines == 0) {
        for (int dr = -1; dr <= 1; ++dr)
            for (int dc = -1; dc <= 1; ++dc)
                if (dr || dc) revealRecursive(r + dr, c + dc);
    }
}

void Board::toggleFlag(int r, int c) {
    if (!inBounds(r, c) || grid[r][c].isRevealed)
        return;
    grid[r][c].isFlagged = !grid[r][c].isFlagged;
}

bool Board::isCleared() const {
    for (const auto& row : grid)
        for (const auto& cell : row)
            if (!cell.isMine && !cell.isRevealed)
                return false;
    return true;
}

int Board::getRow()const {
    return rows;
}
int Board::getCol()const {
    return cols;
}
Cell Board::getCell(int r, int c) const {
    if (inBounds(r, c)) return grid[r][c];
    return Cell();
}