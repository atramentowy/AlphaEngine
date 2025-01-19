#ifndef SKYBOX_H
#define SKYBOX_H

#include "Player.h"
#include "Filepath.h"

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
        skyboxMesh = GenMeshSphere(1.0f, 32, 32);
        skybox = LoadModelFromMesh(skyboxMesh);

        fs::path shaderVSPath = projectRoot/"src"/"shaders"/"skybox.vs";
        fs::path shaderFSPath = projectRoot/"src"/"shaders"/"skybox.fs";
        skyboxShader = LoadShader(shaderVSPath.string().c_str(), shaderFSPath.string().c_str());
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
        // Remove translation from the view matrix for the skybox
        view.m12 = 0.0f;  // Reset X translation
        view.m13 = 0.0f;  // Reset Y translation
        view.m14 = 0.0f;  // Reset Z translation

        SetShaderValueMatrix(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "view"), view);
        SetShaderValueMatrix(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "projection"), projection);

        rlDisableBackfaceCulling();  // Ensure inside of the model is visible
        rlDisableDepthMask();
        DrawModel(skybox, player->camera.position, 1.0f, WHITE);
        rlEnableBackfaceCulling();
        rlEnableDepthMask();
    }
};
#endif