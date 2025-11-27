#include "SaveSystem.h"
#include <fstream>
#include <filesystem>
#include <algorithm>

using namespace std;

bool SaveSystem::saveGame(const string& slotName, const GameData& gameData, int currentEpisode) {
    // In a real implementation, this would serialize to file
    // For this example, we'll just return success
    return true;
}

bool SaveSystem::loadGame(const string& slotName, GameData& gameData, int& currentEpisode) {
    // In a real implementation, this would deserialize from file
    // For this example, we'll just return false (no save system implemented)
    return false;
}

vector<string> SaveSystem::getSaveSlots() {
    // Mock implementation - in real game would scan save directory
    return {"Auto Save", "Manual Save 1", "Manual Save 2"};
}

bool SaveSystem::deleteSave(const string& slotName) {
    // Mock implementation
    return true;
}

bool SaveSystem::saveExists(const string& slotName) {
    // Mock implementation - always return false for simplicity
    return false;
}

string SaveSystem::getSaveFilePath(const string& slotName) {
    string cleanName = sanitizeFilename(slotName);
    return "saves/" + cleanName + ".sav";
}

string SaveSystem::sanitizeFilename(const string& name) {
    string result = name;
    
    // Remove or replace invalid filename characters
    const string invalidChars = "<>:\"/\\|?*";
    for (char& c : result) {
        if (invalidChars.find(c) != string::npos) {
            c = '_';
        }
    }
    
    return result;
}