#pragma once
#include "Board.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;
class Toolbox {
public:
    Toolbox(Board& board,int l,int c,int r,int p);
    void locateMine();              // 顯示一顆地雷的位置
    void checkFlagErrors();         // 檢查旗子是否有錯誤
    void revealSafeCell();          // 隨機揭示一格安全格
    void probeCell(int r, int c);   // 探測某格是否為地雷

    int getMineTries() const;
    int getCheckTries() const;
    int getSafeTries() const;
    int getProbeTries() const;
private:
    Board& boardRef;
    int mineTries;
    int checkTries;
    int safeTries;
    int probeTries;
};