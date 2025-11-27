#include "game/GameManager.h"
#include <iostream>

using namespace std;

int main() {
    try {
        GameManager game;
        game.run();
    } catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }
    return 0;
}