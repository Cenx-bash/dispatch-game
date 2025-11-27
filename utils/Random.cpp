#include "Random.h"

// Define the generator locally in the .cpp file
namespace {
    std::mt19937& getLocalGenerator() {
        static std::mt19937 generator(std::chrono::steady_clock::now().time_since_epoch().count());
        return generator;
    }
}

void Random::initialize() {
    // No need to do anything since it's initialized statically
}

int Random::getInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(getLocalGenerator());
}

double Random::getDouble(double min, double max) {
    std::uniform_real_distribution<double> dist(min, max);
    return dist(getLocalGenerator());
}

bool Random::getBool(double probability) {
    std::bernoulli_distribution dist(probability);
    return dist(getLocalGenerator());
}