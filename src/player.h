#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include <raymath.h>
#include "physics_body.h"

class Player { // no private variables
public:
    // Physics world pointer
    btDiscreteDynamicsWorld* world;

    // Camera
    Camera camera = { 0 };
    int cameraMode = CAMERA_FIRST_PERSON;

    // Collisions
    btCollisionShape* playerShape = new btCapsuleShape(0.5f, 2.0f);
    PhysicsBody* playerBody = nullptr;

    Vector3 position = {0.0f, 0.0f, 0.0f};

    // speed
    float walkSpeed = 600.0f;
    float sprintSpeed = 900.0f;

    // jump
    float jumpStrength = 1000.0f;  // Jump force strength
    bool isJumping = false;

    // Mouse
    float mouseSensitivity = 0.1f;
    float rotationSpeed = 90.0f;
    float yaw = 0.0f;
    float pitch = 0.0f;

    Player(){}
    ~Player() {
        delete playerShape;
        delete playerBody;
    }

    void Init(btDiscreteDynamicsWorld* dynamicsWorld) {
        world = dynamicsWorld;
        playerBody = new PhysicsBody(world, playerShape, btVector3{1.0f, 3.0f, 1.0f}, 50.0f);
        position = playerBody->GetPosition();

        playerBody->body->setActivationState(DISABLE_DEACTIVATION);
        playerBody->body->setAngularFactor(btVector3(0, 1, 0)); // make so that player shape collision shape will not fall on its side
        playerBody->body->setDamping(0.0f, 0.0f);

        camera.target = Vector3{0.0f, 0.0f, 0.0f};
        camera.position = Vector3{ 0.0f, 2.8f, 0.0f };
        camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
        camera.fovy = 60.0f;                         
        camera.projection = CAMERA_PERSPECTIVE;
    }

    void Player::Update(float deltaTime) {
        position = playerBody->GetPosition();

        // Get mouse input for camera rotation
        Vector2 mouseDelta = GetMouseDelta();
        yaw -= mouseDelta.x * mouseSensitivity;
        pitch -= mouseDelta.y * mouseSensitivity;

        //
        // Handle camera up/down rotation based on key input (arrows or Q/E)
        if (IsKeyDown(KEY_UP)) pitch += rotationSpeed * deltaTime;  // Look up
        if (IsKeyDown(KEY_DOWN)) pitch -= rotationSpeed * deltaTime;  // Look down
        if (IsKeyDown(KEY_LEFT)) yaw += rotationSpeed * deltaTime;
        if (IsKeyDown(KEY_RIGHT)) yaw -= rotationSpeed * deltaTime;

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

        // Handle movement based on keyboard input
        Vector3 movement = {0.0f, 0.0f, 0.0f};
        if (IsKeyDown(KEY_W)) movement = Vector3Add(movement, forward);
        if (IsKeyDown(KEY_S)) movement = Vector3Subtract(movement, forward);
        if (IsKeyDown(KEY_A)) movement = Vector3Subtract(movement, right);
        if (IsKeyDown(KEY_D)) movement = Vector3Add(movement, right);

        // Normalize movement vector if necessary
        if (Vector3Length(movement) > 0) movement = Vector3Normalize(movement);

        // Apply movement based on the sprint or walk speed
        if (IsKeyDown(KEY_LEFT_SHIFT)) {
            playerBody->ApplyMovement(movement, sprintSpeed * deltaTime);
        } else {
            playerBody->ApplyMovement(movement, walkSpeed * deltaTime);
        }

        // bool isOnGround = false;
        // world->rayTest(playerBody->body->GetPosition(), rayEnd, rayCallback);
        // if (rayCallback.hasHit()) {
        //     isOnGround = true;
        // } else {
        //     isOnGround = false;
        // }

        if(!isJumping && IsKeyDown(KEY_SPACE)) {
            btVector3 jumpForce(0.0f, jumpStrength, 0.0f);
            //playerBody->body->applyCentralForce(jumpForce);
            playerBody->body->applyImpulse(jumpForce, btVector3(0.0f, 0.0f, 0.0f));
            isJumping = true;
        } else {
            isJumping = false;
        }

        // Update camera position and target
        camera.position = Vector3{position.x, position.y + 1.0f, position.z};
        camera.target = Vector3Add(camera.position, cameraDirection);
    }

    void Draw() {
        // Draw player cube
        if (cameraMode == CAMERA_THIRD_PERSON) {
            DrawCube(position, 1.0f, 2.0f, 1.0f, RED);
        } 
        else if (cameraMode == CAMERA_FIRST_PERSON) {
            // DrawCubeWires(position, 1.0f, 2.0f, 1.0f, RED);
        }
    }
};

#endif