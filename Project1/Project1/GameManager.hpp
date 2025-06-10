#pragma once
#include <string>

class GameManager {
public:
    void run();
private:
    std::string currentUser;
    void showMainMenu();
    void handleMenuChoice(const std::string& choice);
    void howToPlay();
    void showLeaderboard();
    void showDifficultyInfo();
    void startGame(int level);
    void showAdAndWait();
    bool askRevive();
};
