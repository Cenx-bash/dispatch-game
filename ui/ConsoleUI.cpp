#include "ConsoleUI.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <limits>

using namespace std;

bool ConsoleUI::colorsEnabled = true;

void ConsoleUI::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ConsoleUI::showTitle(const std::string& title) {
    cout << "\n" << string(60, '=') << "\n";
    showCentered(title);
    cout << string(60, '=') << "\n\n";
}

void ConsoleUI::showCentered(const std::string& text) {
    int consoleWidth = 80; // Default assumption
    int padding = (consoleWidth - text.length()) / 2;
    
    if (padding > 0) {
        cout << string(padding, ' ');
    }
    cout << text << "\n";
}

void ConsoleUI::showGrid(const std::string& gridDisplay) {
    cout << "\n";
    stringstream ss(gridDisplay);
    string line;
    
    while (getline(ss, line)) {
        cout << "  " << line << "\n";
    }
    cout << "\n";
}

void ConsoleUI::showMessage(const std::string& role, const std::string& message) {
    string formattedRole = role + ":";
    cout << left << setw(12) << formattedRole << " " << message << "\n";
}

void ConsoleUI::slowPrint(const std::string& text, int delayMs) {
    for (char c : text) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
    cout << endl;
}

void ConsoleUI::typewriterEffect(const std::string& text, int delayMs) {
    cout << "> ";
    for (char c : text) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
    cout << "\n\n";
}

void ConsoleUI::progressBar(int current, int total, int width) {
    float progress = static_cast<float>(current) / total;
    int bars = static_cast<int>(progress * width);
    
    cout << "[";
    for (int i = 0; i < width; i++) {
        if (i < bars) cout << "=";
        else cout << " ";
    }
    cout << "] " << int(progress * 100.0) << "%\r";
    cout.flush();
}

std::string ConsoleUI::getInput(const std::string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

int ConsoleUI::getChoice(const std::string& prompt, const std::vector<std::string>& options) {
    cout << prompt << "\n";
    for (size_t i = 0; i < options.size(); i++) {
        cout << "  " << (i + 1) << ". " << options[i] << "\n";
    }
    
    while (true) {
        string input = getInput("Choose (1-" + to_string(options.size()) + "): ");
        try {
            int choice = stoi(input);
            if (choice >= 1 && choice <= static_cast<int>(options.size())) {
                return choice;
            }
        } catch (const exception&) {
            // Invalid input, try again
        }
        cout << "Invalid choice. Please try again.\n";
    }
}

bool ConsoleUI::getConfirmation(const std::string& prompt) {
    string input = getInput(prompt + " (y/n): ");
    return !input.empty() && (input[0] == 'y' || input[0] == 'Y');
}

void ConsoleUI::showStats(const std::vector<std::pair<std::string, std::string>>& stats) {
    cout << "\n" << string(40, '-') << "\n";
    cout << "STATISTICS:\n";
    cout << string(40, '-') << "\n";
    
    for (const auto& stat : stats) {
        cout << left << setw(25) << stat.first << ": " << stat.second << "\n";
    }
    cout << string(40, '-') << "\n";
}

void ConsoleUI::showHelp() {
    clearScreen();
    showTitle("DISPATCH GAME - HELP");
    
    cout << "GAME OVERVIEW:\n";
    cout << "Three roles work together to reconstruct a hidden pattern:\n\n";
    
    cout << colorCyan("DISPATCHER") << " - Sees the target pattern, creates instructions\n";
    cout << colorYellow("MESSENGER") << " - Relays instructions (with errors)\n";
    cout << colorGreen("BUILDER") << " - Executes commands to build the grid\n\n";
    
    cout << "BUILDER COMMANDS:\n";
    cout << "- SET(row,col)=value    (e.g., SET(1,2)=A)\n";
    cout << "- FILL ROW n WITH value (e.g., FILL ROW 3 WITH B)\n";
    cout << "- FILL COLUMN n WITH value (e.g., FILL COLUMN 2 WITH C)\n";
    cout << "- REPLACE ALL x WITH y  (e.g., REPLACE ALL A WITH B)\n";
    cout << "- CLEAR                 (Clear entire grid)\n\n";
    
    cout << "STRATEGIES:\n";
    cout << "- Be specific and clear in descriptions\n";
    cout << "- Use patterns and repetitions to your advantage\n";
    cout << "- Verify important details multiple times\n";
    cout << "- Monitor bandwidth and message limits\n\n";
    
    getInput("Press Enter to continue...");
}

std::string ConsoleUI::colorRed(const std::string& text) {
    return colorsEnabled ? "\033[31m" + text + "\033[0m" : text;
}

std::string ConsoleUI::colorGreen(const std::string& text) {
    return colorsEnabled ? "\033[32m" + text + "\033[0m" : text;
}

std::string ConsoleUI::colorYellow(const std::string& text) {
    return colorsEnabled ? "\033[33m" + text + "\033[0m" : text;
}

std::string ConsoleUI::colorBlue(const std::string& text) {
    return colorsEnabled ? "\033[34m" + text + "\033[0m" : text;
}

std::string ConsoleUI::colorCyan(const std::string& text) {
    return colorsEnabled ? "\033[36m" + text + "\033[0m" : text;
}