#include "Builder.h"
#include "../utils/Utilities.h"
#include <sstream>

using namespace std;

Builder::Builder(int gridSize) : currentGrid(gridSize) {}

bool Builder::executeInstruction(const string& instruction) {
    receivedInstructions.push_back(instruction);
    lastError.clear();
    
    ParsedCommand command = CommandParser::parse(instruction);
    
    if (command.type == ParsedCommand::Type::INVALID) {
        lastError = "Invalid command format: " + instruction;
        logAction("ERROR: " + lastError);
        return false;
    }
    
    if (!CommandParser::validateCommand(command, currentGrid)) {
        lastError = "Invalid coordinates or parameters";
        logAction("ERROR: " + lastError);
        return false;
    }
    
    return executeParsedCommand(command);
}

bool Builder::executeParsedCommand(const ParsedCommand& command) {
    commandHistory.push_back(command);
    
    switch(command.type) {
        case ParsedCommand::Type::SET_CELL:
            currentGrid.setCell(command.row, command.col, command.value);
            logAction("SET(" + to_string(command.row + 1) + "," + 
                     to_string(command.col + 1) + ") = " + command.value);
            break;
            
        case ParsedCommand::Type::FILL_ROW:
            currentGrid.fillRow(command.row, command.value);
            logAction("FILL ROW " + to_string(command.row + 1) + " WITH " + command.value);
            break;
            
        case ParsedCommand::Type::FILL_COLUMN:
            currentGrid.fillColumn(command.col, command.value);
            logAction("FILL COLUMN " + to_string(command.col + 1) + " WITH " + command.value);
            break;
            
        case ParsedCommand::Type::REPLACE_ALL:
            currentGrid.replaceAll(command.oldValue, command.value);
            logAction("REPLACE ALL " + string(1, command.oldValue) + 
                     " WITH " + command.value);
            break;
            
        case ParsedCommand::Type::CLEAR_GRID:
            currentGrid.clear();
            logAction("CLEAR GRID");
            break;
            
        default:
            lastError = "Unsupported command type";
            return false;
    }
    
    return true;
}

void Builder::reset() {
    currentGrid.clear();
    receivedInstructions.clear();
    actionLog.clear();
    commandHistory.clear();
    lastError.clear();
}

void Builder::undoLastCommand() {
    if (commandHistory.empty()) {
        lastError = "No commands to undo";
        return;
    }
    
    // For simplicity, just reset and replay all but last command
    vector<ParsedCommand> previousCommands = commandHistory;
    commandHistory.pop_back();
    
    reset();
    
    for (const auto& cmd : commandHistory) {
        executeParsedCommand(cmd);
    }
    
    logAction("UNDO last command");
}

string Builder::getGridDisplay() const {
    return formatGridForDisplay();
}

string Builder::getGridStateDescription() const {
    stringstream ss;
    int size = currentGrid.getSize();
    
    ss << "Current grid state: ";
    for (int row = 0; row < size; row++) {
        ss << "Row " << (row + 1) << ": ";
        for (int col = 0; col < size; col++) {
            ss << currentGrid.getCell(row, col);
            if (col < size - 1) ss << " ";
        }
        if (row < size - 1) ss << " | ";
    }
    
    return ss.str();
}

bool Builder::detectProbableErrors(const PatternGrid& targetHint) {
    // Simple error detection by comparing with target hint
    // In real game, builder doesn't see target, but this is for AI assistance
    int size = currentGrid.getSize();
    int errors = 0;
    
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (currentGrid.getCell(row, col) != targetHint.getCell(row, col)) {
                errors++;
            }
        }
    }
    
    return errors > (size * size) / 4; // More than 25% errors
}

vector<string> Builder::getSuggestedCorrections() const {
    vector<string> suggestions;
    
    // Analyze current grid state and suggest improvements
    int size = currentGrid.getSize();
    
    // Check for incomplete rows
    for (int row = 0; row < size; row++) {
        int emptyCount = 0;
        for (int col = 0; col < size; col++) {
            if (currentGrid.getCell(row, col) == '_') {
                emptyCount++;
            }
        }
        if (emptyCount > 0 && emptyCount < size) {
            suggestions.push_back("Row " + to_string(row + 1) + " has " + 
                                to_string(emptyCount) + " empty cells");
        }
    }
    
    // Check for inconsistent patterns
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size - 1; col++) {
            if (currentGrid.getCell(row, col) != '_' &&
                currentGrid.getCell(row, col) == currentGrid.getCell(row, col + 1)) {
                // Found repeated symbol, might be intentional
            }
        }
    }
    
    if (suggestions.empty()) {
        suggestions.push_back("Grid looks consistent. Continue with current strategy.");
    }
    
    return suggestions;
}

void Builder::logAction(const string& action) {
    actionLog.push_back(action);
}

string Builder::formatGridForDisplay() const {
    stringstream ss;
    int size = currentGrid.getSize();
    
    // Create a visual grid display
    for (int row = 0; row < size; row++) {
        ss << "[ ";
        for (int col = 0; col < size; col++) {
            ss << currentGrid.getCell(row, col);
            if (col < size - 1) ss << " ";
        }
        ss << " ]";
        if (row < size - 1) ss << "\n";
    }
    
    return ss.str();
}