#ifndef APPLICATION_H
#define APPLICATION_H

#include <iostream>
#include <stdexcept>

//#include <raylib.h>
//#include <raymath.h>
//#include <rcamera.h>
#include "player.h"

#include "btBulletDynamicsCommon.h"
// #include <entt/entt.hpp>
// #include <entt/entity/registry.hpp>

class Application {
private:
    const int screenWidth = 800;
	const int screenHeight = 450;

    Player player;
    
    // floor settings
    Vector3 floorPosition = { 0.0f, 0.0f, 0.0f };
    Vector3 floorSize = { 50.0f, 1.0f, 50.0f };

    // cubes settings
    float heights[20] = { 0 };
    Vector3 positions[20] = { 0 };
    Color colors[20] = { 0 };
public:
    void Init();
    void Run();
    void Update();
    void Draw();
};

#endif