#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include <raymath.h>

class Player {
public:
    // These variables could changed to geters/setters or moved back into application class
    // Should be private
    Vector3 position = {0.0f, 0.0f, 0.0f};
    Vector3 size = {1.0f, 2.0f, 1.0f};

    float walkSpeed = 4.0f;
    float sprintSpeed = 9.0f;

    // Mouse
    float mouseSensitivity = 0.1f;
    float rotationSpeed = 90.0f; 
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

        // Handle rotation with keyboard input (arrow keys)
        if (IsKeyDown(KEY_RIGHT)) yaw -= rotationSpeed * deltaTime;  // Turn right
        if (IsKeyDown(KEY_LEFT)) yaw += rotationSpeed * deltaTime;   // Turn left
        if (IsKeyDown(KEY_UP)) pitch += rotationSpeed * deltaTime;   // Look up
        if (IsKeyDown(KEY_DOWN)) pitch -= rotationSpeed * deltaTime; // Look down

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

        // Handle player movement
        Vector3 movement = {0.0f, 0.0f, 0.0f};
        if (IsKeyDown(KEY_W)) movement = Vector3Add(movement, forward);
        if (IsKeyDown(KEY_S)) movement = Vector3Subtract(movement, forward);
        if (IsKeyDown(KEY_A)) movement = Vector3Subtract(movement, right);
        if (IsKeyDown(KEY_D)) movement = Vector3Add(movement, right);

        // Normalize movement
        if(Vector3Length(movement) > 0) movement = Vector3Normalize(movement);

        if(IsKeyDown(KEY_LEFT_SHIFT)) {
            position = Vector3Add(position, Vector3Scale(movement, sprintSpeed * deltaTime));
        } else {
            position = Vector3Add(position, Vector3Scale(movement, walkSpeed * deltaTime));
        }
        
        // Handle vertical movement
        if (IsKeyDown(KEY_E)) position.y += walkSpeed * deltaTime;
        if (IsKeyDown(KEY_Q)) position.y -= walkSpeed * deltaTime;

        // Update camera position and target
        camera.position = Vector3{position.x, position.y + 1.0f, position.z};
        camera.target = Vector3Add(camera.position, cameraDirection);
    }

    void Draw(){
        // Draw player cube
        if (cameraMode == CAMERA_THIRD_PERSON) {
            DrawCube(position, 1.0f, 2.0f, 1.0f, RED);
        } 
        else if (cameraMode == CAMERA_FIRST_PERSON) {
            DrawCubeWires(position, 1.0f, 2.0f, 1.0f, RED);
        }
    }
};

#endif