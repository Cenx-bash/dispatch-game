#pragma once
#include <string>
#include <vector>
#include "PatternGrid.h"

struct ParsedCommand {
    enum class Type {
        SET_CELL,
        FILL_ROW,
        FILL_COLUMN,
        REPLACE_ALL,
        CLEAR_GRID,
        INVALID
    };
    
    Type type;
    int row;           // For SET_CELL, FILL_ROW
    int col;           // For SET_CELL, FILL_COLUMN
    char value;        // Value to set
    char oldValue;     // For REPLACE_ALL
    std::string rawCommand;
    
    ParsedCommand() : type(Type::INVALID), row(-1), col(-1), value('_'), oldValue('_') {}
};

class CommandParser {
public:
    static ParsedCommand parse(const std::string& command);
    static bool validateCommand(const ParsedCommand& cmd, const PatternGrid& grid);
    static std::string getCommandHelp();
    
private:
    static ParsedCommand parseSetCommand(const std::string& command);
    static ParsedCommand parseFillRowCommand(const std::string& command);
    static ParsedCommand parseFillColumnCommand(const std::string& command);
    static ParsedCommand parseReplaceCommand(const std::string& command);
    static ParsedCommand parseClearCommand(const std::string& command);
    
    static std::vector<std::string> tokenize(const std::string& command);
    static bool extractCoordinates(const std::string& coordStr, int& row, int& col);
    static char extractValue(const std::string& valueStr);
};