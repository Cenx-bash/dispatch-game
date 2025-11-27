#pragma once
#include <string>
#include <vector>
#include <chrono>
#include "../core/PatternGrid.h"

enum class Difficulty {
    TRAINING = 0,
    NORMAL = 1,
    HARD = 2,
    EXPERT = 3
};

enum class Role {
    DISPATCHER = 0,
    MESSENGER = 1, 
    BUILDER = 2
};

enum class AILevel {
    BASIC = 0,
    INTERMEDIATE = 1,
    ADVANCED = 2
};

struct GameMetrics {
    double accuracy;
    int turnsTaken;
    int messagesUsed;
    int messengerMistakes;
    int dispatcherClarity;
    std::chrono::seconds timeElapsed;
    int score;
    
    // Advanced metrics
    double communicationEfficiency;
    double errorRecoveryRate;
    double patternRecognitionScore;
};

struct PerformanceReport {
    GameMetrics metrics;
    std::string overallGrade;
    std::vector<std::string> strengths;
    std::vector<std::string> improvements;
    int skillPointsEarned;
    std::string nextEpisodeUnlock;
};

struct SaveData {
    std::string saveName;
    std::chrono::system_clock::time_point saveTime;
    int episodeNumber;
    Difficulty difficulty;
    GameMetrics currentMetrics;
};

class GameData {
public:
    GameData();
    
    // Player progression
    void addCompletedEpisode(int episode, const GameMetrics& metrics);
    void unlockAchievement(const std::string& achievement);
    void addSkillPoints(int points);
    
    // Statistics
    int getTotalGamesPlayed() const;
    double getAverageAccuracy() const;
    int getHighestScore() const;
    std::vector<std::string> getUnlockedAchievements() const;
    
    // Settings
    void setDifficulty(Difficulty diff);
    void enableTutorial(bool enable);
    
    // Getters
    Difficulty getDifficulty() const { return currentDifficulty; }
    int getSkillPoints() const { return skillPoints; }
    int getCurrentEpisode() const { return currentEpisode; }
    bool isTutorialEnabled() const { return tutorialEnabled; }

private:
    Difficulty currentDifficulty;
    int skillPoints;
    int currentEpisode;
    bool tutorialEnabled;
    std::vector<std::string> unlockedAchievements;
    std::vector<GameMetrics> gameHistory;
};