#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "Scene.h"
#include "Player.h"
#include "Skybox.h"
#include "Wrappers.h"
#include "GameObject.h"

#include <raylib.h>
#include <iostream>

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

        //
        object = new GameObject(dynamicsWorld, bodyShape, btVector3{0.0f, 5.0f, 0.0f}, 1.0f, "D:/Projects/raylib_game/res/monkey.obj", "D:/Projects/raylib_game/res/monkey.jpg");
        object1 = new GameObject(dynamicsWorld, bodyShape, btVector3{0.0f, 5.0f, 0.0f}, 1.0f, "D:/Projects/raylib_game/res/monkey.obj", "D:/Projects/raylib_game/res/monkey.jpg");
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
        // Remove translation from the view matrix for the skybox
        view.m12 = 0.0f;  // Reset X translation
        view.m13 = 0.0f;  // Reset Y translation
        view.m14 = 0.0f;  // Reset Z translation

        ClearBackground(RAYWHITE);
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
        }
        // Draw crosshair
        // Get the center of the screen
        Vector2 center = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
        // Draw circle crosshair at the center
        DrawCircleV(center, 2.0f, WHITE); // Circle with center and radius
        // Optionally draw a border around the circle (like a ring)
        DrawCircleLines((int)center.x, (int)center.y, 1.5f, BLACK); // Border around the circle


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
        DrawText(TextFormat("Fps: %i", GetFPS()), 610, 85, 10, WHITE);
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