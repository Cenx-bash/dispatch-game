#include "CommandParser.h"
#include "../utils/Utilities.h"
#include <sstream>
#include <algorithm>

using namespace std;

ParsedCommand CommandParser::parse(const string& command) {
    string upperCmd = Utilities::toUpper(Utilities::trim(command));
    
    if (upperCmd.find("SET") != string::npos || upperCmd.find("PUT") != string::npos) {
        return parseSetCommand(upperCmd);
    } else if (upperCmd.find("FILL ROW") != string::npos) {
        return parseFillRowCommand(upperCmd);
    } else if (upperCmd.find("FILL COLUMN") != string::npos || upperCmd.find("FILL COL") != string::npos) {
        return parseFillColumnCommand(upperCmd);
    } else if (upperCmd.find("REPLACE") != string::npos) {
        return parseReplaceCommand(upperCmd);
    } else if (upperCmd.find("CLEAR") != string::npos) {
        return parseClearCommand(upperCmd);
    }
    
    ParsedCommand invalid;
    invalid.rawCommand = command;
    return invalid;
}

ParsedCommand CommandParser::parseSetCommand(const string& command) {
    ParsedCommand cmd;
    cmd.rawCommand = command;
    
    // Look for patterns like: SET(1,2)=A or PUT 1,2 A
    size_t setPos = command.find("SET");
    size_t putPos = command.find("PUT");
    
    if (setPos != string::npos) {
        // SET format: SET(row,col)=value
        size_t parenStart = command.find('(', setPos);
        size_t parenEnd = command.find(')', parenStart);
        size_t eqPos = command.find('=', parenEnd);
        
        if (parenStart != string::npos && parenEnd != string::npos && eqPos != string::npos) {
            string coordStr = command.substr(parenStart + 1, parenEnd - parenStart - 1);
            string valueStr = command.substr(eqPos + 1);
            
            if (extractCoordinates(coordStr, cmd.row, cmd.col)) {
                cmd.value = extractValue(valueStr);
                cmd.type = ParsedCommand::Type::SET_CELL;
            }
        }
    } else if (putPos != string::npos) {
        // PUT format: PUT row,col value
        vector<string> tokens = tokenize(command);
        if (tokens.size() >= 3) {
            if (extractCoordinates(tokens[1], cmd.row, cmd.col)) {
                cmd.value = extractValue(tokens[2]);
                cmd.type = ParsedCommand::Type::SET_CELL;
            }
        }
    }
    
    return cmd;
}

ParsedCommand CommandParser::parseFillRowCommand(const string& command) {
    ParsedCommand cmd;
    cmd.rawCommand = command;
    
    vector<string> tokens = tokenize(command);
    if (tokens.size() >= 5) { // FILL ROW X WITH Y
        auto rowIt = find(tokens.begin(), tokens.end(), "ROW");
        auto withIt = find(tokens.begin(), tokens.end(), "WITH");
        
        if (rowIt != tokens.end() && withIt != tokens.end() && withIt > rowIt) {
            try {
                cmd.row = stoi(*(rowIt + 1)) - 1; // Convert to 0-based
                cmd.value = extractValue(*(withIt + 1));
                cmd.type = ParsedCommand::Type::FILL_ROW;
            } catch (const exception&) {
                // Conversion failed
            }
        }
    }
    
    return cmd;
}

ParsedCommand CommandParser::parseFillColumnCommand(const string& command) {
    ParsedCommand cmd;
    cmd.rawCommand = command;
    
    vector<string> tokens = tokenize(command);
    if (tokens.size() >= 5) { // FILL COLUMN X WITH Y
        auto colIt = find(tokens.begin(), tokens.end(), "COLUMN");
        if (colIt == tokens.end()) colIt = find(tokens.begin(), tokens.end(), "COL");
        
        auto withIt = find(tokens.begin(), tokens.end(), "WITH");
        
        if (colIt != tokens.end() && withIt != tokens.end() && withIt > colIt) {
            try {
                cmd.col = stoi(*(colIt + 1)) - 1; // Convert to 0-based
                cmd.value = extractValue(*(withIt + 1));
                cmd.type = ParsedCommand::Type::FILL_COLUMN;
            } catch (const exception&) {
                // Conversion failed
            }
        }
    }
    
    return cmd;
}

ParsedCommand CommandParser::parseReplaceCommand(const string& command) {
    ParsedCommand cmd;
    cmd.rawCommand = command;
    
    vector<string> tokens = tokenize(command);
    if (tokens.size() >= 6) { // REPLACE ALL X WITH Y
        auto allIt = find(tokens.begin(), tokens.end(), "ALL");
        auto withIt = find(tokens.begin(), tokens.end(), "WITH");
        
        if (allIt != tokens.end() && withIt != tokens.end() && withIt > allIt) {
            cmd.oldValue = extractValue(*(allIt + 1));
            cmd.value = extractValue(*(withIt + 1));
            cmd.type = ParsedCommand::Type::REPLACE_ALL;
        }
    }
    
    return cmd;
}

ParsedCommand CommandParser::parseClearCommand(const string& command) {
    ParsedCommand cmd;
    cmd.rawCommand = command;
    cmd.type = ParsedCommand::Type::CLEAR_GRID;
    return cmd;
}

bool CommandParser::validateCommand(const ParsedCommand& cmd, const PatternGrid& grid) {
    int size = grid.getSize();
    
    switch(cmd.type) {
        case ParsedCommand::Type::SET_CELL:
            return cmd.row >= 0 && cmd.row < size && cmd.col >= 0 && cmd.col < size;
        case ParsedCommand::Type::FILL_ROW:
            return cmd.row >= 0 && cmd.row < size;
        case ParsedCommand::Type::FILL_COLUMN:
            return cmd.col >= 0 && cmd.col < size;
        case ParsedCommand::Type::REPLACE_ALL:
        case ParsedCommand::Type::CLEAR_GRID:
            return true;
        default:
            return false;
    }
}

string CommandParser::getCommandHelp() {
    return R"(
Available Commands:
- SET(row,col)=value    (e.g., SET(1,2)=A)
- FILL ROW n WITH value (e.g., FILL ROW 3 WITH B)
- FILL COLUMN n WITH value (e.g., FILL COLUMN 2 WITH C)
- REPLACE ALL x WITH y  (e.g., REPLACE ALL A WITH B)
- CLEAR                 (Clear the entire grid)

Coordinates are 1-based (row 1-4, column 1-4)
Values: A-Z, 0-9, or _ for empty
)";
}

vector<string> CommandParser::tokenize(const string& command) {
    return Utilities::split(command, ' ');
}

bool CommandParser::extractCoordinates(const string& coordStr, int& row, int& col) {
    string cleanStr = Utilities::replaceAll(coordStr, " ", "");
    size_t commaPos = cleanStr.find(',');
    
    if (commaPos == string::npos) return false;
    
    try {
        row = stoi(cleanStr.substr(0, commaPos)) - 1; // Convert to 0-based
        col = stoi(cleanStr.substr(commaPos + 1)) - 1;
        return true;
    } catch (const exception&) {
        return false;
    }
}

char CommandParser::extractValue(const string& valueStr) {
    if (valueStr.empty()) return '_';
    
    // Take first character and convert to uppercase
    char value = valueStr[0];
    if (isalpha(value)) {
        return toupper(value);
    } else if (isdigit(value) || value == '_') {
        return value;
    } else {
        return '_';
    }
}