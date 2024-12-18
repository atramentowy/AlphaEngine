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



    // player settings
    //Vector3 playerPosition = {0.0f, 1.0f, 0.0f};
    //const float playerSpeed = 5.0f;
    //Vector3 playerSize = {1.0f, 2.0f, 1.0f};
    
    // floor settings
    Vector3 floorPosition = { 0.0f, 0.0f, 0.0f };
    Vector3 floorSize = { 50.0f, 1.0f, 50.0f };

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