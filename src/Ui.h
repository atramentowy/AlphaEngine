#ifndef UI_H
#define UI_H

#include "Player.h"
#include <raylib.h>

Vector2 GetScreenCenter() {
    return {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
}

void DrawCrosshair() {
    Vector2 center = GetScreenCenter();

    float crosshairSize = 8.0f;

    DrawLineEx(Vector2{center.x - crosshairSize, center.y}, 
            Vector2{center.x + crosshairSize, center.y}, 
            1.5f, RED);
    DrawLineEx(Vector2{center.x, center.y - crosshairSize}, 
            Vector2{center.x, center.y + crosshairSize}, 
            1.5f, RED);
}

void DrawInfo(Player* player) {
    DrawRectangle(0, 0, 195, 120, Fade(BLACK, 0.5f));

    DrawText("Player status:", 10, 5, 10, RED);
    DrawText(TextFormat("- position: (%06.3f, %06.3f, %06.3f)", player->position.x, player->position.y, player->position.z), 10, 15, 10, WHITE);
    DrawText(TextFormat("- yaw/pitch: (%06.3f, %06.3f)", player->yaw, player->pitch), 10, 25, 10, WHITE);

    DrawText("Camera status:", 10, 35, 10, RED);
    DrawText(TextFormat("- mode: %s", 
        (player->cameraMode == CAMERA_FREE) ? "FREE" :
        (player->cameraMode == CAMERA_FIRST_PERSON) ? "FIRST_PERSON" :
        (player->cameraMode == CAMERA_THIRD_PERSON) ? "THIRD_PERSON" :
        (player->cameraMode == CAMERA_ORBITAL) ? "ORBITAL" : "CUSTOM"), 
        10, 45, 10, WHITE
    );
    DrawText(TextFormat("- projection: %s", 
        (player->camera.projection == CAMERA_PERSPECTIVE) ? "PERSPECTIVE" :
        (player->camera.projection == CAMERA_ORTHOGRAPHIC) ? "ORTHOGRAPHIC" : "CUSTOM"),
        10, 55, 10, WHITE
    );
    DrawText(TextFormat("- position: (%06.3f, %06.3f, %06.3f)", player->camera.position.x, player->camera.position.y, player->camera.position.z), 10, 65, 10, WHITE);
    DrawText(TextFormat("- target: (%06.3f, %06.3f, %06.3f)", player->camera.target.x, player->camera.target.y, player->camera.target.z), 10, 75, 10, WHITE);
    DrawText(TextFormat("Fps: %i", GetFPS()), 10, 85, 10, WHITE);

    /*
    DrawRectangle(0, 0, 195, 120, Fade(BLACK, 0.5f));

    DrawText("Player status:", 10, 5, 10, RED);
    DrawText(TextFormat("- position: (%06.3f, %06.3f, %06.3f)", player.entity.position.x, player.entity.position.y, player.entity.position.z), 10, 15, 10, WHITE);
    DrawText(TextFormat("- yaw/pitch: (%06.3f, %06.3f)", player.yaw, player.pitch), 10, 25, 10, WHITE);

    DrawText("Camera status:", 10, 35, 10, RED);
    DrawText(TextFormat("- mode: %s", 
        (player.cameraMode == CAMERA_FREE) ? "FREE" :
        (player.cameraMode == CAMERA_FIRST_PERSON) ? "FIRST_PERSON" :
        (player.cameraMode == CAMERA_THIRD_PERSON) ? "THIRD_PERSON" :
        (player.cameraMode == CAMERA_ORBITAL) ? "ORBITAL" : "CUSTOM"), 
        10, 45, 10, WHITE
    );
    DrawText(TextFormat("- projection: %s", 
        (player.camera.projection == CAMERA_PERSPECTIVE) ? "PERSPECTIVE" :
        (player.camera.projection == CAMERA_ORTHOGRAPHIC) ? "ORTHOGRAPHIC" : "CUSTOM"),
        10, 55, 10, WHITE
    );
    DrawText(TextFormat("- position: (%06.3f, %06.3f, %06.3f)", player.camera.position.x, player.camera.position.y, player.camera.position.z), 10, 65, 10, WHITE);
    DrawText(TextFormat("- target: (%06.3f, %06.3f, %06.3f)", player.camera.target.x, player.camera.target.y, player.camera.target.z), 10, 75, 10, WHITE);
    DrawText(TextFormat("Fps: %i", GetFPS()), 10, 85, 10, WHITE);
    */
}

#endif