#ifndef DIALOGUE_MANAGER_H
#define DIALOGUE_MANAGER_H

#include <raylib.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "Filepath.h"

class DialogueManager {
private:
    std::vector<std::string> dialogueLines;

    int currentLine = 0;
    int currentChar = 0;

    bool isTyping = false;
    float typingSpeed = 20.0f;
    float typingTimer = 0.0f;

    //
    Rectangle dialogueBox = {50, 400, 700, 150};
    int fontSize = 20;
    Color textColor = BLACK;
public:
    bool LoadDialogueFromFile(const char* filePath) {
        std::ifstream file(filePath);
        if(!file.is_open()) {
            std::cerr << "Error: Unable to open dialogue file: " << filePath << std::endl;
            return false;
        }

        dialogueLines.clear();
        std::string line;
        while (std::getline(file, line)) {
            if(!line.empty()) {
                dialogueLines.push_back(line);
            }
        }
        file.close();

        if(!dialogueLines.empty()) {
            currentLine = 0;

            // Reset typing effect
            currentChar = 0;
            isTyping = true;
            typingTimer = 0.0f;

            return true;
        }

        std::cerr << "Error: Dialogue file is empty or invalid." << std::endl;
        return false;
    }

    void Update(float deltaTime) {
        if (isTyping) {
            typingTimer += deltaTime * typingSpeed;
            currentChar = int(typingTimer);

            // If all characters are displayer, stop the typing effect
            if (currentChar >= dialogueLines[currentLine].size()) {
                isTyping = false;
            }
        } else if (IsKeyPressed(KEY_ENTER)) {
            NextLine();
        }
    }

    void Draw() {
        DrawRectangleRec(dialogueBox, GRAY);
        DrawRectangleLinesEx(dialogueBox, 2, DARKGRAY);

        std::string displayedText = dialogueLines[currentLine].substr(0, currentChar);
        DrawText(displayedText.c_str(), dialogueBox.x + 10, dialogueBox.y + 10, fontSize, textColor);

        // Draw "Press space to continue" prompt when typing is done
        if (!isTyping) {
            DrawText("Press SPACE to continue...", dialogueBox.x + 10, dialogueBox.y - 20, fontSize, textColor);
        }
    }

    void NextLine() {
        if (currentLine < dialogueLines.size() - 1) {
            currentLine++;
            // Reset typing effect
            currentChar = 0;
            isTyping = true;
            typingTimer = 0.0f;
        }
    }

    bool IsFinished() const {
        return currentLine >= dialogueLines.size() - 1 && !isTyping;
    }
};

#endif