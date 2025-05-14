#pragma once
#include <string>

class Player {
public:
    Player(const std::string& name, int score);
    std::string getName() const;
    int getScore() const;
    void setScore(int score);
    bool operator<(const Player& other) const;

private:
    std::string name;
    int score;
};
