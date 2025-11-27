#include "Messenger.h"
#include "../utils/Utilities.h"
#include "../utils/Random.h"
#include <sstream>
#include <algorithm>
#include <random>

using namespace std;

// Define the constant that's declared in the header
const int Messenger::MAX_HISTORY = 10;

Messenger::Messenger(shared_ptr<MessageNoiseSimulator> simulator, int maxLines, bool canAsk)
    : noiseSimulator(simulator), maxLinesPerTurn(maxLines), canAskForRepeat(canAsk),
      currentBandwidth(100), maxBandwidth(100), detailOriented(false), rushed(false), technical(false) {}

string Messenger::processMessage(const string& message) {
    receivedMessages.push_back(message);
    messageHistory.push_back(message);
    
    // Keep history within limits
    if (messageHistory.size() > MAX_HISTORY) {
        messageHistory.erase(messageHistory.begin());
    }
    
    string processed = applyNoiseAndParaphrase(message);
    sentMessages.push_back(processed);
    return processed;
}

string Messenger::processWithContext(const string& message, const string& context) {
    receivedMessages.push_back(message);
    
    string noisyMessage = noiseSimulator->applyStrategicNoise(message, context);
    string paraphrased = paraphraseMessage(noisyMessage);
    string personalityApplied = applyPersonality(paraphrased);
    string limited = enforceLineLimit(personalityApplied);
    
    sentMessages.push_back(limited);
    return limited;
}

string Messenger::applyMemoryRecall(const string& currentMessage, int turnsAgo) {
    // Remove unused parameter warning by marking it as unused
    (void)currentMessage; // This prevents the unused parameter warning
    
    if (turnsAgo <= 0 || turnsAgo > static_cast<int>(messageHistory.size())) {
        return "I don't remember that far back.";
    }
    
    size_t index = messageHistory.size() - turnsAgo;
    string recalledMessage = messageHistory[index];
    
    // Apply memory decay based on how long ago it was
    string decayed = noiseSimulator->applyMemoryDecay(recalledMessage, turnsAgo);
    return "I recall: " + decayed;
}

string Messenger::summarizeMultipleMessages(const vector<string>& messages) {
    if (messages.empty()) return "No messages to summarize.";
    
    stringstream summary;
    summary << "Summary of previous messages: ";
    
    // Simple concatenation with ellipsis for now
    size_t maxMessagesToShow = min(messages.size(), static_cast<size_t>(3));
    for (size_t i = 0; i < maxMessagesToShow; i++) {
        summary << messages[i];
        if (i < maxMessagesToShow - 1) {
            summary << "... ";
        }
    }
    
    if (messages.size() > 3) {
        summary << "... and " << (messages.size() - 3) << " more";
    }
    
    return summary.str();
}

string Messenger::requestClarification(const string& unclearPart) {
    vector<string> clarificationRequests = {
        "Can you repeat the part about " + unclearPart + "?",
        "I didn't catch the " + unclearPart + " clearly.",
        "Could you clarify " + unclearPart + "?",
        "The " + unclearPart + " was unclear, please repeat.",
        "Say again about " + unclearPart + "?"
    };
    
    int index = Random::getInt(0, static_cast<int>(clarificationRequests.size()) - 1);
    return clarificationRequests[index];
}

bool Messenger::canSendMessage(int proposedLength) const {
    // Simple bandwidth calculation
    int bandwidthCost = proposedLength / 10; // 10 chars = 1 bandwidth unit
    return (currentBandwidth - bandwidthCost) >= 0;
}

vector<string> Messenger::chunkMessage(const string& message) {
    return MessageFormatter::splitByBandwidth(message, maxLinesPerTurn, 50);
}

void Messenger::setPersonalityTraits(bool isDetailOriented, bool isRushed, bool isTechnical) {
    detailOriented = isDetailOriented;
    rushed = isRushed;
    technical = isTechnical;
}

string Messenger::applyNoiseAndParaphrase(const string& message) {
    string noisyMessage = noiseSimulator->applyNoise(message);
    string paraphrased = paraphraseMessage(noisyMessage);
    string personalityApplied = applyPersonality(paraphrased);
    return enforceLineLimit(personalityApplied);
}

string Messenger::paraphraseMessage(const string& message) {
    vector<string> sentences;
    stringstream ss(message);
    string sentence;
    
    // Split into sentences
    while (getline(ss, sentence, '.')) {
        sentence = Utilities::trim(sentence);
        if (!sentence.empty()) {
            sentences.push_back(sentence);
        }
    }
    
    if (sentences.empty()) return message;
    
    // Occasionally reorder sentences - create local random generator
    static std::random_device rd;
    static std::mt19937 localGenerator(rd());
    
    if (Random::getDouble(0.0, 1.0) < 0.3 && sentences.size() > 1) {
        shuffle(sentences.begin() + 1, sentences.end(), localGenerator);
    }
    
    // Reconstruct with paraphrasing
    stringstream result;
    for (size_t i = 0; i < sentences.size(); i++) {
        // Simple paraphrasing by replacing common phrases
        string paraphrased = sentences[i];
        
        // Replace common instruction phrases with synonyms
        paraphrased = Utilities::replaceAll(paraphrased, "row", "line");
        paraphrased = Utilities::replaceAll(paraphrased, "column", "vertical");
        paraphrased = Utilities::replaceAll(paraphrased, "grid", "layout");
        paraphrased = Utilities::replaceAll(paraphrased, "pattern", "arrangement");
        
        result << paraphrased;
        if (i < sentences.size() - 1) result << ". ";
    }
    
    return result.str();
}

string Messenger::enforceLineLimit(const string& message) {
    vector<string> lines = chunkMessage(message);
    
    stringstream result;
    size_t maxLines = min(lines.size(), static_cast<size_t>(maxLinesPerTurn));
    for (size_t i = 0; i < maxLines; i++) {
        result << lines[i];
        if (i < maxLines - 1) {
            result << "\n";
        }
    }
    
    return result.str();
}

string Messenger::applyPersonality(const string& message) {
    string result = message;
    
    if (rushed) {
        // Shorten message, add rushed indicators
        if (message.length() > 100) {
            result = message.substr(0, 100) + "... hurry!";
        }
        // Add rushed phrases
        vector<string> rushedPhrases = {"Quick: ", "Fast: ", "Rush: "};
        if (Random::getBool(0.3)) {
            int index = Random::getInt(0, static_cast<int>(rushedPhrases.size()) - 1);
            result = rushedPhrases[index] + result;
        }
    }
    
    if (detailOriented && !rushed) {
        // Add more detail and confirmation
        vector<string> detailPhrases = {"Confirming: ", "Detailed: ", "Noting: "};
        if (Random::getBool(0.4)) {
            int index = Random::getInt(0, static_cast<int>(detailPhrases.size()) - 1);
            result = detailPhrases[index] + result;
        }
    }
    
    if (technical) {
        // Use more technical language
        result = Utilities::replaceAll(result, "line", "row vector");
        result = Utilities::replaceAll(result, "vertical", "column vector");
        result = Utilities::replaceAll(result, "layout", "matrix configuration");
    }
    
    return result;
}

// Getters implementation
// Remove or fix line 20 - it appears to be a comment or misplaced code
// async a message configuration; // This line seems invalid

const std::vector<std::string>& Messenger::getSentMessages() const {
    return sentMessages;
}

const std::vector<std::string>& Messenger::getReceivedMessages() const {
    return receivedMessages;
}

int Messenger::getBandwidthUsed() const {
    return currentBandwidth; // or whatever calculation you need
    // Possible alternatives:
    // return maxBandwidth - currentBandwidth; // if you want remaining bandwidth
    // return bandwidthUsed; // if you have a separate variable tracking usage
}