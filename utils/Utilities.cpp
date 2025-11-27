#include "Utilities.h"
#include <algorithm>
#include <cctype>
#include <thread>
#include <iostream>
#include <chrono>

using namespace std;

vector<string> Utilities::split(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    
    while (getline(ss, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    
    return tokens;
}

string Utilities::join(const vector<string>& tokens, const string& delimiter) {
    string result;
    for (size_t i = 0; i < tokens.size(); i++) {
        result += tokens[i];
        if (i < tokens.size() - 1) {
            result += delimiter;
        }
    }
    return result;
}

string Utilities::trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

string Utilities::toUpper(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

string Utilities::toLower(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

bool Utilities::contains(const string& str, const string& substring) {
    return str.find(substring) != string::npos;
}

string Utilities::replaceAll(string str, const string& from, const string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}

bool Utilities::startsWith(const string& str, const string& prefix) {
    return str.size() >= prefix.size() && 
           str.compare(0, prefix.size(), prefix) == 0;
}

bool Utilities::endsWith(const string& str, const string& suffix) {
    return str.size() >= suffix.size() && 
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

void Utilities::sleep(int milliseconds) {
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

void Utilities::clearConsole() {
    cout << "\033[2J\033[1;1H";
}

int Utilities::getConsoleWidth() {
    return 80; // Simplified implementation
}