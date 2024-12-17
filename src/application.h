#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <stdexcept>

#include "raylib.h"
#include "rcamera.h"

#include "btBulletDynamicsCommon.h"
#include <entt/entt.hpp>
// #include <entt/entity/registry.hpp>

class Application {
private:
    const int screenWidth = 800;
	const int screenHeight = 450;

    Camera camera = { 0 };
    int cameraMode = CAMERA_FIRST_PERSON;

    //const int MAX_COLUMNS = 20;

    float heights[20] = { 0 };
    Vector3 positions[20] = { 0 };
    Color colors[20] = { 0 };
public:
    void Init();
    void Run();
    //void Input();
    void Update();
    void Draw();
};

#endif