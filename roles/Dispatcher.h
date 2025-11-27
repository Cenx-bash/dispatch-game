#pragma once
#include "../core/PatternGrid.h"
#include <vector>
#include <string>
#include <memory>

class Dispatcher {
public:
    Dispatcher(const PatternGrid& target);
    
    // Communication strategies
    std::string createInitialDescription();
    std::string createDetailedAnalysis();
    std::string createPatternBasedDescription();
    std::string createCompressedDescription();
    
    // Advanced strategies
    std::string useRowMajorStrategy();
    std::string useColumnMajorStrategy(); 
    std::string useQuadrantStrategy();
    std::string useRunLengthEncoding();
    
    // Interactive
    std::string createCustomDescription(const std::string& playerInput);
    
    // Analysis tools
    std::string getPatternAnalysis() const;
    std::vector<std::string> getHintSuggestions() const;
    
    // Getters
    const PatternGrid& getTargetPattern() const { return targetPattern; }
    const std::vector<std::string>& getSentMessages() const { return sentMessages; }
    int getMessagesSent() const { return sentMessages.size(); }
    
    // Add the missing method
    std::string getTargetDescription() const;
    
    // Strategy scoring
    double evaluateStrategyEffectiveness(const std::string& strategy) const;

private:
    PatternGrid targetPattern;
    std::vector<std::string> sentMessages;
    
    std::string describeByRows();
    std::string describeByColumns();
    std::string describeByQuadrants();
    std::string describeUsingRLE();
    std::string findAndDescribePatterns();
    
    void logMessage(const std::string& message);
};