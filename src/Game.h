#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "Physics.h"

#include "Debug.h"
#include "Scene.h"
#include "Player.h"
#include "Ui.h"
#include "Skybox.h"
#include "Wrappers.h"
#include "GameObject.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "DialogueManager.h"

#include <raylib.h>
#include <iostream>

class Game : public Scene {
public:
    AssetManager manager;
    Physics physics;
    Player player;
    Skybox skybox;

    // Dialogue Manager
    DialogueManager dialogueManager;

    // Monkey model
    btCollisionShape* bodyShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
    // Create object pointers
    GameObject* object;
    GameObject* object1;

    // pause state
    bool paused = false;

    void Init() override {
        HideCursor();
        DisableCursor();

        physics.Init();
        player.Init(physics.dynamicsWorld);

        // Load bindings
        fs::path bindingsPath = projectRoot/"res"/"dialogue.txt";
        if (!InputManager::LoadBindingsFromFile(bindingsPath.string().c_str())) {
            std::cerr << "Failed to load bindings!" << std::endl;
            CloseWindow();
            exit(0); // Exit if bindings fail to load
        }

        // Load dialogue from file
        fs::path filePath = projectRoot/"res"/"dialogue.txt";
        if (!dialogueManager.LoadDialogueFromFile(filePath.string().c_str())) {
            CloseWindow();
            exit(0); // Exit if dialogue file couldn't be loaded
        }

        // Game objects
        fs::path modelPath = projectRoot/"res"/"monkey.obj";
        fs::path modelTexPath = projectRoot/"res"/"monkey.jpg";
        object = new GameObject(physics.dynamicsWorld, bodyShape, btVector3{0.0f, 5.0f, 0.0f}, 1.0f, modelPath.string().c_str(), modelTexPath.string().c_str());
        object1 = new GameObject(physics.dynamicsWorld, bodyShape, btVector3{0.0f, 5.0f, 0.0f}, 1.0f, modelPath.string().c_str(), modelTexPath.string().c_str());
    }

    void Update(float deltaTime) override {
        if(IsKeyPressed(KEY_P)) paused = !paused;

        if(!paused) {
            physics.Update();
            player.Update(deltaTime);
            // Update the dialogue manager
            dialogueManager.Update(deltaTime);
        }
    }

    void Draw() override {
        Matrix view = GetCameraMatrix(player.camera);
        Matrix projection = MatrixPerspective(45.0f * DEG2RAD, (float)GetScreenWidth() / GetScreenHeight(), 0.1f, 1000.0f);
        
        ClearBackground(RAYWHITE);

        UpdateDebug();

        BeginMode3D(player.camera);
            skybox.Draw(&player, view, projection);

            // Draw ground
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
            dialogueManager.Draw();
        }
        DrawInfo(&player);
    }

    void Unload() override {
        physics.Unload();
        
        // objects
        delete object;
        delete object1;
    }
};

#endif