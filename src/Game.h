#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "Debug.h"
#include "Scene.h"
#include "Player.h"
#include "Ui.h"
#include "Skybox.h"
#include "Wrappers.h"
#include "GameObject.h"
#include "AssetManager.h"

#include <raylib.h>
#include <iostream>

class Game : public Scene {
public:
    // Physics config
    btDefaultCollisionConfiguration* collisionConfig;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* broadphase;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;

    // Ground members
    btCollisionShape* groundShape;
    btTransform groundTransform;
    btDefaultMotionState* groundMotionState;
    btRigidBody* groundBody;

    AssetManager manager;

    Player player;
    Skybox skybox;

    // Monkey model
    btCollisionShape* bodyShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));

    // Create object pointers
    GameObject* object;
    GameObject* object1;

    // pause state
    bool paused = false;

    // cubes settings
    float heights[20] = { 0 };
    Vector3 positions[20] = { 0 };
    Color colors[20] = { 0 };

    void Init() override {
        HideCursor();
        DisableCursor();

        // Bullet Physics members
        collisionConfig = new btDefaultCollisionConfiguration();
        dispatcher = new btCollisionDispatcher(collisionConfig);
        broadphase = new btDbvtBroadphase();
        solver = new btSequentialImpulseConstraintSolver();
        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);

        // Set gravity
        dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));

        // Ground members
        btCollisionShape* groundShape;
        btTransform groundTransform;
        btDefaultMotionState* groundMotionState;
        btRigidBody* groundBody;

        // Initialize ground (static plane)
        groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0); // Ground is at y=0
        groundTransform.setIdentity();
        groundMotionState = new btDefaultMotionState(groundTransform);

        btRigidBody::btRigidBodyConstructionInfo groundCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
        groundBody = new btRigidBody(groundCI);

        dynamicsWorld->addRigidBody(groundBody);

        player.Init(dynamicsWorld);

        // Game objects
        fs::path modelPath = projectRoot/"res"/"monkey.obj";
        fs::path modelTexPath = projectRoot/"res"/"monkey.jpg";
        object = new GameObject(dynamicsWorld, bodyShape, btVector3{0.0f, 5.0f, 0.0f}, 1.0f, modelPath.string().c_str(), modelTexPath.string().c_str());
        object1 = new GameObject(dynamicsWorld, bodyShape, btVector3{0.0f, 5.0f, 0.0f}, 1.0f, modelPath.string().c_str(), modelTexPath.string().c_str());
    }

    void Update(float deltaTime) override {
        if(IsKeyPressed(KEY_P)) paused = !paused;

        if(!paused) {
            // Handle collision between objA and objB
            int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
            for (int i = 0; i < numManifolds; i++) {
                btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
                const btCollisionObject* objA = contactManifold->getBody0();
                const btCollisionObject* objB = contactManifold->getBody1();
            }

            dynamicsWorld->stepSimulation(1.0f / 60.0f);
            player.Update(deltaTime);
        }
    }

    void Draw() override {
        Matrix view = GetCameraMatrix(player.camera);
        Matrix projection = MatrixPerspective(45.0f * DEG2RAD, (float)GetScreenWidth() / GetScreenHeight(), 0.1f, 1000.0f);
        
        ClearBackground(RAYWHITE);

        UpdateDebug();

        BeginMode3D(player.camera);
            skybox.Draw(&player, view, projection);

            DrawGrid(50, 2.0f);
            DrawPlane(Vector3{-0.01f, -0.01f, -0.01f}, Vector2{100.0f, 100.0f}, WHITE);

            DrawCube(Vector3{ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, RED); // Draw a blue wall
            DrawCube(Vector3{ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME); // Draw a green wall
            DrawCube(Vector3{ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD); // Draw a yellow wall

            object->Draw();
            object1->Draw();

            player.Draw();
        EndMode3D();

        // Draw ui
        if (paused) { // Draw pause menu
            DrawText("Paused", 300, 300, 40, BLACK); 
        } else {
            DrawCrosshair();
            DrawInfo(&player);
        }

        DrawInfo(&player);
    }

    void Unload() override {
        dynamicsWorld->removeRigidBody(groundBody);
        delete groundBody;
        delete groundMotionState;
        delete groundShape;
        delete dynamicsWorld;
        delete solver;
        delete dispatcher;
        delete collisionConfig;
        delete broadphase;

        delete object;
        delete object1;
    }
};

#endif