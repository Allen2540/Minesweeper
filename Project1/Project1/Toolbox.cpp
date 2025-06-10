#include "Toolbox.hpp"

Toolbox::Toolbox(Board& board, int l, int c, int r, int p) : boardRef(board), mineTries(l), checkTries(c), safeTries(r), probeTries(p) {
}

int Toolbox::getMineTries() const { return mineTries; }
int Toolbox::getCheckTries() const { return checkTries; }
int Toolbox::getSafeTries() const { return safeTries; }
int Toolbox::getProbeTries() const { return probeTries; }

void Toolbox::locateMine() {
    if (mineTries <= 0) {
        cout << " 定位地雷道具已用完。\n";
        return;
    }
    mineTries--;

    int rows = boardRef.getRow();
    int cols = boardRef.getCol();

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            Cell cell = boardRef.getCell(r, c);

            if (cell.isMine && !cell.isFlagged && !cell.isRevealed) {
                cout << "提示：其中一顆地雷位於 (" << r << ", " << c << ")\n";
                return;
            }
        }
    }

    cout << "目前找不到未揭露或未標記的地雷。\n";
}
void Toolbox::checkFlagErrors() {
    if (checkTries <= 0) {
        std::cout << " 錯誤旗子檢查道具已用完。\n";
        return;
    }
    
    int rows = boardRef.getRow();
    int cols = boardRef.getCol();

    int wrongFlags = 0, totalFlags = 0;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            Cell cell = boardRef.getCell(r, c);
            if (cell.isFlagged) {
                totalFlags++;
                if (!cell.isMine) {
                    wrongFlags++;
                }
            }
        }
    }

    if (totalFlags == 0) {
        cout << "你目前尚未插任何旗子。\n";
    }
    else if (wrongFlags == 0) {
        checkTries--;
        cout << "太棒了！你目前所有的旗子都插對了。\n";
    }
    else {
        checkTries--;
        cout << "警告：你目前有 " << wrongFlags << " 個旗子插錯了！\n";
    }
}
void Toolbox::revealSafeCell() {
    if (safeTries <= 0) {
        std::cout << " 安全格道具已用完。\n";
        return;
    }
    safeTries--;
    int rows = boardRef.getRow();
    int cols = boardRef.getCol();
    vector<pair<int, int>> safeCells;

    // 收集所有未被揭示且不是地雷的格子
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            Cell cell = boardRef.getCell(r, c);
            if (!cell.isMine && !cell.isRevealed && !cell.isFlagged) {
                safeCells.push_back({ r, c });
            }
        }
    }

    if (safeCells.empty()) {
        cout << " 沒有可揭示的安全格子了。\n";
        return;
    }

    srand(static_cast<unsigned>(time(0)));
    int idx = rand() % safeCells.size();
    int r = safeCells[idx].first;
    int c = safeCells[idx].second;

    boardRef.reveal(r, c);
    cout << "幫你揭示了 (" << r << ", " << c << ")，這是一個安全格！\n";
}
void Toolbox::probeCell(int r, int c) {
    if (probeTries <= 0) {
        std::cout << "探測道具已用完。\n";
        return;
    }
    probeTries--;
    if (!boardRef.inBounds(r, c)) {
        cout << " 位置 (" << r << ", " << c << ") 超出邊界。\n";
        return;
    }

    const Cell& cell = boardRef.getCell(r, c);
    if (cell.isMine) {
       cout << "這一格 (" << r << ", " << c << ") 是地雷！\n";
    }
    else {
        cout << "這一格 (" << r << ", " << c << ") 是安全的。\n";
    }
}
