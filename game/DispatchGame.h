#pragma once
#include "../core/PatternGrid.h"
#include "../roles/Dispatcher.h"
#include "../roles/Messenger.h"
#include "../roles/Builder.h"
#include "../core/MessageSystem.h"
#include "../data/GameData.h"
#include <memory>

class DispatchGame {
public:
    DispatchGame(const PatternGrid& targetPattern, Difficulty difficulty = Difficulty::NORMAL);
    
    // Core gameplay
    void playEpisode();
    void playTurn();
    bool isComplete() const;
    
    // Game state
    void saveGameState(const std::string& slotName = "auto");
    bool loadGameState(const std::string& slotName = "auto");
    
    // Getters
    const PatternGrid& getTargetPattern() const { return dispatcher->getTargetPattern(); }
    const PatternGrid& getBuilderGrid() const { return builder->getCurrentGrid(); }
    double getAccuracy() const;
    
    // Metrics
    GameMetrics calculateMetrics() const;

private:
    std::unique_ptr<Dispatcher> dispatcher;
    std::unique_ptr<Messenger> messenger;
    std::unique_ptr<Builder> builder;
    std::shared_ptr<MessageNoiseSimulator> noiseSimulator;
    
    GameData gameData;
    Difficulty difficulty;
    
    int totalTurns;
    int currentTurn;
    int maxTurns;
    int messageLimit;
    int messagesUsed;
    
    std::chrono::steady_clock::time_point startTime;
    
    void initializeGame();
    void processHumanTurn();
    void showResults();
    void showEpisodeSummary();
    
    bool checkTimeLimit() const;
    void applyDifficultySettings();
};