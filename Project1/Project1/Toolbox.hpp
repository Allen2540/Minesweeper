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
    void locateMine();              // ��ܤ@���a�p����m
    void checkFlagErrors();         // �ˬd�X�l�O�_�����~
    void revealSafeCell();          // �H�����ܤ@��w����
    void probeCell(int r, int c);   // �����Y��O�_���a�p

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