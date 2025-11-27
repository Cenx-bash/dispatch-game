#pragma once
#include <string>
#include <vector>
#include <map>
#include <random>
#include "../utils/Random.h"

enum class NoiseLevel {
    LOW = 0,    // 5% noise - Training
    MEDIUM = 1, // 15% noise - Normal  
    HIGH = 2,   // 30% noise - Hard
    EXTREME = 3 // 50% noise - Expert
};

class MessageNoiseSimulator {
public:
    MessageNoiseSimulator(NoiseLevel level = NoiseLevel::MEDIUM);
    
    void setNoiseLevel(NoiseLevel level);
    std::string applyNoise(const std::string& message);
    std::string applyStrategicNoise(const std::string& message, const std::string& context);
    
    // Advanced noise types
    std::string applyMemoryDecay(const std::string& message, int turnDelay);
    std::string applyChannelInterference(const std::string& message);
    std::string applyProtocolLimits(const std::string& message, int maxLength);
    
private:
    double forgetProbability;
    double misinterpretProbability;
    double reorderProbability;
    std::mt19937 rng;
    
    std::map<std::string, std::vector<std::string>> misinterpretations;
    std::map<std::string, std::vector<std::string>> technicalTerms;
    
    void initializeDictionaries();
    std::vector<std::string> splitMessage(const std::string& message);
    std::string joinWords(const std::vector<std::string>& words);
    std::string misinterpretWord(const std::string& word);
    std::string simulateTypo(const std::string& word);
};

class MessageFormatter {
public:
    static std::string formatAsProtocol(const std::string& message, int protocolVersion = 1);
    static std::string compressGridDescription(const std::string& description);
    static std::vector<std::string> splitByBandwidth(const std::string& message, int maxLines, int maxLineLength);
    
    // Protocol versions
    static std::string useProtocolV1(const std::string& message); // Basic
    static std::string useProtocolV2(const std::string& message); // Compressed
    static std::string useProtocolV3(const std::string& message); // Binary-like
};