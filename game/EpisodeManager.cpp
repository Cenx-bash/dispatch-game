#include "EpisodeManager.h"
#include "../utils/Random.h"
#include <algorithm>

using namespace std;

EpisodeManager::EpisodeManager() {
    initializeEpisodes();
}

void EpisodeManager::initializeEpisodes() {
    episodes.clear();
    
    // Episode 1 - The First Job
    Episode ep1;
    ep1.number = 1;
    ep1.title = "The First Job";
    ep1.description = "Your introduction to Clearline Dispatch. A simple 4x4 pattern to get you started.";
    ep1.pattern = PatternGrid({
        {'A', 'B', 'C', 'D'},
        {'A', 'A', 'D', 'D'},
        {'C', 'B', 'B', 'A'},
        {'D', 'C', 'C', 'C'}
    });
    ep1.recommendedDifficulty = Difficulty::TRAINING;
    ep1.unlocked = true;
    ep1.requiredSkillPoints = 0;
    episodes.push_back(ep1);
    
    // Episode 2 - Split Channels
    Episode ep2;
    ep2.number = 2;
    ep2.title = "Split Channels";
    ep2.description = "Communication gets trickier with multiple patterns and limited bandwidth.";
    ep2.pattern = PatternGrid({
        {'X', 'X', 'O', 'O'},
        {'X', 'O', 'O', 'X'},
        {'O', 'X', 'X', 'O'},
        {'O', 'O', 'X', 'X'}
    });
    ep2.recommendedDifficulty = Difficulty::NORMAL;
    ep2.unlocked = false;
    ep2.requiredSkillPoints = 10;
    episodes.push_back(ep2);
    
    // Episode 3 - Protocol Upgrade
    Episode ep3;
    ep3.number = 3;
    ep3.title = "Protocol Upgrade";
    ep3.description = "New communication protocols introduce compression but also new challenges.";
    ep3.pattern = PatternGrid({
        {'1', '1', '1', '2'},
        {'3', '3', '2', '2'},
        {'3', '4', '4', '4'},
        {'1', '4', '4', '2'}
    });
    ep3.recommendedDifficulty = Difficulty::HARD;
    ep3.unlocked = false;
    ep3.requiredSkillPoints = 25;
    episodes.push_back(ep3);
}

vector<Episode> EpisodeManager::getAvailableEpisodes(int playerSkillPoints) const {
    vector<Episode> available;
    
    for (const auto& episode : episodes) {
        if (episode.unlocked && episode.requiredSkillPoints <= playerSkillPoints) {
            available.push_back(episode);
        }
    }
    
    return available;
}

Episode EpisodeManager::getEpisode(int episodeNumber) const {
    for (const auto& episode : episodes) {
        if (episode.number == episodeNumber) {
            return episode;
        }
    }
    
    // Return a default episode if not found
    return episodes[0];
}

bool EpisodeManager::unlockEpisode(int episodeNumber, int playerSkillPoints) {
    for (auto& episode : episodes) {
        if (episode.number == episodeNumber && episode.requiredSkillPoints <= playerSkillPoints) {
            episode.unlocked = true;
            return true;
        }
    }
    return false;
}

bool EpisodeManager::isEpisodeUnlocked(int episodeNumber) const {
    for (const auto& episode : episodes) {
        if (episode.number == episodeNumber) {
            return episode.unlocked;
        }
    }
    return false;
}

Episode EpisodeManager::generateRandomEpisode(Difficulty difficulty) {
    Episode randomEpisode;
    randomEpisode.number = 100 + Random::getInt(1, 999);
    randomEpisode.title = "Random Challenge";
    randomEpisode.description = "A randomly generated pattern for endless replayability.";
    randomEpisode.pattern = generatePatternForDifficulty(difficulty);
    randomEpisode.recommendedDifficulty = difficulty;
    randomEpisode.unlocked = true;
    randomEpisode.requiredSkillPoints = 0;
    
    return randomEpisode;
}

Episode EpisodeManager::createCustomEpisode(const string& title, const PatternGrid& pattern) {
    Episode customEpisode;
    customEpisode.number = 1000 + Random::getInt(1, 9999);
    customEpisode.title = title;
    customEpisode.description = "Custom created pattern.";
    customEpisode.pattern = pattern;
    customEpisode.recommendedDifficulty = Difficulty::NORMAL;
    customEpisode.unlocked = true;
    customEpisode.requiredSkillPoints = 0;
    
    return customEpisode;
}

PatternGrid EpisodeManager::generatePatternForDifficulty(Difficulty difficulty) {
    int size = 4;
    PatternGrid grid(size);
    
    string symbols;
    switch (difficulty) {
        case Difficulty::TRAINING:
            symbols = "AB";
            break;
        case Difficulty::NORMAL:
            symbols = "ABCD";
            break;
        case Difficulty::HARD:
            symbols = "ABCD123";
            break;
        case Difficulty::EXPERT:
            symbols = "ABCDEFGH123456";
            break;
    }
    
    // Fill grid with random symbols
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            char symbol = symbols[Random::getInt(0, symbols.length() - 1)];
            grid.setCell(row, col, symbol);
        }
    }
    
    return grid;
}