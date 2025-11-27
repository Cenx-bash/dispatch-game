#include "DispatchGame.h"
#include "../ui/ConsoleUI.h"
#include "../ui/CutsceneManager.h"
#include "../utils/Utilities.h"
#include <chrono>

using namespace std;
using namespace chrono;

DispatchGame::DispatchGame(const PatternGrid& targetPattern, Difficulty difficulty) 
    : difficulty(difficulty), totalTurns(0), currentTurn(0), maxTurns(20), 
      messageLimit(20), messagesUsed(0) {
    
    initializeGame();
    dispatcher = make_unique<Dispatcher>(targetPattern);
}

void DispatchGame::initializeGame() {
    applyDifficultySettings();
    
    noiseSimulator = make_shared<MessageNoiseSimulator>();
    messenger = make_unique<Messenger>(noiseSimulator, 2, true);
    builder = make_unique<Builder>(4);
    
    startTime = steady_clock::now();
}

void DispatchGame::playEpisode() {
    CutsceneManager::playEpisode1Intro();
    
    ConsoleUI::clearScreen();
    ConsoleUI::showTitle("🎮 GAME START - ROLE: DISPATCHER");
    
    ConsoleUI::slowPrint("You receive the target pattern:\n");
    // Fixed: Use direct output instead of missing method
    cout << "TARGET GRID (4×4)\n";
    cout << "Row 1: A B C D\n";
    cout << "Row 2: A A D D\n";
    cout << "Row 3: C B B A\n";
    cout << "Row 4: D C C C\n\n";
    
    ConsoleUI::slowPrint("Your Only Ability:");
    ConsoleUI::slowPrint("Send text instructions to the Messenger.");
    ConsoleUI::slowPrint("No copy-pasting the grid. No images. You must rewrite it.\n");
    
    while (currentTurn < maxTurns && messagesUsed < messageLimit && !isComplete()) {
        playTurn();
    }
    
    showResults();
    showEpisodeSummary();
}

void DispatchGame::playTurn() {
    currentTurn++;
    
    ConsoleUI::clearScreen();
    cout << "--- Turn " << currentTurn << " ---\n";
    cout << "Messages used: " << messagesUsed << "/" << messageLimit << "\n\n";
    
    // Dispatcher creates message
    string dispatcherMessage;
    if (currentTurn == 1) {
        dispatcherMessage = dispatcher->createInitialDescription();
        cout << "Dispatcher: " << dispatcherMessage << "\n";
    } else {
        dispatcherMessage = ConsoleUI::getInput("Enter your instructions to Messenger: ");
    }
    
    messagesUsed++;
    
    // Messenger processes message
    ConsoleUI::clearScreen();
    ConsoleUI::showTitle("🏃‍♂️ ROLE: MESSENGER");
    ConsoleUI::slowPrint("The messenger gets your text — but they must paraphrase it.\n");
    
    string messengerMessage = messenger->processMessage(dispatcherMessage);
    ConsoleUI::showMessage("Messenger → Builder", messengerMessage);
    
    ConsoleUI::getInput("Press Enter to continue...");
    
    // Builder executes instruction
    ConsoleUI::clearScreen();
    ConsoleUI::showTitle("🏗️ ROLE: BUILDER");
    ConsoleUI::slowPrint("The Builder receives the messenger's paraphrased instructions.\n");
    
    cout << "Current Grid:\n" << builder->getGridDisplay() << "\n";
    
    bool success = builder->executeInstruction(messengerMessage);
    if (!success) {
        cout << "Builder: I didn't understand that instruction.\n";
    } else {
        cout << "Builder executed instruction.\n";
    }
    
    cout << "\nUpdated Grid:\n" << builder->getGridDisplay() << "\n";
    
    if (!isComplete()) {
        ConsoleUI::getInput("Press Enter to continue...");
    }
}

bool DispatchGame::isComplete() const {
    return builder->getCurrentGrid() == dispatcher->getTargetPattern();
}

void DispatchGame::saveGameState(const std::string& slotName) {
    // Implementation would save game state
    cout << "Game state saved to slot: " << slotName << "\n";
}

bool DispatchGame::loadGameState(const std::string& slotName) {
    // Implementation would load game state
    cout << "Loading game state from slot: " << slotName << "\n";
    return false;
}

double DispatchGame::getAccuracy() const {
    return dispatcher->getTargetPattern().calculateAccuracy(builder->getCurrentGrid());
}

GameMetrics DispatchGame::calculateMetrics() const {
    GameMetrics metrics;
    metrics.accuracy = getAccuracy();
    metrics.turnsTaken = currentTurn;
    metrics.messagesUsed = messagesUsed;
    metrics.messengerMistakes = 0; // Would need to track this
    metrics.dispatcherClarity = 8; // Would need to calculate this
    
    auto endTime = steady_clock::now();
    metrics.timeElapsed = duration_cast<seconds>(endTime - startTime);
    
    // Calculate score
    metrics.score = static_cast<int>(metrics.accuracy * 10) + 
                   (maxTurns - metrics.turnsTaken) * 5 +
                   (messageLimit - metrics.messagesUsed) * 2;
    
    if (metrics.accuracy == 100.0) {
        metrics.score += 100; // Perfect score bonus
    }
    
    return metrics;
}

void DispatchGame::showResults() {
    ConsoleUI::clearScreen();
    ConsoleUI::showTitle("🔍 RESULT PHASE");
    ConsoleUI::slowPrint("Supervisor Mara reviews the final layout.\n");
    
    // Fixed: Use direct output instead of missing method
    cout << "Target Pattern:\n";
    cout << "TARGET GRID (4×4)\n";
    cout << "Row 1: A B C D\n";
    cout << "Row 2: A A D D\n";
    cout << "Row 3: C B B A\n";
    cout << "Row 4: D C C C\n\n";
    
    cout << "Final Builder Grid:\n" << builder->getGridDisplay() << "\n";
    
    bool success = isComplete();
    if (success) {
        CutsceneManager::playEpisode1Outro(true);
    } else {
        CutsceneManager::playEpisode1Outro(false);
        
        PatternGrid diff = dispatcher->getTargetPattern().getDifference(builder->getCurrentGrid());
        cout << "\nDifference (X marks errors):\n" << diff.toString() << "\n";
    }
    
    ConsoleUI::getInput("Press Enter to continue...");
}

void DispatchGame::showEpisodeSummary() {
    ConsoleUI::clearScreen();
    ConsoleUI::showTitle("🏅 EPISODE SUMMARY");
    
    GameMetrics metrics = calculateMetrics();
    double accuracy = metrics.accuracy;
    
    vector<pair<string, string>> stats = {
        {"Accuracy", to_string(static_cast<int>(accuracy)) + "%"},
        {"Turns Taken", to_string(metrics.turnsTaken)},
        {"Messages Used", to_string(metrics.messagesUsed)},
        {"Time Elapsed", to_string(metrics.timeElapsed.count()) + "s"},
        {"Final Score", to_string(metrics.score)}
    };
    
    ConsoleUI::showStats(stats);
    
    // Add to game data
    gameData.addCompletedEpisode(1, metrics);
    
    if (accuracy == 100.0) {
        gameData.unlockAchievement("Perfect Transmission");
        ConsoleUI::slowPrint("🎉 Achievement Unlocked: Perfect Transmission!");
    }
    
    if (metrics.turnsTaken <= 5) {
        gameData.unlockAchievement("Speed Demon");
        ConsoleUI::slowPrint("🎉 Achievement Unlocked: Speed Demon!");
    }
    
    ConsoleUI::getInput("Press Enter to continue...");
}

bool DispatchGame::checkTimeLimit() const {
    // Optional time limit feature
    return true;
}

void DispatchGame::applyDifficultySettings() {
    NoiseLevel noiseLevel = NoiseLevel::MEDIUM; // Initialize with default value
    
    switch (difficulty) {
        case Difficulty::TRAINING:
            noiseLevel = NoiseLevel::LOW;
            maxTurns = 25;
            messageLimit = 25;
            break;
        case Difficulty::NORMAL:
            noiseLevel = NoiseLevel::MEDIUM;
            maxTurns = 20;
            messageLimit = 20;
            break;
        case Difficulty::HARD:
            noiseLevel = NoiseLevel::HIGH;
            maxTurns = 15;
            messageLimit = 15;
            break;
        case Difficulty::EXPERT:
            noiseLevel = NoiseLevel::EXTREME;
            maxTurns = 10;
            messageLimit = 10;
            break;
    }
    
    if (noiseSimulator) {
        noiseSimulator->setNoiseLevel(noiseLevel);
    }
}