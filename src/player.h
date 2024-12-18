#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include <raymath.h>

class Player {
public:
    // These variables could changed to geters/setters or moved back into application class

    Vector3 position = {0.0f, 0.0f, 0.0f};
    Vector3 size = {1.0f, 2.0f, 1.0f};
    const float speed = 5.0f;
    // Mouse
    float mouseSensitivity = 0.1f;
    float yaw = 0.0f;
    float pitch = 0.0f;
    // Camera
    Camera camera = { 0 };
    int cameraMode = CAMERA_FIRST_PERSON;
    // =================================================

    void Init() {
        position = {0.0f, 1.0f, 2.0f};

        camera.target = Vector3{0.0f, 0.0f, 0.0f};
        camera.position = Vector3{ 0.0f, 2.0f, 0.0f };
        camera.up = Vector3{ 0.0f, 1.0f, 0.0f };    
        camera.fovy = 60.0f;                         
        camera.projection = CAMERA_PERSPECTIVE;
    }

    void Update(float deltaTime) {
        // Get mouse input
        Vector2 mouseDelta = GetMouseDelta();

        yaw -= mouseDelta.x * mouseSensitivity;
        pitch -= mouseDelta.y * mouseSensitivity;

        // Clamp pitch to avoid flipping the camera
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        // Calculate direction vectors from yaw and pitch
        Vector3 cameraDirection = {
            cosf(DEG2RAD * pitch) * sinf(DEG2RAD * yaw),
            sinf(DEG2RAD * pitch),
            cosf(DEG2RAD * pitch) * cosf(DEG2RAD * yaw)
        };

        Vector3 forward = {
            cosf(DEG2RAD * pitch) * sinf(DEG2RAD * yaw),
            0.0f, // restrict y direction
            cosf(DEG2RAD * pitch) * cosf(DEG2RAD * yaw)
        };

        Vector3 right = Vector3CrossProduct(forward, Vector3{0.0f, 1.0f, 0.0f});

        // Handle player movement in horizontal plane only (XZ)
        if (IsKeyDown(KEY_W)) position = Vector3Add(position, Vector3Scale(forward, speed * deltaTime)); // Move forward
        if (IsKeyDown(KEY_S)) position = Vector3Subtract(position, Vector3Scale(forward, speed * deltaTime)); // Move backward
        if (IsKeyDown(KEY_A)) position = Vector3Subtract(position, Vector3Scale(right, speed * deltaTime)); // Strafe left
        if (IsKeyDown(KEY_D)) position = Vector3Add(position, Vector3Scale(right, speed * deltaTime)); // Strafe right

        // Handle vertical movement
        if (IsKeyDown(KEY_SPACE)) position.y += speed * deltaTime; // Move up
        if (IsKeyDown(KEY_LEFT_SHIFT)) position.y -= speed * deltaTime; // Move down

        // Update camera position and target
        // camera.position = position;
        camera.position = Vector3{position.x, position.y + 1.0f, position.z};
        camera.target = Vector3Add(camera.position, cameraDirection);

    }

    void Draw(){

    }
};

#endif