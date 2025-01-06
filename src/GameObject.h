#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Wrappers.h"
#include "PhysicsBody.h"

class GameObject {
private:
    btDiscreteDynamicsWorld* world;
    PhysicsBody* body;
    ModelWrapper* model;
    TextureWrapper* texture;

public:
    GameObject(btDiscreteDynamicsWorld* world,
        btCollisionShape* bodyShape,
        const btVector3& startPosition,
        float mass,
        const char* modelPath,
        const char* texturePath) {
            body = new PhysicsBody(world, bodyShape, startPosition, mass);
            model = new ModelWrapper(modelPath);
            texture = new TextureWrapper(texturePath);
    }

    ~GameObject() {
        delete body;
        delete model;
        delete texture;
    }

    void Update() {
        // update physics?
    }

    void Draw() {
        model->Draw(body->GetPosition());
    }
};
#endif