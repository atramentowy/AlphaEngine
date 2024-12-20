#include "application.h"

void Application::Init() {
    SetTraceLogLevel(LOG_NONE); // before window init
    InitWindow(screenWidth, screenHeight, "Raylib game");
    //SetExitKey(KEY_NULL); // after window init
    DisableCursor();
	SetTargetFPS(60);

    player.Init();
}

void Application::Run() {
    // Physics world setup
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    btDefaultCollisionConfiguration* collisionConfig = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfig);
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
    btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);

    // Set gravity
    dynamicsWorld->setGravity(btVector3(0, -9.8f, 0));

    btCollisionShape* groundShape = new btBoxShape(btVector3(50, 1, 50)); // Size of the ground
    btDefaultMotionState* groundMotion = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
    btRigidBody::btRigidBodyConstructionInfo groundRBCI(0, groundMotion, groundShape, btVector3(0, 0, 0)); // Mass 0 for static
    btRigidBody* groundBody = new btRigidBody(groundRBCI);
    dynamicsWorld->addRigidBody(groundBody);

    // Player
    btCollisionShape* playerShape = new btCapsuleShape(0.5f, 1.8f); // Capsule for player
    btDefaultMotionState* playerMotion = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 5, 0))); // Start position
    btScalar playerMass = 1.0f;
    btVector3 playerInertia(0, 0, 0);
    playerShape->calculateLocalInertia(playerMass, playerInertia);
    btRigidBody::btRigidBodyConstructionInfo playerRBCI(playerMass, playerMotion, playerShape, playerInertia);
    btRigidBody* playerBody = new btRigidBody(playerRBCI);
    dynamicsWorld->addRigidBody(playerBody);

    // Generates some random columns
    for (int i = 0; i < 20; i++)
    {
        heights[i] = (float)GetRandomValue(1, 12);
        positions[i] = Vector3{ (float)GetRandomValue(-15, 15), heights[i]/2.0f, (float)GetRandomValue(-15, 15) };
        colors[i] = Color{ static_cast<unsigned char>(GetRandomValue(20, 255)), static_cast<unsigned char>(GetRandomValue(10, 55)), 30, 255 };
    }

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        // Step the physics world
        dynamicsWorld->stepSimulation(deltaTime, 10);

        // Get player position
        btTransform playerTransform;
        playerBody->getMotionState()->getWorldTransform(playerTransform);
        btVector3 playerPosition = playerTransform.getOrigin();

        // Update player position in your game
        // camera.position = { playerPosition.getX(), playerPosition.getY(), playerPosition.getZ() };

        int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
        for (int i = 0; i < numManifolds; i++) {
            btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
            const btCollisionObject* objA = contactManifold->getBody0();
            const btCollisionObject* objB = contactManifold->getBody1();
            
            // Handle collision between objA and objB
        }

        Update();
        Draw();
    }
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfig;
    delete broadphase;
    delete groundBody;
    delete groundShape;
    delete playerBody;
    delete playerShape;

    CloseWindow();
}

void Application::Update() {
    float deltaTime = GetFrameTime();

    player.Update(deltaTime);
}

void Application::Draw() {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(player.camera);
        DrawPlane(Vector3{ 0.0f, 0.0f, 0.0f }, Vector2{ 32.0f, 32.0f }, LIGHTGRAY); // Draw ground
        DrawCube(Vector3{ -16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, BLUE);     // Draw a blue wall
        DrawCube(Vector3{ 16.0f, 2.5f, 0.0f }, 1.0f, 5.0f, 32.0f, LIME);      // Draw a green wall
        DrawCube(Vector3{ 0.0f, 2.5f, 16.0f }, 32.0f, 5.0f, 1.0f, GOLD);      // Draw a yellow wall

        // Draw some cubes around
        for (int i = 0; i < 20; i++) {
            DrawCube(positions[i], 2.0f, heights[i], 2.0f, colors[i]);
            DrawCubeWires(positions[i], 2.0f, heights[i], 2.0f, MAROON);
        }

        player.Draw();

    EndMode3D();

    DrawRectangle(600, 5, 195, 120, Fade(BLACK, 0.5f));

    DrawText(TextFormat("Player position: (%06.3f, %06.3f, %06.3f)", player.position.x, player.position.y, player.position.z), 610, 10, 10, WHITE);
    DrawText(TextFormat("Player yaw/pitch: (%06.3f, %06.3f, %06.3f)", player.yaw, player.pitch), 610, 20, 10, WHITE);
    DrawText("Camera status:", 610, 30, 10, WHITE);
    DrawText(TextFormat("- Mode: %s", (player.cameraMode == CAMERA_FREE) ? "FREE" :
                                        (player.cameraMode == CAMERA_FIRST_PERSON) ? "FIRST_PERSON" :
                                        (player.cameraMode == CAMERA_THIRD_PERSON) ? "THIRD_PERSON" :
                                        (player.cameraMode == CAMERA_ORBITAL) ? "ORBITAL" : "CUSTOM"), 610, 40, 10, WHITE);
    DrawText(TextFormat("- Projection: %s", (player.camera.projection == CAMERA_PERSPECTIVE) ? "PERSPECTIVE" :
                                            (player.camera.projection == CAMERA_ORTHOGRAPHIC) ? "ORTHOGRAPHIC" : "CUSTOM"), 610, 50, 10, WHITE);
    DrawText(TextFormat("- Position: (%06.3f, %06.3f, %06.3f)", player.camera.position.x, player.camera.position.y, player.camera.position.z), 610, 60, 10, WHITE);
    DrawText(TextFormat("- Target: (%06.3f, %06.3f, %06.3f)", player.camera.target.x, player.camera.target.y, player.camera.target.z), 610, 70, 10, WHITE);
    DrawText(TextFormat("- Up: (%06.3f, %06.3f, %06.3f)", player.camera.up.x, player.camera.up.y, player.camera.up.z), 610, 80, 10, WHITE);

	EndDrawing();
}