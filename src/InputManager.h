#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <raylib.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "Filepath.h"

class InputManager {
public:
    static bool LoadBindingsFromFile(const char* filePath) {
        std::ifstream file(filePath);
        if(!file.is_open()) {
            std::cerr << "Error: Unable to open bindings file: " << filePath << std::endl;
            return false;
        }

        GetInstance().bindings.clear();

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string action, keyName;
            if (!(iss >> action >> keyName)) continue; // Skip invalid lines

            int key = GetKeyFromName(keyName);
            if(key != -1) {
                GetInstance().bindings[action].push_back(key);
            } else {
                std::cerr << "Warning: Unknown key '" << keyName << "' in bindings file." << std::endl;
            }
        }

        file.close();
        return true;
    }

    static bool SaveBindingsToFile(const std::string& filePath) {
        std::ofstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to save bindings to file: " << filePath << std::endl;
            return false;
        }

        for (const auto& [action, keys] : GetInstance().bindings) {
            for (int key : keys) {
                file << action << " " << GetNameFromKey(key) << "\n";
            }
        }

        file.close();
        return true;
    }

    // Update a key binding for an action (replaces all existing keys for the action)
    static void UpdateBinding(const std::string& action, int key) {
        GetInstance().bindings[action] = { key };
    }

    // Check if any key bound to an action is just pressed
    static bool IsActionPressed(const std::string& action) {
        auto it = GetInstance().bindings.find(action);
        if (it != GetInstance().bindings.end()) {
            for (int key : it->second) {
                if (IsKeyPressed(key)) return true;
            }
        }
        return false;
    }

    // Check if any key bound to an action is held down
    static bool IsActionDown(const std::string& action) {
        auto it = GetInstance().bindings.find(action);
        if (it != GetInstance().bindings.end()) {
            for (int key : it->second) {
                if (IsKeyDown(key)) return true;
            }
        }
        return false;
    }

    // Check if any key bound to an action is just released
    static bool IsActionReleased(const std::string& action) {
        auto it = GetInstance().bindings.find(action);
        if (it != GetInstance().bindings.end()) {
            for (int key : it->second) {
                if (IsKeyReleased(key)) return true;
            }
        }
        return false;
    }

private:
    // Private constructor for singleton
    InputManager() = default;

    // Singleton instance
    static InputManager& GetInstance() {
        static InputManager instance;
        return instance;
    }

    // Map of action names to their bound keys
    std::unordered_map<std::string, std::vector<int>> bindings;

    // Helper function to convert a key name to a raylib key code
    static int GetKeyFromName(const std::string& keyName) {
        if (keyName == "KEY_A") return KEY_A;
        if (keyName == "KEY_D") return KEY_D;
        if (keyName == "KEY_SPACE") return KEY_SPACE;
        if (keyName == "KEY_F") return KEY_F;
        if (keyName == "KEY_W") return KEY_W;
        if (keyName == "KEY_S") return KEY_S;
        // Add more key mappings if needed
        return -1; // Invalid key
    }

    // Helper function to convert a raylib key code to a string
    static std::string GetNameFromKey(int key) {
        if (key == KEY_A) return "KEY_A";
        if (key == KEY_D) return "KEY_D";
        if (key == KEY_SPACE) return "KEY_SPACE";
        if (key == KEY_F) return "KEY_F";
        if (key == KEY_W) return "KEY_W";
        if (key == KEY_S) return "KEY_S";
        // Add more key mappings if needed
        return "UNKNOWN_KEY";
    }
};

#endif