#pragma once
#include <vector>
#include <string>
#include <memory>

class PatternGrid {
public:
    PatternGrid(int size = 4);
    PatternGrid(const std::vector<std::vector<char>>& initialGrid);
    
    // Core operations
    char getCell(int row, int col) const;
    void setCell(int row, int col, char value);
    void fillRow(int row, char value);
    void fillColumn(int col, char value);
    void replaceAll(char oldVal, char newVal);
    void clear();
    
    // Pattern operations
    void rotate90();
    void flipHorizontal();
    void flipVertical();
    
    // Analysis
    std::string detectPatterns() const;
    int countSymbol(char symbol) const;
    bool hasFullRow(char symbol) const;
    bool hasFullColumn(char symbol) const;
    
    // Comparison
    bool operator==(const PatternGrid& other) const;
    PatternGrid getDifference(const PatternGrid& other) const;
    double calculateAccuracy(const PatternGrid& other) const;
    
    // Serialization
    std::string toString() const;
    std::string toCompressedString() const;
    static PatternGrid fromString(const std::string& data);
    
    int getSize() const { return size; }
    
private:
    std::vector<std::vector<char>> grid;
    int size;
    
    bool isValidPosition(int row, int col) const;
};