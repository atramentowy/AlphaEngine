#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "scene.h"
#include <raylib.h>

#include <iostream>

#include "player.h"

class GameScene : public Scene {
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

    Player player;

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
        collisionConfig = new btDefaultCollisionConfiguration();;
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

        //Generates some random columns
        for (int i = 0; i < 20; i++)
        {
            heights[i] = (float)GetRandomValue(1, 12);
            positions[i] = Vector3{ (float)GetRandomValue(-15, 15), heights[i]/2.0f, (float)GetRandomValue(-15, 15) };
            colors[i] = Color{ static_cast<unsigned char>(GetRandomValue(20, 255)), static_cast<unsigned char>(GetRandomValue(10, 55)), 30, 255 };
        }
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
        ClearBackground(RAYWHITE);
        BeginMode3D(player.camera);

            DrawGrid(30, 1.0f);

            // btVector3 position = spherePosition;
            DrawCube(Vector3{ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
            DrawCube(Vector3{ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
            DrawCube(Vector3{ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);      // Draw a yellow wall

            //Draw some cubes around
            for (int i = 0; i < 20; i++) {
                DrawCube(positions[i], 2.0f, heights[i], 2.0f, colors[i]);
                DrawCubeWires(positions[i], 2.0f, heights[i], 2.0f, MAROON);
            }
            
            player.Draw();

        EndMode3D();

        // Draw ui
        if (paused) { // Draw pause menu
            DrawText("Paused", 300, 300, 40, BLACK);
        }

        DrawRectangle(600, 0, 195, 120, Fade(BLACK, 0.5f));

        DrawText("Player status:", 610, 5, 10, RED);
        DrawText(TextFormat("- position: (%06.3f, %06.3f, %06.3f)", player.position.x, player.position.y, player.position.z), 610, 15, 10, WHITE);
        DrawText(TextFormat("- yaw/pitch: (%06.3f, %06.3f)", player.yaw, player.pitch), 610, 25, 10, WHITE);

        DrawText("Camera status:", 610, 35, 10, RED);
        DrawText(TextFormat("- mode: %s", 
            (player.cameraMode == CAMERA_FREE) ? "FREE" :
            (player.cameraMode == CAMERA_FIRST_PERSON) ? "FIRST_PERSON" :
            (player.cameraMode == CAMERA_THIRD_PERSON) ? "THIRD_PERSON" :
            (player.cameraMode == CAMERA_ORBITAL) ? "ORBITAL" : "CUSTOM"), 
            610, 45, 10, WHITE
        );
        DrawText(TextFormat("- projection: %s", 
            (player.camera.projection == CAMERA_PERSPECTIVE) ? "PERSPECTIVE" :
            (player.camera.projection == CAMERA_ORTHOGRAPHIC) ? "ORTHOGRAPHIC" : "CUSTOM"),
            610, 55, 10, WHITE
        );
        DrawText(TextFormat("- position: (%06.3f, %06.3f, %06.3f)", player.camera.position.x, player.camera.position.y, player.camera.position.z), 610, 65, 10, WHITE);
        DrawText(TextFormat("- target: (%06.3f, %06.3f, %06.3f)", player.camera.target.x, player.camera.target.y, player.camera.target.z), 610, 75, 10, WHITE);
        DrawText(TextFormat("isOnGround: (%06.3f)", player.isOnGround), 610, 85, 10, WHITE);
    }

    void Unload() override {
        //delete sphere;
        dynamicsWorld->removeRigidBody(groundBody);
        delete groundBody;
        delete groundMotionState;
        delete groundShape;
        delete dynamicsWorld;
        delete solver;
        delete dispatcher;
        delete collisionConfig;
        delete broadphase;
    }
};

#endif