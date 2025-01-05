#ifndef WRAPPERS_H
#define WRAPPERS_H

#include <raylib.h>

class ModelWrapper {
public:
    Model model;
    Vector3 position;

    ModelWrapper(const char* modelPath, Vector3 pos) {
        model = LoadModel(modelPath);
        position = pos;
    }

    ~ModelWrapper() {
        UnloadModel(model);
    }

    void Draw() {
        DrawModel(model, position, 1.0f, WHITE);
    }
};

class TextureWrapper {
public:
    Texture2D texture;

    TextureWrapper(const char* texturePath) {
        texture = LoadTexture(texturePath);
    }

    ~TextureWrapper() {
        UnloadTexture(texture);
    }

    void ApplyTexture(Model* model, const char* vertPath, const char* fragPath) {
        // Apply the texture to the model material
        model->materials[0].shader = LoadShader(vertPath, fragPath);  // Apply a basic shader
        model->materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    }
};

#endif