#ifndef MAIN_MENU_SCENE_H
#define MAIN_MENU_SCENE_H

#include "Scene.h"
#include "Game.h"

#include <raylib.h>
#include <iostream>

class MainMenu : public Scene {
public:
    void Init() override {
        ShowCursor();
        EnableCursor();
        // Initialize main menu
        std::cout<<"init main menu"<<std::endl;
    }

    void Update(float deltaTime) override {
        // Transition to the game scene
        std::cout<<"update main menu"<<std::endl;
        if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
        {
            sceneManager->SetScene(new Game());
        }
    }

    void Draw() override {
        // Draw main menu
        std::cout<<"draw main menu"<<std::endl;
        ClearBackground(RAYWHITE);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GREEN);
        DrawText("TITLE SCREEN", 20, 20, 40, DARKGREEN);
        DrawText("PRESS ENTER to JUMP to GAMEPLAY SCREEN", 120, 220, 20, DARKGREEN);
    }

    void Unload() override {
        // Unload any scene resources
    }
};

#endif