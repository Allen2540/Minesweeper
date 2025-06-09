#include "GameManager.hpp"
#include "Utils.hpp"
#include "Board.hpp"
#include "PlayerManager.hpp"
#include "Toolbox.hpp"
#include <iostream>
#include <limits>
#include <sstream>

void GameManager::run() {
    std::cout << "�п�J�b���W�١G";
    std::getline(std::cin, currentUser);
    Utils::clearScreen();
    showMainMenu();
}

void GameManager::showMainMenu() {
    while (true) {
        std::cout << "\n====== �D��� ======\n";
        std::cout << "1. �C�����k����\n";
        std::cout << "2. �d�ݱƦ�]\n";
        std::cout << "3. ���פ���\n";
        std::cout << "4~10. �}�l�C���]���� 1~7�^\n";
        std::cout << "��J return �i�^�����]�~���h�X���O���^\n";
        std::cout << "�п�J�ﶵ�]1-10 �� return�^�G";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "return") {
            Utils::clearScreen();
            continue;
        }

        handleMenuChoice(choice);
    }
}

void GameManager::handleMenuChoice(const std::string& choice) {
    Utils::clearScreen();

    if (choice == "1") {
        howToPlay();
    }
    else if (choice == "2") {
        showLeaderboard();
    }
    else if (choice == "3") {
        showDifficultyInfo();
    }
    else {
        try {
            int num = std::stoi(choice);
            if (num >= 4 && num <= 10) {
                int level = num - 3;  // ��������1~7
                startGame(level);
            }
            else {
                std::cout << "?? �L�Ŀﶵ�A�п�J 1 ~ 10�C\n";
            }
        }
        catch (...) {
            std::cout << "?? �п�J���Ī��ﶵ�s���]1 ~ 10�^�C\n";
        }
    }
}




void GameManager::howToPlay() {
    std::cout << "\n??�i��a�p���k�����j??\n";
    std::cout << "�ؼСG�N�Ҧ����O�a�p����l�������}�I\n";
    std::cout << "���O�榡�G\n";
    std::cout << "  r �� �C �� ���}�Ӯ�\n";
    std::cout << "  f �� �C �� ���X�Ψ����X�l\n";
    std::cout << "��J return �i�H�ɦ^�����A�h�X���O���C\n\n";
}

void GameManager::showLeaderboard() {
    PlayerManager playerManager;
    playerManager.loadLeaderboard();
    playerManager.showLeaderboard();
}

void GameManager::showDifficultyInfo() {
    std::cout << "\n??�i���פ��Сj\n";
    std::cout << "���� 1�G5x5�A3 ���a�p�A�o�� 10\n";
    std::cout << "���� 2�G6x6�A6 ���a�p�A�o�� 20\n";
    std::cout << "���� 3�G8x8�A10 ���a�p�A�o�� 30\n";
    std::cout << "���� 4�G10x10�A15 ���a�p�A�o�� 50\n";
    std::cout << "���� 5�G12x12�A20 ���a�p�A�o�� 70\n";
    std::cout << "���� 6�G14x14�A30 ���a�p�A�o�� 90\n";
    std::cout << "���� 7�G16x16�A40 ���a�p�A�o�� 120\n\n";
}

void GameManager::startGame(int level) {
    // �ٲ���l�� Board �M�C���޿賡���A�Ӥ��e���覡
    int sizes[] = { 5, 6, 8, 10, 12, 14, 16 };
    int mines[] = { 3, 6, 10, 15, 20, 30, 40 };
    int score[] = { 10, 20, 30, 50, 70, 90, 120 };

    int rows = sizes[level - 1];
    int cols = sizes[level - 1];
    int mineCount = mines[level - 1];

    Board board(rows, cols, mineCount);
    int errors = 0;
    std::string input;

    PlayerManager playerManager;
    playerManager.loadLeaderboard();

    Toolbox t(board,level, level, level+2, level);
    while (true) {
        board.display();
        std::cout << "���~������(l): " << t.getMineTries() << std::endl<<"�p���w�쾹(c): "<<t.getCheckTries()<<std::endl<<"�w�����޾�(s): "<<t.getSafeTries()<<std::endl<<"�a�p�P����(p): "<<t.getProbeTries()<<std::endl;
        std::cout << "���O r=����(�� �C)�Af=���X(�� �C)�Ap=�a�p�P����(�� �C)�Al=���~�������Ac=�p���w�쾹�As=�w�����޾��A�ο�Jreturn ��^�D���G";
        std::getline(std::cin, input);
        if (input == "return") {
            std::cout << "�w��^�D���A���Z���O���C\n";
            return;
        }

        std::stringstream ss(input);
        char cmd;
        int r, c;
        ss >> cmd >> r >> c;
        if (cmd == 'r') {
            if (board.reveal(r, c)) {
                errors++;
                std::cout << "?? �A���a�p�F�I�]���~���� " << errors << "/3�^\n";
                if (errors >= 3) {
                    board.display(true);
                    std::cout << "�C�����ѡA�w�W�L 3 �����~�C\n";
                    return;
                }
            }
        }
        else if (cmd == 'f') {
            board.toggleFlag(r, c);
        }
        else if (cmd == 'l') {
            t.locateMine();
        }
        else if (cmd == 'c') {
            t.checkFlagErrors();
        }
        else if (cmd == 's') {
            t.revealSafeCell();
        }
        else if (cmd == 'p') {
            t.probeCell(r, c);
        }

        if (board.isCleared()) {
            std::cout << "?? ���ߧA�����C���I�o���G" << score[level - 1] << " ��\n";
            playerManager.updatePlayerScore(currentUser, score[level - 1]);
            playerManager.saveLeaderboard();
            return;
        }
    }
}
