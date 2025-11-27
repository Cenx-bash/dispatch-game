#pragma once
#include "../core/PatternGrid.h"
#include "../data/GameData.h"
#include <vector>
#include <string>

struct Episode {
    int number;
    std::string title;
    std::string description;
    PatternGrid pattern;
    Difficulty recommendedDifficulty;
    bool unlocked;
    int requiredSkillPoints;
};

class EpisodeManager {
public:
    EpisodeManager();
    
    std::vector<Episode> getAvailableEpisodes(int playerSkillPoints) const;
    Episode getEpisode(int episodeNumber) const;
    bool unlockEpisode(int episodeNumber, int playerSkillPoints);
    bool isEpisodeUnlocked(int episodeNumber) const;
    
    Episode generateRandomEpisode(Difficulty difficulty);
    Episode createCustomEpisode(const std::string& title, const PatternGrid& pattern);

private:
    std::vector<Episode> episodes;
    
    void initializeEpisodes();
    PatternGrid generatePatternForDifficulty(Difficulty difficulty);
};