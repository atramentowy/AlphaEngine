#include "raylib.h"

int main(void) {
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Basic raylib Project");

    SetTargetFPS(60);  // Set FPS to 60

    // Main game loop
    while (!WindowShouldClose()) {
        // Update

        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);  // Clear screen with white color

        DrawText("Hello, raylib!", 190, 200, 20, LIGHTGRAY);
        DrawCircle(screenWidth / 2, screenHeight / 2, 50, RED);  // Draw a circle

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();  // Close the window and release resources

    return 0;
}