#include "Dispatcher.h"
#include "../utils/Utilities.h"
#include <sstream>
#include <algorithm>

using namespace std;

Dispatcher::Dispatcher(const PatternGrid& target) : targetPattern(target) {}

string Dispatcher::createInitialDescription() {
    return describeByRows();
}

string Dispatcher::createDetailedAnalysis() {
    stringstream ss;
    ss << describeByRows() << " ";
    ss << findAndDescribePatterns();
    
    string message = ss.str();
    logMessage(message);
    return message;
}

string Dispatcher::createPatternBasedDescription() {
    return findAndDescribePatterns();
}

string Dispatcher::createCompressedDescription() {
    return describeUsingRLE();
}

string Dispatcher::useRowMajorStrategy() {
    return describeByRows();
}

string Dispatcher::useColumnMajorStrategy() {
    return describeByColumns();
}

string Dispatcher::useQuadrantStrategy() {
    return describeByQuadrants();
}

string Dispatcher::useRunLengthEncoding() {
    return describeUsingRLE();
}

string Dispatcher::createCustomDescription(const std::string& playerInput) {
    // For now, just use row major as default for custom input
    string message = "Custom: " + playerInput;
    logMessage(message);
    return message;
}

string Dispatcher::getPatternAnalysis() const {
    return targetPattern.detectPatterns();
}

vector<string> Dispatcher::getHintSuggestions() const {
    vector<string> hints;
    int size = targetPattern.getSize();
    
    // Check for patterns to suggest optimal strategies
    string patterns = targetPattern.detectPatterns();
    if (patterns.find("Full row") != string::npos) {
        hints.push_back("Use FILL ROW commands for complete rows");
    }
    if (patterns.find("Full column") != string::npos) {
        hints.push_back("Use FILL COLUMN commands for complete columns");
    }
    
    // Count symbols to suggest REPLACE commands
    for (char sym = 'A'; sym <= 'D'; sym++) {
        int count = targetPattern.countSymbol(sym);
        if (count > 6) {
            hints.push_back("Many " + string(1, sym) + " symbols - consider using REPLACE if needed");
        }
    }
    
    if (hints.empty()) {
        hints.push_back("Describe rows one by one for maximum clarity");
        hints.push_back("Use SET commands for individual cell corrections");
    }
    
    return hints;
}

double Dispatcher::evaluateStrategyEffectiveness(const std::string& strategy) const {
    // Simple heuristic-based evaluation
    double score = 0.0;
    
    if (strategy.find("row") != string::npos) score += 0.3;
    if (strategy.find("pattern") != string::npos) score += 0.4;
    if (strategy.find("RLE") != string::npos) score += 0.5;
    if (strategy.find("quadrant") != string::npos) score += 0.2;
    if (strategy.find("column") != string::npos) score += 0.1;
    
    // Adjust based on actual grid patterns
    string patterns = targetPattern.detectPatterns();
    if (patterns.find("Full row") != string::npos && strategy.find("row") != string::npos) {
        score += 0.3;
    }
    if (patterns.find("Full column") != string::npos && strategy.find("column") != string::npos) {
        score += 0.3;
    }
    
    return min(1.0, score);
}

string Dispatcher::describeByRows() {
    stringstream ss;
    int size = targetPattern.getSize();
    
    ss << "Grid is " << size << " by " << size << ". ";
    for (int row = 0; row < size; row++) {
        ss << "Row " << (row + 1) << ": ";
        for (int col = 0; col < size; col++) {
            ss << targetPattern.getCell(row, col);
            if (col < size - 1) ss << " ";
        }
        if (row < size - 1) ss << ". ";
    }
    
    string message = ss.str();
    logMessage(message);
    return message;
}

string Dispatcher::describeByColumns() {
    stringstream ss;
    int size = targetPattern.getSize();
    
    ss << "Grid is " << size << " by " << size << ". ";
    for (int col = 0; col < size; col++) {
        ss << "Column " << (col + 1) << ": ";
        for (int row = 0; row < size; row++) {
            ss << targetPattern.getCell(row, col);
            if (row < size - 1) ss << " ";
        }
        if (col < size - 1) ss << ". ";
    }
    
    string message = ss.str();
    logMessage(message);
    return message;
}

string Dispatcher::describeByQuadrants() {
    stringstream ss;
    int size = targetPattern.getSize();
    int half = size / 2;
    
    ss << "Grid divided into quadrants. ";
    ss << "Top-left: ";
    for (int row = 0; row < half; row++) {
        for (int col = 0; col < half; col++) {
            ss << targetPattern.getCell(row, col) << " ";
        }
    }
    
    ss << "Top-right: ";
    for (int row = 0; row < half; row++) {
        for (int col = half; col < size; col++) {
            ss << targetPattern.getCell(row, col) << " ";
        }
    }
    
    ss << "Bottom-left: ";
    for (int row = half; row < size; row++) {
        for (int col = 0; col < half; col++) {
            ss << targetPattern.getCell(row, col) << " ";
        }
    }
    
    ss << "Bottom-right: ";
    for (int row = half; row < size; row++) {
        for (int col = half; col < size; col++) {
            ss << targetPattern.getCell(row, col) << " ";
        }
    }
    
    string message = ss.str();
    logMessage(message);
    return message;
}

string Dispatcher::describeUsingRLE() {
    stringstream ss;
    int size = targetPattern.getSize();
    
    ss << "RLE encoded: ";
    for (int row = 0; row < size; row++) {
        char current = targetPattern.getCell(row, 0);
        int count = 1;
        
        for (int col = 1; col < size; col++) {
            if (targetPattern.getCell(row, col) == current) {
                count++;
            } else {
                ss << current;
                if (count > 1) ss << count;
                current = targetPattern.getCell(row, col);
                count = 1;
            }
        }
        ss << current;
        if (count > 1) ss << count;
        if (row < size - 1) ss << "/";
    }
    
    string message = ss.str();
    logMessage(message);
    return message;
}

string Dispatcher::getTargetDescription() const {
    stringstream ss;
    ss << "TARGET GRID (4×4)\n";
    ss << "Row 1: A B C D\n";
    ss << "Row 2: A A D D\n";
    ss << "Row 3: C B B A\n";
    ss << "Row 4: D C C C";
    return ss.str();
}

string Dispatcher::findAndDescribePatterns() {
    stringstream ss;
    
    ss << "Pattern analysis: " << targetPattern.detectPatterns() << ". ";
    
    // Add symbol counts
    ss << "Symbol counts: ";
    for (char sym = 'A'; sym <= 'D'; sym++) {
        int count = targetPattern.countSymbol(sym);
        if (count > 0) {
            ss << sym << ":" << count << " ";
        }
    }
    
    string message = ss.str();
    logMessage(message);
    return message;
}

void Dispatcher::logMessage(const std::string& message) {
    sentMessages.push_back(message);
}