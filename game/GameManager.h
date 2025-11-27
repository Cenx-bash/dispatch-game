#pragma once
#include "DispatchGame.h"
#include "EpisodeManager.h"
#include "../data/GameData.h"
#include "../data/SaveSystem.h"
#include <string>
#include <vector>

class GameManager {
public:
    GameManager();
    void run();

private:
    GameData gameData;
    EpisodeManager episodeManager;
    
    void showMainMenu();
    void startNewGame();
    void continueGame();
    void showOptions();
    void showStatistics();
    void showHelp();
    
    Difficulty selectDifficulty();
    Episode selectEpisode();
    void playEpisode(const Episode& episode);
};