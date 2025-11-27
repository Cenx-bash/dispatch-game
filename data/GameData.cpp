#include "GameData.h"
#include <algorithm>

using namespace std;

GameData::GameData() 
    : currentDifficulty(Difficulty::NORMAL), 
      skillPoints(0), 
      currentEpisode(1),
      tutorialEnabled(true) {}

void GameData::addCompletedEpisode(int episode, const GameMetrics& metrics) {
    gameHistory.push_back(metrics);
    currentEpisode = max(currentEpisode, episode + 1);
    
    // Award skill points based on performance
    int points = static_cast<int>(metrics.accuracy / 10) + 
                 (metrics.score / 100) + 
                 (metrics.accuracy == 100.0 ? 5 : 0);
    
    addSkillPoints(points);
}

void GameData::unlockAchievement(const std::string& achievement) {
    if (find(unlockedAchievements.begin(), unlockedAchievements.end(), achievement) == unlockedAchievements.end()) {
        unlockedAchievements.push_back(achievement);
    }
}

void GameData::addSkillPoints(int points) {
    skillPoints += points;
}

int GameData::getTotalGamesPlayed() const {
    return gameHistory.size();
}

double GameData::getAverageAccuracy() const {
    if (gameHistory.empty()) return 0.0;
    
    double total = 0.0;
    for (const auto& metrics : gameHistory) {
        total += metrics.accuracy;
    }
    return total / gameHistory.size();
}

int GameData::getHighestScore() const {
    int highest = 0;
    for (const auto& metrics : gameHistory) {
        if (metrics.score > highest) {
            highest = metrics.score;
        }
    }
    return highest;
}

vector<string> GameData::getUnlockedAchievements() const {
    return unlockedAchievements;
}

void GameData::setDifficulty(Difficulty diff) {
    currentDifficulty = diff;
}

void GameData::enableTutorial(bool enable) {
    tutorialEnabled = enable;
}