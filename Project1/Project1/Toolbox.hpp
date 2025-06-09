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
    void locateMine();              // 雷源定位器
    void checkFlagErrors();         // 錯誤偵測儀
    void revealSafeCell();          // 安全指引器
    void probeCell(int r, int c);   // 地雷感測器

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