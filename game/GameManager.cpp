#include "GameManager.h"
#include "../ui/ConsoleUI.h"
#include "../ui/CutsceneManager.h"
#include "../utils/Random.h"
#include <iostream>
#include <cstdlib>

using namespace std;

GameManager::GameManager() {
    // Initialize random number generator
    Random::initialize();
}

void GameManager::run() {
    while (true) {
        showMainMenu();
    }
}

void GameManager::showMainMenu() {
    ConsoleUI::clearScreen();
    ConsoleUI::showTitle("🚚 CLEARLINE DISPATCH");
    
    vector<string> options = {
        "New Game",
        "Continue Game", 
        "Options",
        "Statistics",
        "Help",
        "Quit"
    };
    
    int choice = ConsoleUI::getChoice("Main Menu:", options);
    
    switch (choice) {
        case 1:
            startNewGame();
            break;
        case 2:
            continueGame();
            break;
        case 3:
            showOptions();
            break;
        case 4:
            showStatistics();
            break;
        case 5:
            showHelp();
            break;
        case 6:
            cout << "Thanks for playing Clearline Dispatch!\n";
            exit(0);
    }
}

void GameManager::startNewGame() {
    Difficulty difficulty = selectDifficulty();
    Episode episode = selectEpisode();
    
    gameData.setDifficulty(difficulty);
    playEpisode(episode);
}

void GameManager::continueGame() {
    vector<string> saveSlots = SaveSystem::getSaveSlots();
    
    if (saveSlots.empty()) {
        ConsoleUI::showMessage("System", "No saved games found.");
        ConsoleUI::getInput("Press Enter to continue...");
        return;
    }
    
    int choice = ConsoleUI::getChoice("Select save slot:", saveSlots);
    string selectedSlot = saveSlots[choice - 1];
    
    if (SaveSystem::saveExists(selectedSlot)) {
        int currentEpisode = 1;
        if (SaveSystem::loadGame(selectedSlot, gameData, currentEpisode)) {
            ConsoleUI::showMessage("System", "Game loaded successfully!");
            Episode episode = episodeManager.getEpisode(currentEpisode);
            playEpisode(episode);
        } else {
            ConsoleUI::showMessage("System", "Failed to load game.");
        }
    } else {
        ConsoleUI::showMessage("System", "Save file not found.");
    }
    
    ConsoleUI::getInput("Press Enter to continue...");
}

void GameManager::showOptions() {
    ConsoleUI::clearScreen();
    ConsoleUI::showTitle("OPTIONS");
    
    // Convert difficulty to string properly
    string difficultyStr;
    switch (gameData.getDifficulty()) {
        case Difficulty::TRAINING: difficultyStr = "Training"; break;
        case Difficulty::NORMAL: difficultyStr = "Normal"; break;
        case Difficulty::HARD: difficultyStr = "Hard"; break;
        case Difficulty::EXPERT: difficultyStr = "Expert"; break;
    }

    vector<string> options = {
        "Change Difficulty (Current: " + difficultyStr + ")",
        gameData.isTutorialEnabled() ? "Disable Tutorial" : "Enable Tutorial",
        "Back to Main Menu"
    };
    
    int choice = ConsoleUI::getChoice("Options:", options);
    
    switch (choice) {
        case 1: {
            Difficulty newDiff = selectDifficulty();
            gameData.setDifficulty(newDiff);
            ConsoleUI::showMessage("System", "Difficulty changed.");
            break;
        }
        case 2:
            gameData.enableTutorial(!gameData.isTutorialEnabled());
            ConsoleUI::showMessage("System", "Tutorial setting updated.");
            break;
        case 3:
            return;
    }
    
    ConsoleUI::getInput("Press Enter to continue...");
}

void GameManager::showStatistics() {
    ConsoleUI::clearScreen();
    ConsoleUI::showTitle("STATISTICS");
    
    vector<pair<string, string>> stats = {
        {"Total Games Played", to_string(gameData.getTotalGamesPlayed())},
        {"Average Accuracy", to_string(static_cast<int>(gameData.getAverageAccuracy())) + "%"},
        {"Highest Score", to_string(gameData.getHighestScore())},
        {"Skill Points", to_string(gameData.getSkillPoints())},
        {"Current Episode", to_string(gameData.getCurrentEpisode())}
    };
    
    ConsoleUI::showStats(stats);
    
    vector<string> achievements = gameData.getUnlockedAchievements();
    if (!achievements.empty()) {
        cout << "\nACHIEVEMENTS:\n";
        for (const auto& achievement : achievements) {
            cout << "🏆 " << achievement << "\n";
        }
    }
    
    ConsoleUI::getInput("Press Enter to continue...");
}

void GameManager::showHelp() {
    ConsoleUI::showHelp();
}

Difficulty GameManager::selectDifficulty() {
    ConsoleUI::clearScreen();
    ConsoleUI::showTitle("SELECT DIFFICULTY");
    
    vector<string> options = {
        "Training (Low noise, more turns)",
        "Normal (Balanced challenge)", 
        "Hard (High noise, limited turns)",
        "Expert (Extreme conditions)"
    };
    
    int choice = ConsoleUI::getChoice("Choose difficulty level:", options);
    
    return static_cast<Difficulty>(choice - 1);
}

Episode GameManager::selectEpisode() {
    ConsoleUI::clearScreen();
    ConsoleUI::showTitle("SELECT EPISODE");
    
    vector<Episode> availableEpisodes = episodeManager.getAvailableEpisodes(gameData.getSkillPoints());
    
    if (availableEpisodes.empty()) {
        // Fallback to episode 1
        return episodeManager.getEpisode(1);
    }
    
    vector<string> episodeOptions;
    for (const auto& episode : availableEpisodes) {
        string option = "Episode " + to_string(episode.number) + ": " + episode.title;
        if (episode.number > 1) {
            option += " (Requires " + to_string(episode.requiredSkillPoints) + " skill points)";
        }
        episodeOptions.push_back(option);
    }
    
    episodeOptions.push_back("Random Challenge");
    episodeOptions.push_back("Back to Main Menu");
    
    int choice = ConsoleUI::getChoice("Select episode:", episodeOptions);
    
    if (choice <= static_cast<int>(availableEpisodes.size())) {
        return availableEpisodes[choice - 1];
    } else if (choice == static_cast<int>(availableEpisodes.size()) + 1) {
        // Random challenge
        return episodeManager.generateRandomEpisode(gameData.getDifficulty());
    } else {
        // Back to main menu
        showMainMenu();
        return availableEpisodes[0]; // Fallback
    }
}

void GameManager::playEpisode(const Episode& episode) {
    CutsceneManager::showTransmissionEffect();
    
    DispatchGame game(episode.pattern, gameData.getDifficulty());
    game.playEpisode();
    
    // Check for episode unlocks
    if (episode.number == gameData.getCurrentEpisode()) {
        episodeManager.unlockEpisode(episode.number + 1, gameData.getSkillPoints());
    }
}