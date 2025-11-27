#pragma once
#include "../core/PatternGrid.h"
#include "../core/CommandParser.h"
#include <vector>
#include <string>

class Builder {
public:
    Builder(int gridSize = 4);
    
    // Core operations
    bool executeInstruction(const std::string& instruction);
    bool executeParsedCommand(const ParsedCommand& command);
    
    // State management
    void reset();
    void undoLastCommand();
    
    // Information
    const PatternGrid& getCurrentGrid() const { return currentGrid; }
    std::string getGridDisplay() const;
    std::string getGridStateDescription() const;
    
    // History and feedback
    const std::vector<std::string>& getActionLog() const { return actionLog; }
    const std::vector<std::string>& getReceivedInstructions() const { return receivedInstructions; }
    std::string getLastError() const { return lastError; }
    
    // Intelligence
    bool detectProbableErrors(const PatternGrid& targetHint);
    std::vector<std::string> getSuggestedCorrections() const;

private:
    PatternGrid currentGrid;
    std::vector<std::string> receivedInstructions;
    std::vector<std::string> actionLog;
    std::vector<ParsedCommand> commandHistory;
    std::string lastError;
    
    void logAction(const std::string& action);
    std::string formatGridForDisplay() const;
};