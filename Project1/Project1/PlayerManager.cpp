#include "PlayerManager.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>

void PlayerManager::addPlayer(const Player& player) {
    players.push_back(player);
}

void PlayerManager::loadLeaderboard() {
    std::ifstream file(leaderboardFile);
    if (file.is_open()) {
        std::string name;
        int score;
        while (file >> name >> score) {
            players.emplace_back(name, score);
        }
        file.close();
    }
}

void PlayerManager::saveLeaderboard() const {
    std::ofstream file(leaderboardFile);
    if (file.is_open()) {
        for (const auto& player : players) {
            file << player.getName() << " " << player.getScore() << "\n";
        }
        file.close();
    }
}

void PlayerManager::showLeaderboard() const {
    std::cout << "\n�i�Ʀ�]�j\n";
    if (players.empty()) {
        std::cout << "�ثe�S�����a�����C\n";
        return;
    }

    int rank = 1;
    for (const auto& player : players) {
        std::cout << rank++ << ". " << player.getName() << " - " << player.getScore() << " ��\n";
    }
}

void PlayerManager::updatePlayerScore(const std::string& name, int score) {
    auto it = std::find_if(players.begin(), players.end(), [&name](const Player& p) {
        return p.getName() == name;
        });

    if (it != players.end()) {
        if (it->getScore() < score) {
            it->setScore(score);  // ��s����
        }
    }
    else {
        addPlayer(Player(name, score));  // �Y�S��쪱�a�h�s�W
    }
}
