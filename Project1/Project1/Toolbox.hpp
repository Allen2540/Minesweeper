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
    void locateMine();              // �p���w�쾹
    void checkFlagErrors();         // ���~������
    void revealSafeCell();          // �w�����޾�
    void probeCell(int r, int c);   // �a�p�P����

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