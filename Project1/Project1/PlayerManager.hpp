#pragma once
#include <vector>
#include "Player.hpp"

class PlayerManager {
public:
    void addPlayer(const Player& player);
    void loadLeaderboard();
    void saveLeaderboard() const;
    void showLeaderboard() const;
    void updatePlayerScore(const std::string& name, int score);

private:
    std::vector<Player> players;
    const std::string leaderboardFile = "leaderboard.txt";
};
