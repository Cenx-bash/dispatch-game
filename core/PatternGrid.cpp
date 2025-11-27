#include "PatternGrid.h"
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

PatternGrid::PatternGrid(int gridSize) : size(gridSize) {
    grid.resize(size, vector<char>(size, '_'));
}

PatternGrid::PatternGrid(const vector<vector<char>>& initialGrid) {
    grid = initialGrid;
    size = grid.size();
}

char PatternGrid::getCell(int row, int col) const {
    return isValidPosition(row, col) ? grid[row][col] : '_';
}

void PatternGrid::setCell(int row, int col, char value) {
    if (isValidPosition(row, col)) {
        grid[row][col] = value;
    }
}

void PatternGrid::fillRow(int row, char value) {
    if (isValidPosition(row, 0)) {
        for (int col = 0; col < size; col++) {
            grid[row][col] = value;
        }
    }
}

void PatternGrid::fillColumn(int col, char value) {
    if (isValidPosition(0, col)) {
        for (int row = 0; row < size; row++) {
            grid[row][col] = value;
        }
    }
}

void PatternGrid::replaceAll(char oldVal, char newVal) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (grid[row][col] == oldVal) {
                grid[row][col] = newVal;
            }
        }
    }
}

void PatternGrid::clear() {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            grid[row][col] = '_';
        }
    }
}

void PatternGrid::rotate90() {
    vector<vector<char>> newGrid(size, vector<char>(size));
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            newGrid[col][size - 1 - row] = grid[row][col];
        }
    }
    grid = newGrid;
}

void PatternGrid::flipHorizontal() {
    for (int row = 0; row < size; row++) {
        reverse(grid[row].begin(), grid[row].end());
    }
}

void PatternGrid::flipVertical() {
    for (int col = 0; col < size; col++) {
        for (int row = 0; row < size / 2; row++) {
            swap(grid[row][col], grid[size - 1 - row][col]);
        }
    }
}

string PatternGrid::detectPatterns() const {
    vector<string> patterns;
    
    // Check for full rows/columns
    for (int i = 0; i < size; i++) {
        if (hasFullRow(grid[i][0])) patterns.push_back("Full row " + to_string(i+1) + " of " + grid[i][0]);
        
        char firstCol = grid[0][i];
        bool fullCol = true;
        for (int j = 0; j < size; j++) {
            if (grid[j][i] != firstCol) {
                fullCol = false;
                break;
            }
        }
        if (fullCol) patterns.push_back("Full column " + to_string(i+1) + " of " + firstCol);
    }
    
    return patterns.empty() ? "No obvious patterns" : patterns[0];
}

int PatternGrid::countSymbol(char symbol) const {
    int count = 0;
    for (const auto& row : grid) {
        for (char cell : row) {
            if (cell == symbol) count++;
        }
    }
    return count;
}

bool PatternGrid::hasFullRow(char symbol) const {
    for (const auto& row : grid) {
        if (all_of(row.begin(), row.end(), [symbol](char c) { return c == symbol; })) {
            return true;
        }
    }
    return false;
}

bool PatternGrid::hasFullColumn(char symbol) const {
    for (int col = 0; col < size; col++) {
        bool full = true;
        for (int row = 0; row < size; row++) {
            if (grid[row][col] != symbol) {
                full = false;
                break;
            }
        }
        if (full) return true;
    }
    return false;
}

bool PatternGrid::operator==(const PatternGrid& other) const {
    if (size != other.size) return false;
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (grid[row][col] != other.grid[row][col]) return false;
        }
    }
    return true;
}

PatternGrid PatternGrid::getDifference(const PatternGrid& other) const {
    PatternGrid diff(size);
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            diff.setCell(row, col, (grid[row][col] == other.grid[row][col]) ? grid[row][col] : 'X');
        }
    }
    return diff;
}

double PatternGrid::calculateAccuracy(const PatternGrid& other) const {
    if (size != other.size) return 0.0;
    int correct = 0;
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (grid[row][col] == other.grid[row][col]) correct++;
        }
    }
    return (static_cast<double>(correct) / (size * size)) * 100.0;
}

string PatternGrid::toString() const {
    stringstream ss;
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            ss << grid[row][col];
            if (col < size - 1) ss << ' ';
        }
        if (row < size - 1) ss << '\n';
    }
    return ss.str();
}

string PatternGrid::toCompressedString() const {
    string result;
    for (const auto& row : grid) {
        for (char cell : row) {
            result += cell;
        }
    }
    return result;
}

PatternGrid PatternGrid::fromString(const std::string& data) {
    int side = static_cast<int>(sqrt(data.length()));
    PatternGrid grid(side);
    for (int i = 0; i < data.length(); i++) {
        grid.setCell(i / side, i % side, data[i]);
    }
    return grid;
}

bool PatternGrid::isValidPosition(int row, int col) const {
    return row >= 0 && row < size && col >= 0 && col < size;
}