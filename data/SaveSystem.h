#pragma once
#include "GameData.h"
#include <string>
#include <vector>

class SaveSystem {
public:
    static bool saveGame(const std::string& slotName, const GameData& gameData, int currentEpisode);
    static bool loadGame(const std::string& slotName, GameData& gameData, int& currentEpisode);
    static std::vector<std::string> getSaveSlots();
    static bool deleteSave(const std::string& slotName);
    static bool saveExists(const std::string& slotName);
    
private:
    static std::string getSaveFilePath(const std::string& slotName);
    static std::string sanitizeFilename(const std::string& name);
};