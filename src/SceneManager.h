#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Scene.h"

#include <stack>

class SceneManager {
private:
    Scene* currentScene;
public:
    SceneManager() : currentScene(nullptr) {}

    // Set the active scene
    void SetScene(Scene* scene) {
        // Unload the current scene (if any)
        if (currentScene) {
            currentScene->Unload();
        }

        currentScene = scene;

        // Initialize the new scene
        if(currentScene) {
            currentScene->Init();
        }
    }

    // Update the active scene
    void Update(float deltaTime) {
        if(currentScene) {
            currentScene->Update(deltaTime);
        }
    }

    void Draw() {
        if(currentScene) {
            currentScene->Draw();
        }
    }
};

#endif