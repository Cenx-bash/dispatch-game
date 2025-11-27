#pragma once
#include <string>
#include <vector>
#include <sstream>

class Utilities {
public:
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::string join(const std::vector<std::string>& tokens, const std::string& delimiter);
    static std::string trim(const std::string& str);
    static std::string toUpper(const std::string& str);
    static std::string toLower(const std::string& str);
    static bool contains(const std::string& str, const std::string& substring);
    static std::string replaceAll(std::string str, const std::string& from, const std::string& to);
    static bool startsWith(const std::string& str, const std::string& prefix);
    static bool endsWith(const std::string& str, const std::string& suffix);
    
    template<typename T>
    static std::string toString(const T& value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }
    
    static void sleep(int milliseconds);
    static void clearConsole();
    static int getConsoleWidth();
};