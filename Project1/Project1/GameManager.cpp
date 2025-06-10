#include "GameManager.hpp"
#include "Utils.hpp"
#include "Board.hpp"
#include "PlayerManager.hpp"
#include "Toolbox.hpp"
#include <iostream>
#include <limits>
#include <sstream>
#include <chrono>

void GameManager::run() {
    std::cout << "請輸入帳號名稱：";
    std::getline(std::cin, currentUser);
    Utils::clearScreen();
    showMainMenu();
}

void GameManager::showMainMenu() {
    while (true) {
        std::cout << "\n====== 主選單 ======\n";
        std::cout << "1. 遊戲玩法和道具說明\n";
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
    std::cout << "  p 行 列 → 地雷感測器 點擊後可選一格，顯示是否為地雷\n";
    std::cout << "  l  →雷源定位器 定位其中一顆地雷\n";
    std::cout << "  c  錯誤偵測儀 通知你當前插的旗子中是否有錯誤\n";
    std::cout << "  s  安全指引器 隨機揭示一格安全格\n";
    std::cout << "輸入 return 可隨時回首頁，退出不記分。\n\n";
}

void GameManager::showLeaderboard() {
    PlayerManager playerManager;
    playerManager.loadLeaderboard();
    playerManager.showLeaderboard();
}

void GameManager::showDifficultyInfo() {
    std::cout << "\n??【難度介紹】\n";
    std::cout << "難度 1：5x5，3 顆地雷，得分 10，20秒\n";
    std::cout << "難度 2：6x6，6 顆地雷，得分 20，200秒\n";
    std::cout << "難度 3：8x8，10 顆地雷，得分 30，220秒\n";
    std::cout << "難度 4：10x10，15 顆地雷，得分 50，240秒\n";
    std::cout << "難度 5：12x12，20 顆地雷，得分 70，270秒\n";
    std::cout << "難度 6：14x14，30 顆地雷，得分 90，300秒\n";
    std::cout << "難度 7：16x16，40 顆地雷，得分 120，360秒\n\n";
}

void GameManager::startGame(int level) {
    int sizes[] = { 5, 6, 8, 10, 12, 14, 16 };
    int mines[] = { 3, 6, 10, 15, 20, 30, 40 };
    int score[] = { 10, 20, 30, 50, 70, 90, 120 };
    int timeLimits[] = { 20, 200, 220, 240, 270, 300, 360 }; // 每個難度的秒數限制，倒數功能
    int timeLimit = timeLimits[level - 1];

    int rows = sizes[level - 1];
    int cols = sizes[level - 1];
    int mineCount = mines[level - 1];

    Board board(rows, cols, mineCount);
    int errors = 0;
    std::string input;

    PlayerManager playerManager;
    playerManager.loadLeaderboard();

    auto startTime = std::chrono::steady_clock::now(); //倒數功能

    Toolbox t(board,level, level, level, level + 2);
    while (true) {
        auto now = std::chrono::steady_clock::now();
        int elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
        int remaining = timeLimit - elapsed;

        std::cout << "================================================\n";
        std::cout << "剩餘時間：" << remaining << " 秒\n";

        if (remaining <= 0) {
            std::cout << "⏰ 時間到！遊戲結束。\n";
            board.display(true);
            return;
        }  //倒數功能
        board.display();
        std::cout << "雷源定位器(l): " << t.getMineTries() << std::endl<<"錯誤偵測儀(c): "<<t.getCheckTries()<<std::endl<<"安全指引器(s): "<<t.getSafeTries()<<std::endl<<"地雷感測器(p): "<<t.getProbeTries()<<std::endl;
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
                std::cout << "💥 你踩到地雷了！";
                if (errors >= 1) {
                    board.display(true);
                    std::cout << "遊戲失敗，已超過 1次錯誤。\n";
                    return;
                }
            }
        }
        else if (cmd == 'f') {
            board.toggleFlag(r, c);
        }
        else if (cmd == 'l') {
            std::cout << "================================================\n";
            t.locateMine();
        }
        else if (cmd == 'c') {
            std::cout << "================================================\n";
            t.checkFlagErrors();
        }
        else if (cmd == 's') {
            std::cout << "================================================\n";
            t.revealSafeCell();
        }
        else if (cmd == 'p') {
            std::cout << "================================================\n";
            t.probeCell(r, c);
        }

        if (board.isCleared()) {
            Utils::clearScreen();
            board.display(true);
            std::cout << "🎉 恭喜你完成遊戲！得分：" << score[level - 1] << " 分\n";
            playerManager.updatePlayerScore(currentUser, score[level - 1]);
            playerManager.saveLeaderboard();
            return;
        }
    }
}