#include "Cell.hpp"

char Cell::display() const {
    if (isFlagged) return 'F';
    if (!isRevealed) return '#';
    if (isMine) return '*';
    return adjacentMines == 0 ? ' ' : '0' + adjacentMines;
}
