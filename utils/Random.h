#pragma once
#include <random>
#include <chrono>

class Random {
private:
    // Remove the static generator declaration from here
    static std::mt19937& getGenerator();
    
public:
    static void initialize();
    static int getInt(int min, int max);
    static double getDouble(double min, double max);
    static bool getBool(double probability = 0.5);
};