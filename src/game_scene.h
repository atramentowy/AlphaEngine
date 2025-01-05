#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "scene.h"
#include <raylib.h>

#include <iostream>

#include "player.h"

#include "rlgl.h"

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

    // Skybox
    Mesh cube;
    Shader skyboxShader;
    Model skybox;

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

        //Generates some random columns
        for (int i = 0; i < 20; i++)
        {
            heights[i] = (float)GetRandomValue(1, 12);
            positions[i] = Vector3{ (float)GetRandomValue(-15, 15), heights[i]/2.0f, (float)GetRandomValue(-15, 15) };
            colors[i] = Color{ static_cast<unsigned char>(GetRandomValue(20, 255)), static_cast<unsigned char>(GetRandomValue(10, 55)), 30, 255 };
        }

        // Initialize skybox components
        cube = GenMeshCube(1.0f, 1.0f, 1.0f);
        skybox = LoadModelFromMesh(cube);

        skyboxShader = LoadShader("D:/Projects/raylib_game/src/shaders/skybox.vs", "D:/Projects/raylib_game/src/shaders/skybox.fs");
        // Configure skybox material
        Material skyboxMaterial = LoadMaterialDefault();
        skyboxMaterial.shader = skyboxShader;
        skybox.materials[0] = skyboxMaterial;

        if (skyboxShader.id == 0) {
            std::cout << "Shader failed to load!" << std::endl;
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
        // skybox
        Matrix view = GetCameraMatrix(player.camera);
        // Matrix projection = GetCameraProjection(player.camera, CAMERA_PERSPECTIVE);
        Matrix projection = MatrixPerspective(45.0f * DEG2RAD, (float)GetScreenWidth() / GetScreenHeight(), 0.1f, 1000.0f);
        // Remove translation from view matrix
        // Remove translation from the view matrix for the skybox
        view.m12 = 0.0f;  // Reset X translation
        view.m13 = 0.0f;  // Reset Y translation
        view.m14 = 0.0f;  // Reset Z translation

        SetShaderValueMatrix(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "view"), view);
        SetShaderValueMatrix(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "projection"), projection);

        ClearBackground(RAYWHITE);
        BeginMode3D(player.camera);
            // Draw skybox
            rlDisableBackfaceCulling();  // Ensure inside of the cube is visible
            rlDisableDepthMask();
            DrawModel(skybox, player.camera.position, 1.0f, WHITE);
            rlEnableBackfaceCulling();
            rlEnableDepthMask();

            DrawGrid(30, 1.0f);

            // btVector3 position = spherePosition;
            DrawCube(Vector3{ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, RED);     // Draw a blue wall
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
        DrawFPS(10, 10);
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

        UnloadModel(skybox);
        UnloadShader(skyboxShader);
    }
};

#endif