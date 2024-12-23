#ifndef SCENE_H
#define SCENE_H

class SceneManager;  // Forward declaration

class Scene {
protected:
    SceneManager* sceneManager = nullptr;
public:
    virtual ~Scene() = default;

    void SetSceneManager(SceneManager* manager) {
        sceneManager = manager;
    }

    virtual void Init() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;
    virtual void Unload() = 0; // cleanup when leaving the scene
};

#endif