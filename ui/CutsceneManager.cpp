#include "CutsceneManager.h"
#include "ConsoleUI.h"
#include "../utils/Utilities.h"
#include <iostream>
#include <thread>

using namespace std;

void CutsceneManager::playEpisode1Intro() {
    ConsoleUI::clearScreen();
    
    Cutscene intro;
    intro.title = "EPISODE 1 - THE FIRST JOB";
    intro.lineDelayMs = 60;
    intro.lines = {
        "🌆 INTRO CUTSCENE (TEXT)",
        "",
        "You arrive at Clearline Dispatch, a small logistics office",
        "with flickering lights, humming servers, and stacks of",
        "half-finished paperwork.",
        "",
        "A tired supervisor looks up from her desk.",
        "",
        "Supervisor Mara: \"You must be the new Dispatcher.",
        "We're short-staffed, so you're starting right now.",
        "Your first task is simple: guide our warehouse team",
        "through a basic layout job.\"",
        "",
        "She hands you a clipboard with a 4×4 layout pattern",
        "printed on faded paper.",
        "",
        "Mara: \"You'll give instructions to the Messenger.",
        "The Messenger will relay it to the Builder.",
        "The Builder will recreate the pattern.",
        "None of you can see each other's info.",
        "If the reconstruction is correct, you pass your first day.\"",
        "",
        "She leans back:",
        "",
        "Mara: \"Welcome to dispatch.\"",
        "",
        "Fade to black..."
    };
    
    playCutscene(intro);
}

void CutsceneManager::playEpisode1Outro(bool success) {
    ConsoleUI::clearScreen();
    
    Cutscene outro;
    outro.title = "MISSION COMPLETE";
    outro.lineDelayMs = 50;
    
    if (success) {
        outro.lines = {
            "Supervisor Mara reviews the final layout...",
            "",
            "Mara: \"Not bad, Dispatcher.",
            "You handled your first transmission cleanly.",
            "The team built the layout perfectly.",
            "Let's see if you survive the next shift.\"",
            "",
            "✅ EPISODE COMPLETE",
            "",
            "You've passed your first day at Clearline Dispatch."
        };
    } else {
        outro.lines = {
            "Supervisor Mara reviews the final layout...",
            "",
            "Mara: \"Close, but not enough.",
            "Breakdown was either in your instructions or",
            "the Messenger's relay.",
            "Review your communication.",
            "Precision saves time — and money.\"",
            "",
            "❌ PATTERN MISMATCH",
            "",
            "The team will need to try again tomorrow."
        };
    }
    
    playCutscene(outro);
}

void CutsceneManager::playGameOver() {
    ConsoleUI::clearScreen();
    
    Cutscene gameOver;
    gameOver.title = "TRANSMISSION FAILED";
    gameOver.lineDelayMs = 70;
    gameOver.lines = {
        "The communication channel has degraded beyond recovery.",
        "",
        "Static fills your headset...",
        "",
        "Mara: \"We're losing them! The connection is—\"",
        "",
        "[SIGNAL LOST]",
        "",
        "The dispatch terminal goes dark.",
        "",
        "Your first assignment ends in failure.",
        "Better luck next time, Dispatcher."
    };
    
    playCutscene(gameOver);
}

void CutsceneManager::playVictory() {
    ConsoleUI::clearScreen();
    
    Cutscene victory;
    victory.title = "MISSION ACCOMPLISHED";
    victory.lineDelayMs = 40;
    victory.lines = {
        "🎉 TRANSMISSION SUCCESSFUL!",
        "",
        "The pattern matches perfectly.",
        "Warehouse reports layout construction complete.",
        "",
        "Mara: \"Outstanding work, Dispatcher!",
        "Your clear communication under pressure",
        "saved this operation.",
        "",
        "The company is promoting you to Senior Dispatcher.",
        "You'll be handling more complex patterns",
        "and higher-stakes missions from now on.\"",
        "",
        "Congratulations! You've mastered the art of",
        "clear communication under constraints."
    };
    
    playCutscene(victory);
}

void CutsceneManager::showSupervisorMessage(const string& message, bool isUrgent) {
    ConsoleUI::clearScreen();
    
    if (isUrgent) {
        ConsoleUI::showTitle("🚨 URGENT MESSAGE FROM SUPERVISOR");
    } else {
        ConsoleUI::showTitle("📞 SUPERVISOR UPDATE");
    }
    
    ConsoleUI::typewriterEffect("Mara: \"" + message + "\"", 40);
    
    Utilities::sleep(2000);
}

void CutsceneManager::showTransmissionEffect() {
    cout << "\n";
    ConsoleUI::slowPrint("📡 ESTABLISHING TRANSMISSION...", 30);
    
    for (int i = 0; i < 3; i++) {
        cout << "🔊 " << string(20 + i * 5, '.') << " CONNECTED" << endl;
        Utilities::sleep(500);
    }
    
    cout << "\n";
    ConsoleUI::slowPrint("✅ CHANNEL SECURE - BEGIN TRANSMISSION", 20);
    cout << "\n";
}

void CutsceneManager::showLoadingScreen(const string& operation) {
    ConsoleUI::clearScreen();
    ConsoleUI::showTitle("SYSTEM PROCESSING");
    
    cout << "\n" << operation << "\n\n";
    
    for (int i = 0; i <= 10; i++) {
        ConsoleUI::progressBar(i, 10, 30);
        Utilities::sleep(100);
    }
    
    cout << "\n\n";
}

void CutsceneManager::playCutscene(const Cutscene& cutscene) {
    ConsoleUI::clearScreen();
    ConsoleUI::showTitle(cutscene.title);
    
    for (const auto& line : cutscene.lines) {
        if (line.empty()) {
            cout << "\n";
            Utilities::sleep(cutscene.lineDelayMs * 3);
        } else {
            typewriterWithEffects(line, cutscene.lineDelayMs);
        }
    }
    
    cout << "\n";
    Utilities::sleep(2000);
}

void CutsceneManager::typewriterWithEffects(const string& text, int delayMs) {
    // Add dramatic effects for supervisor lines
    if (text.find("Mara:") != string::npos) {
        cout << ConsoleUI::colorCyan("> ");
        delayMs = 40; // Slower for dramatic effect
    }
    
    ConsoleUI::slowPrint(text, delayMs);
}

void CutsceneManager::pause(int milliseconds) {
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}