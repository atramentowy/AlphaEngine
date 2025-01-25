#ifndef APPLICATION_H
#define APPLICATION_H

#include "SceneManager.h"
#include "MainMenu.h"
#include "Game.h"

#include <raylib.h>

#include <iostream>
#include <stdexcept>

class Application {
private:
    SceneManager sceneManager;
    MainMenu* mainMenu;
    Game* game;

public:
    Application() : sceneManager(), mainMenu(nullptr), game(nullptr) {}

    SceneManager* GetSceneManager() {
        return &sceneManager;
    }

    void Init() {
        // Suppress info logs but keep warnings and errors
        SetTraceLogLevel(LOG_WARNING);

        InitWindow(800, 600, "AlphaEngine");
        SetTargetFPS(60);
        HideCursor();

        // Create scenes
        mainMenu = new MainMenu();
        game = new Game();

        mainMenu->SetSceneManager(GetSceneManager());
        game->SetSceneManager(GetSceneManager());

        // Set initial scene
        sceneManager.SetScene(game);
        // sceneManager.SetScene(mainMenu);
    }

    void Run() {
        while(!WindowShouldClose()) {
            float deltaTime = GetFrameTime();
            sceneManager.Update(deltaTime);

            BeginDrawing();
            sceneManager.Draw();
            EndDrawing();
        }
    }

    void Cleanup() {
        // Unload and delete scenes to free memory
        if (mainMenu) {
            mainMenu->Unload();
            delete mainMenu;
        }

        if (game) {
            game->Unload();
            delete game;
        }

        CloseWindow();
    }
};

#endif