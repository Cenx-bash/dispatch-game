#pragma once
#include "../core/MessageSystem.h"
#include <vector>
#include <string>
#include <memory>

class Messenger {
public:
    Messenger(std::shared_ptr<MessageNoiseSimulator> simulator, 
              int maxLines = 2, bool canAsk = true);
    
    // Core message processing
    std::string processMessage(const std::string& message);
    std::string processWithContext(const std::string& message, const std::string& context);
    
    // Advanced features
    std::string applyMemoryRecall(const std::string& currentMessage, int turnsAgo);
    std::string summarizeMultipleMessages(const std::vector<std::string>& messages);
    std::string requestClarification(const std::string& unclearPart);
    
    // Bandwidth management
    bool canSendMessage(int proposedLength) const;
    std::vector<std::string> chunkMessage(const std::string& message);
    
    // State management
    bool canStillAsk() const { return canAskForRepeat; }
    void useRepeatAsk() { canAskForRepeat = false; }
    void resetBandwidth() { currentBandwidth = maxBandwidth; }
    
    // Personality traits (affects message style)
    void setPersonalityTraits(bool isDetailOriented, bool isRushed, bool isTechnical);

    // Getters
    const std::vector<std::string>& getSentMessages() const;
    const std::vector<std::string>& getReceivedMessages() const;
    int getBandwidthUsed() const;
    
private:
    std::shared_ptr<MessageNoiseSimulator> noiseSimulator;
    std::vector<std::string> receivedMessages;
    std::vector<std::string> sentMessages;
    std::vector<std::string> messageHistory;
    
    int maxLinesPerTurn;
    bool canAskForRepeat;
    int currentBandwidth;
    int maxBandwidth;
    
    // Personality traits
    bool detailOriented;
    bool rushed;
    bool technical;
    
    static const int MAX_HISTORY;
    
    std::string applyNoiseAndParaphrase(const std::string& message);
    std::string paraphraseMessage(const std::string& message);
    std::string enforceLineLimit(const std::string& message);
    std::string applyPersonality(const std::string& message);
};