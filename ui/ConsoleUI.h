#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

class ConsoleUI {
public:
    static void clearScreen();
    static void showTitle(const std::string& title);
    static void showCentered(const std::string& text);
    static void showGrid(const std::string& gridDisplay);
    static void showMessage(const std::string& role, const std::string& message);
    
    static void slowPrint(const std::string& text, int delayMs = 30);
    static void typewriterEffect(const std::string& text, int delayMs = 50);
    static void progressBar(int current, int total, int width = 50);
    
    static std::string getInput(const std::string& prompt);
    static int getChoice(const std::string& prompt, const std::vector<std::string>& options);
    static bool getConfirmation(const std::string& prompt);
    
    static void showStats(const std::vector<std::pair<std::string, std::string>>& stats);
    static void showHelp();
    
    // Color support (if terminal supports it)
    static std::string colorRed(const std::string& text);
    static std::string colorGreen(const std::string& text);
    static std::string colorYellow(const std::string& text);
    static std::string colorBlue(const std::string& text);
    static std::string colorCyan(const std::string& text);
    
private:
    static bool colorsEnabled;
};