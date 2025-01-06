#ifndef SKYBOX_H
#define SKYBOX_H

#include "Player.h"

#include <raylib.h>
#include <rlgl.h>
#include <iostream>

class Skybox {
public:
    Mesh skyboxMesh;
    Shader skyboxShader;
    Model skybox;

    Skybox() {
        // Initialize skybox components
        // cube = GenMeshCube(1.0f, 1.0f, 1.0f);
        skyboxMesh = GenMeshSphere(1.0f, 32, 32);
        skybox = LoadModelFromMesh(skyboxMesh);

        skyboxShader = LoadShader("D:/Projects/raylib_game/src/shaders/skybox.vs", "D:/Projects/raylib_game/src/shaders/skybox.fs");
        // Configure skybox material
        Material skyboxMaterial = LoadMaterialDefault();
        skyboxMaterial.shader = skyboxShader;
        skybox.materials[0] = skyboxMaterial;

        if (skyboxShader.id == 0) {
            std::cout << "Shader failed to load!" << std::endl;
        }
    }
    ~Skybox() {
        UnloadModel(skybox);
        UnloadShader(skyboxShader);
    }

    void Draw(Player* player, Matrix view, Matrix projection) {
        SetShaderValueMatrix(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "view"), view);
        SetShaderValueMatrix(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "projection"), projection);

        rlDisableBackfaceCulling();  // Ensure inside of the cube is visible
        rlDisableDepthMask();
        DrawModel(skybox, player->camera.position, 1.0f, WHITE);
        rlEnableBackfaceCulling();
        rlEnableDepthMask();
    }
};
#endif