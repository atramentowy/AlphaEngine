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
    // Generates some random columns
    for (int i = 0; i < 20; i++)
    {
        heights[i] = (float)GetRandomValue(1, 12);
        positions[i] = Vector3{ (float)GetRandomValue(-15, 15), heights[i]/2.0f, (float)GetRandomValue(-15, 15) };
        colors[i] = Color{ static_cast<unsigned char>(GetRandomValue(20, 255)), static_cast<unsigned char>(GetRandomValue(10, 55)), 30, 255 };
    }

    while (!WindowShouldClose()) {
        Update();
        Draw();
    }
    CloseWindow();
}

//void Application::Input() {}

void Application::Update() {
    float deltaTime = GetFrameTime();

    player.Update(deltaTime);

    /*
    // Switch camera projection
    // if (IsKeyPressed(KEY_P)) {
    //     if (camera.projection == CAMERA_PERSPECTIVE) {
    //         // Create isometric view
    //         cameraMode = CAMERA_THIRD_PERSON;
    //         // Note: The target distance is related to the render distance in the orthographic projection
    //         camera.position = Vector3{ 0.0f, 2.0f, -100.0f };
    //         camera.target = Vector3{ 0.0f, 2.0f, 0.0f };
    //         camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
    //         camera.projection = CAMERA_ORTHOGRAPHIC;
    //         camera.fovy = 20.0f; // near plane width in CAMERA_ORTHOGRAPHIC
    //         CameraYaw(&camera, -135 * DEG2RAD, true);
    //         CameraPitch(&camera, -45 * DEG2RAD, true, true, false);
    //     } else if (camera.projection == CAMERA_ORTHOGRAPHIC) {
    //         // Reset to default view
    //         cameraMode = CAMERA_FIRST_PERSON;
    //         camera.position = Vector3{ 0.0f, 2.0f, 0.0f };
    //         camera.target = Vector3{ 0.0f, 2.0f, 0.0f };
    //         camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
    //         camera.projection = CAMERA_PERSPECTIVE;
    //         camera.fovy = 60.0f;
    //     }
    // }

    // Switch camera mode
    if (IsKeyPressed(KEY_ONE)) {
        cameraMode = CAMERA_FREE;
        camera.up = Vector3{ 0.0f, 1.0f, 0.0f }; // Reset roll
    }

    if (IsKeyPressed(KEY_TWO)) {
        cameraMode = CAMERA_FIRST_PERSON;
        camera.up = Vector3{ 0.0f, 1.0f, 0.0f }; // Reset roll
    }

    if (IsKeyPressed(KEY_THREE)) {
        cameraMode = CAMERA_THIRD_PERSON;
        camera.up = Vector3{ 0.0f, 1.0f, 0.0f }; // Reset roll
    }

    if (IsKeyPressed(KEY_FOUR)) {
        cameraMode = CAMERA_ORBITAL;
        camera.up = Vector3{ 0.0f, 1.0f, 0.0f }; // Reset roll
    }

    // Switch camera projection
    if (IsKeyPressed(KEY_P)) {
        if (camera.projection == CAMERA_PERSPECTIVE) {
            // Create isometric view
            cameraMode = CAMERA_THIRD_PERSON;
            // Note: The target distance is related to the render distance in the orthographic projection
            camera.position = Vector3{ 0.0f, 2.0f, -100.0f };
            camera.target = Vector3{ 0.0f, 2.0f, 0.0f };
            camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
            camera.projection = CAMERA_ORTHOGRAPHIC;
            camera.fovy = 20.0f; // near plane width in CAMERA_ORTHOGRAPHIC
            CameraYaw(&camera, -135 * DEG2RAD, true);
            CameraPitch(&camera, -45 * DEG2RAD, true, true, false);
        } else if (camera.projection == CAMERA_ORTHOGRAPHIC) {
            // Reset to default view
            cameraMode = CAMERA_THIRD_PERSON;
            camera.position = Vector3{ 0.0f, 2.0f, 10.0f };
            camera.target = Vector3{ 0.0f, 2.0f, 0.0f };
            camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
            camera.projection = CAMERA_PERSPECTIVE;
            camera.fovy = 60.0f;
        }
    }
    */
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

        // Draw player cube
        if (player.cameraMode == CAMERA_THIRD_PERSON) {
            DrawCube(player.position, 1.0f, 2.0f, 1.0f, RED);
        } 
        else if (player.cameraMode == CAMERA_FIRST_PERSON) {
            DrawCubeWires(player.position, 1.0f, 2.0f, 1.0f, RED);
        }

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