#ifndef APPLICATION_H
#define APPLICATION_H

#include "SceneManager.h"
#include "MainMenuScene.h"
#include "GameScene.h"

#include <raylib.h>

#include <iostream>
#include <stdexcept>

class Application {
private:
    SceneManager sceneManager;
    MainMenuScene* mainMenuScene;
    GameScene* gameScene;

public:
    Application() : sceneManager(), mainMenuScene(nullptr), gameScene(nullptr) {}

    SceneManager* GetSceneManager() {
        return &sceneManager;
    }

    void Init() {
        InitWindow(800, 600, "Raylib Game");
        SetTargetFPS(60);
        HideCursor();

        // Create scenes
        mainMenuScene = new MainMenuScene();
        gameScene = new GameScene();

        mainMenuScene->SetSceneManager(GetSceneManager());
        gameScene->SetSceneManager(GetSceneManager());

        // Set initial scene
        sceneManager.SetScene(gameScene);
        // sceneManager.SetScene(mainMenuScene);
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
        if (mainMenuScene) {
            mainMenuScene->Unload();
            delete mainMenuScene;
        }

        if (gameScene) {
            gameScene->Unload();
            delete gameScene;
        }

        CloseWindow();
    }
};

#endif