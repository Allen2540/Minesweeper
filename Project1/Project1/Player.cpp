#include "Player.hpp"

Player::Player(const std::string& name, int score) : name(name), score(score) {}

std::string Player::getName() const {
    return name;
}

int Player::getScore() const {
    return score;
}

void Player::setScore(int newScore) {
    score = newScore;
}

bool Player::operator<(const Player& other) const {
    return score > other.score;  // 以分數降序排序
}
