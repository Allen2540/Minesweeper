#include "GameManager.hpp"
#include "Utils.hpp"
#include "Board.hpp"
#include "PlayerManager.hpp"
#include "Toolbox.hpp"
#include <iostream>
#include <limits>
#include <sstream>

void GameManager::run() {
    std::cout << "請輸入帳號名稱：";
    std::getline(std::cin, currentUser);
    Utils::clearScreen();
    showMainMenu();
}

void GameManager::showMainMenu() {
    while (true) {
        std::cout << "\n====== 主選單 ======\n";
        std::cout << "1. 遊戲玩法說明\n";
        std::cout << "2. 查看排行榜\n";
        std::cout << "3. 難度介紹\n";
        std::cout << "4~10. 開始遊戲（難度 1~7）\n";
        std::cout << "輸入 return 可回首頁（途中退出不記分）\n";
        std::cout << "請輸入選項（1-10 或 return）：";

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
                int level = num - 3;  // 對應難度1~7
                startGame(level);
            }
            else {
                std::cout << "?? 無效選項，請輸入 1 ~ 10。\n";
            }
        }
        catch (...) {
            std::cout << "?? 請輸入有效的選項編號（1 ~ 10）。\n";
        }
    }
}




void GameManager::howToPlay() {
    std::cout << "\n??【踩地雷玩法說明】??\n";
    std::cout << "目標：將所有不是地雷的格子全部打開！\n";
    std::cout << "指令格式：\n";
    std::cout << "  r 行 列 → 打開該格\n";
    std::cout << "  f 行 列 → 插旗或取消旗子\n";
    std::cout << "輸入 return 可隨時回首頁，退出不記分。\n\n";
}

void GameManager::showLeaderboard() {
    PlayerManager playerManager;
    playerManager.loadLeaderboard();
    playerManager.showLeaderboard();
}

void GameManager::showDifficultyInfo() {
    std::cout << "\n??【難度介紹】\n";
    std::cout << "難度 1：5x5，3 顆地雷，得分 10\n";
    std::cout << "難度 2：6x6，6 顆地雷，得分 20\n";
    std::cout << "難度 3：8x8，10 顆地雷，得分 30\n";
    std::cout << "難度 4：10x10，15 顆地雷，得分 50\n";
    std::cout << "難度 5：12x12，20 顆地雷，得分 70\n";
    std::cout << "難度 6：14x14，30 顆地雷，得分 90\n";
    std::cout << "難度 7：16x16，40 顆地雷，得分 120\n\n";
}

void GameManager::startGame(int level) {
    // 省略初始化 Board 和遊戲邏輯部分，照之前的方式
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
        std::cout << "錯誤偵測儀(l): " << t.getMineTries() << std::endl<<"雷源定位器(c): "<<t.getCheckTries()<<std::endl<<"安全指引器(s): "<<t.getSafeTries()<<std::endl<<"地雷感測器(p): "<<t.getProbeTries()<<std::endl;
        std::cout << "指令 r=揭示(行 列)，f=插旗(行 列)，p=地雷感測器(行 列)，l=錯誤偵測儀，c=雷源定位器，s=安全指引器，或輸入return 返回主選單：";
        std::getline(std::cin, input);
        if (input == "return") {
            std::cout << "已返回主選單，成績不記錄。\n";
            return;
        }

        std::stringstream ss(input);
        char cmd;
        int r, c;
        ss >> cmd >> r >> c;
        if (cmd == 'r') {
            if (board.reveal(r, c)) {
                errors++;
                std::cout << "?? 你踩到地雷了！（錯誤次數 " << errors << "/3）\n";
                if (errors >= 3) {
                    board.display(true);
                    std::cout << "遊戲失敗，已超過 3 次錯誤。\n";
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
            std::cout << "?? 恭喜你完成遊戲！得分：" << score[level - 1] << " 分\n";
            playerManager.updatePlayerScore(currentUser, score[level - 1]);
            playerManager.saveLeaderboard();
            return;
        }
    }
}
