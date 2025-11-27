#include "MessageSystem.h"
#include <sstream>
#include <algorithm>

using namespace std;

MessageNoiseSimulator::MessageNoiseSimulator(NoiseLevel level) {
    random_device rd;
    rng.seed(rd());
    setNoiseLevel(level);
    initializeDictionaries();
}

void MessageNoiseSimulator::setNoiseLevel(NoiseLevel level) {
    switch(level) {
        case NoiseLevel::LOW:
            forgetProbability = 0.05;
            misinterpretProbability = 0.08;
            reorderProbability = 0.02;
            break;
        case NoiseLevel::MEDIUM:
            forgetProbability = 0.15;
            misinterpretProbability = 0.20;
            reorderProbability = 0.10;
            break;
        case NoiseLevel::HIGH:
            forgetProbability = 0.25;
            misinterpretProbability = 0.35;
            reorderProbability = 0.20;
            break;
        case NoiseLevel::EXTREME:
            forgetProbability = 0.40;
            misinterpretProbability = 0.50;
            reorderProbability = 0.30;
            break;
    }
}

void MessageNoiseSimulator::initializeDictionaries() {
    misinterpretations = {
        {"A", {"B", "eight", "hey", "8", "K"}},
        {"B", {"D", "P", "three", "13", "R"}},
        {"C", {"see", "sea", "G", "sea", "Z"}},
        {"D", {"B", "the", "P", "0", "O"}},
        {"ROW", {"COLUMN", "LINE", "ARRAY", "SEQUENCE", "TIER"}},
        {"COLUMN", {"ROW", "COLLUM", "COMLUMN", "VERTICAL", "PILE"}},
        {"FILL", {"FULL", "FEEL", "PUT", "PLACE", "LOAD"}},
        {"SET", {"PUT", "SAT", "LET", "PLACE", "ASSIGN"}},
        {"REPLACE", {"SUBSTITUTE", "REPLAY", "DISPLACE", "SWAP", "EXCHANGE"}},
        {"ALL", {"WHOLE", "EACH", "EVERY", "ENTIRE", "COMPLETE"}},
        {"FIRST", {"ONE", "1ST", "BEGINNING", "INITIAL", "PRIMARY"}},
        {"SECOND", {"TWO", "2ND", "NEXT", "SECONDARY", "ANOTHER"}},
        {"THIRD", {"THREE", "3RD", "TERTIARY", "ANOTHER", "FINAL"}},
        {"FOURTH", {"FOUR", "4TH", "LAST", "FINAL", "ULTIMATE"}},
        {"WITH", {"USING", "VIA", "BY", "THROUGH", "EMPLOYING"}}
    };
    
    technicalTerms = {
        {"ROW", {"LINE", "SEQUENCE", "TIER"}},
        {"COLUMN", {"VERTICAL", "PILE", "STACK"}},
        {"GRID", {"MATRIX", "ARRAY", "TABLE"}},
        {"PATTERN", {"DESIGN", "ARRANGEMENT", "SEQUENCE"}},
        {"SYMBOL", {"CHARACTER", "MARK", "LETTER"}}
    };
}

string MessageNoiseSimulator::applyNoise(const string& message) {
    vector<string> words = splitMessage(message);
    vector<string> noisyWords;
    
    for (const auto& word : words) {
        double roll = Random::getDouble(0.0, 1.0);
        
        if (roll < forgetProbability) {
            continue; // Word forgotten
        } else if (roll < forgetProbability + misinterpretProbability) {
            noisyWords.push_back(misinterpretWord(word));
        } else if (roll < forgetProbability + misinterpretProbability + reorderProbability) {
            noisyWords.push_back(simulateTypo(word));
        } else {
            noisyWords.push_back(word);
        }
    }
    
    // Occasionally reorder the entire sentence
    if (Random::getDouble(0.0, 1.0) < reorderProbability && noisyWords.size() > 3) {
        shuffle(noisyWords.begin() + 1, noisyWords.end() - 1, rng);
    }
    
    return joinWords(noisyWords);
}

string MessageNoiseSimulator::applyStrategicNoise(const string& message, const string& context) {
    string noisyMessage = applyNoise(message);
    
    // Strategic noise based on context
    if (context.find("urgent") != string::npos) {
        // Urgent messages get rushed and sloppy
        if (Random::getDouble(0.0, 1.0) < 0.3) {
            noisyMessage = noisyMessage.substr(0, noisyMessage.length() / 2) + "...";
        }
    }
    
    if (context.find("technical") != string::npos) {
        // Technical terms get replaced with simpler words
        for (const auto& [tech, simple] : technicalTerms) {
            if (noisyMessage.find(tech) != string::npos && !simple.empty()) {
                size_t pos = noisyMessage.find(tech);
                if (pos != string::npos) {
                    noisyMessage.replace(pos, tech.length(), 
                        simple[Random::getInt(0, simple.size() - 1)]);
                }
            }
        }
    }
    
    return noisyMessage;
}

string MessageNoiseSimulator::applyMemoryDecay(const string& message, int turnDelay) {
    double decayFactor = min(0.8, turnDelay * 0.2);
    vector<string> words = splitMessage(message);
    vector<string> decayedWords;
    
    for (const auto& word : words) {
        if (Random::getDouble(0.0, 1.0) > decayFactor) {
            decayedWords.push_back(word);
        }
    }
    
    return decayedWords.empty() ? "I forgot..." : joinWords(decayedWords);
}

string MessageNoiseSimulator::applyChannelInterference(const string& message) {
    string result = message;
    
    // Simulate static interference
    if (Random::getDouble(0.0, 1.0) < 0.2) {
        static const string staticChars = "#%&*@$!~";
        int insertPos = Random::getInt(0, result.length() - 1);
        result.insert(insertPos, 1, staticChars[Random::getInt(0, staticChars.length() - 1)]);
    }
    
    // Simulate cutouts
    if (Random::getDouble(0.0, 1.0) < 0.15) {
        result = result.substr(0, result.length() / 2) + "---";
    }
    
    return result;
}

string MessageNoiseSimulator::applyProtocolLimits(const string& message, int maxLength) {
    if (message.length() <= maxLength) return message;
    
    string truncated = message.substr(0, maxLength - 3) + "...";
    return truncated;
}

vector<string> MessageNoiseSimulator::splitMessage(const string& message) {
    vector<string> words;
    istringstream stream(message);
    string word;
    
    while (stream >> word) {
        words.push_back(word);
    }
    return words;
}

string MessageNoiseSimulator::joinWords(const vector<string>& words) {
    string result;
    for (size_t i = 0; i < words.size(); i++) {
        result += words[i];
        if (i < words.size() - 1) result += ' ';
    }
    return result;
}

string MessageNoiseSimulator::misinterpretWord(const string& word) {
    string upperWord = word;
    transform(upperWord.begin(), upperWord.end(), upperWord.begin(), ::toupper);
    
    auto it = misinterpretations.find(upperWord);
    if (it != misinterpretations.end()) {
        return it->second[Random::getInt(0, it->second.size() - 1)];
    }
    
    // Number misinterpretations
    if (word == "1") return "one";
    if (word == "2") return "two"; 
    if (word == "3") return "three";
    if (word == "4") return "four";
    
    return simulateTypo(word);
}

string MessageNoiseSimulator::simulateTypo(const string& word) {
    if (word.length() <= 2) return word;
    
    string typo = word;
    int typoType = Random::getInt(0, 3);
    
    switch(typoType) {
        case 0: // Missing letter
            typo.erase(Random::getInt(0, typo.length() - 1), 1);
            break;
        case 1: // Duplicate letter
            if (typo.length() < 10) {
                int pos = Random::getInt(0, typo.length() - 1);
                typo.insert(pos, 1, typo[pos]);
            }
            break;
        case 2: // Wrong letter (keyboard adjacent)
            if (isalpha(typo[0])) {
                int pos = Random::getInt(0, typo.length() - 1);
                char original = tolower(typo[pos]);
                // Simple keyboard adjacency (qwerty)
                if (original == 'a') typo[pos] = 's';
                else if (original == 's') typo[pos] = 'a';
                else if (original == 'd') typo[pos] = 'f';
                else if (original == 'f') typo[pos] = 'd';
                // Add more as needed
            }
            break;
        case 3: // Transposition
            if (typo.length() >= 2) {
                int pos = Random::getInt(0, typo.length() - 2);
                swap(typo[pos], typo[pos + 1]);
            }
            break;
    }
    
    return typo;
}

// MessageFormatter implementations
string MessageFormatter::formatAsProtocol(const string& message, int protocolVersion) {
    switch(protocolVersion) {
        case 1: return useProtocolV1(message);
        case 2: return useProtocolV2(message);
        case 3: return useProtocolV3(message);
        default: return message;
    }
}

string MessageFormatter::useProtocolV1(const string& message) {
    // Basic: Just add protocol headers
    return "[DISPATCH] " + message + " [END]";
}

string MessageFormatter::useProtocolV2(const string& message) {
    // Compressed: Remove vowels from long words, abbreviate
    stringstream ss(message);
    string word, result;
    
    while (ss >> word) {
        if (word.length() > 4) {
            string compressed;
            for (char c : word) {
                if (string("aeiouAEIOU").find(c) == string::npos || isdigit(c)) {
                    compressed += c;
                }
            }
            result += (compressed.empty() ? word : compressed) + " ";
        } else {
            result += word + " ";
        }
    }
    
    return "[COMP:" + result + "]";
}

string MessageFormatter::useProtocolV3(const string& message) {
    // Binary-like: Convert to code-like format
    string result;
    for (char c : message) {
        if (isalpha(c)) {
            result += to_string(toupper(c) - 'A' + 1) + "-";
        } else if (isdigit(c)) {
            result += string(1, c) + "-";
        } else if (c == ' ') {
            result += "/";
        } else {
            result += string(1, c);
        }
    }
    return "[BIN:" + result + "]";
}

string MessageFormatter::compressGridDescription(const string& description) {
    // Simple compression for grid patterns
    string result = description;
    
    // Replace common phrases
    size_t pos = 0;
    while ((pos = result.find("row", pos)) != string::npos) {
        result.replace(pos, 3, "r");
        pos += 1;
    }
    
    pos = 0;
    while ((pos = result.find("column", pos)) != string::npos) {
        result.replace(pos, 6, "c");
        pos += 1;
    }
    
    return result;
}

vector<string> MessageFormatter::splitByBandwidth(const string& message, int maxLines, int maxLineLength) {
    vector<string> lines;
    stringstream ss(message);
    string line;
    
    while (getline(ss, line)) {
        if (line.length() <= maxLineLength) {
            lines.push_back(line);
        } else {
            // Split long lines
            size_t start = 0;
            while (start < line.length()) {
                size_t end = min(start + maxLineLength, line.length());
                if (end < line.length()) {
                    // Try to break at space
                    size_t breakPos = line.rfind(' ', end);
                    if (breakPos != string::npos && breakPos > start) {
                        end = breakPos;
                    }
                }
                lines.push_back(line.substr(start, end - start));
                start = end + (end < line.length() ? 1 : 0);
            }
        }
    }
    
    if (lines.size() > maxLines) {
        lines.resize(maxLines);
        lines.back() += "...";
    }
    
    return lines;
}