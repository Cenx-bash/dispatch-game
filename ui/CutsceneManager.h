#pragma once
#include <string>
#include <vector>

struct Cutscene {
    std::string title;
    std::vector<std::string> lines;
    int lineDelayMs;
};

class CutsceneManager {
public:
    static void playEpisode1Intro();
    static void playEpisode1Outro(bool success);
    static void playGameOver();
    static void playVictory();
    
    static void showSupervisorMessage(const std::string& message, bool isUrgent = false);
    static void showTransmissionEffect();
    static void showLoadingScreen(const std::string& operation);
    
private:
    static void playCutscene(const Cutscene& cutscene);
    static void typewriterWithEffects(const std::string& text, int delayMs = 50);
    static void pause(int milliseconds);
};