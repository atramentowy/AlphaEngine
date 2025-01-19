#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <raylib.h>
#include <string.h>

#define MAX_ASSETS 1

struct TextureAsset {
    const char *id;
    Texture2D texture;
};

struct SoundAsset{
    const char *id;
    Sound sound;
};

struct ModelAsset{
    const char *id;
    Model model;
};

class AssetManager {
public:
    TextureAsset textures[MAX_ASSETS];
    SoundAsset sounds[MAX_ASSETS];
    ModelAsset models[MAX_ASSETS];

    int textureCount = 0;
    int soundCount = 0;
    int modelCount = 0;

    AssetManager() {}
    ~AssetManager() {
        UnloadAssets();
    }

    void UnloadAssets() {
        for(int i = 0; i < this->textureCount; i++) {
            UnloadTexture(this->textures[i].texture);
        }
        for(int i = 0; i < this->soundCount; i++) {
            UnloadSound(this->sounds[i].sound);
        }
        for(int i = 0; i < this->modelCount; i++) {
            UnloadModel(this->models[i].model);
    }
}
};

void LoadTextureAsset(AssetManager *manager, const char *id, const char *path) {
    if(manager->textureCount < MAX_ASSETS) {
        manager->textures[manager->textureCount].id = id;
        manager->textures[manager->textureCount].texture = LoadTexture(path);
        manager->textureCount++;
    }
}

void LoadSoundAsset(AssetManager *manager, const char *id, const char *path) {
    if(manager->soundCount < MAX_ASSETS) {
        manager->sounds[manager->soundCount].id = id;
        manager->sounds[manager->soundCount].sound = LoadSound(path);
        manager->soundCount++;
    }
}

void LoadModelAsset(AssetManager *manager, const char *id, const char *path) {
    if(manager->modelCount < MAX_ASSETS) {
        manager->models[manager->modelCount].id = id;
        manager->models[manager->modelCount].model = LoadModel(path);
        manager->modelCount++;
    }
}

Texture2D GetTextureAsset(AssetManager *manager, const char *id, const char *path) {
    for (int i = 0; i < manager->textureCount; i++) {
        if (strcmp(manager->textures[i].id, id) == 0) {
            return manager->textures[i].texture;
        }
    }
    return Texture2D{0};
}

Sound GetSoundAsset(AssetManager *manager, const char *id, const char *path) {
    for (int i = 0; i < manager->soundCount; i++) {
        if (strcmp(manager->sounds[i].id, id) == 0) {
            return manager->sounds[i].sound;
        }
    }
    return Sound{0};
}

Model GetModelAsset(AssetManager *manager, const char *id, const char *path) {
    for (int i = 0; i < manager->modelCount; i++) {
        if (strcmp(manager->models[i].id, id) == 0) {
            return manager->models[i].model;
        }
    }
    return Model{0};
}

#endif