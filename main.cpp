#include <iostream>
#include <raylib.h>

int main() {

    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Stillness");

    // Define camera
    Camera3D camera;
    camera.position = { 0.0f, 2.5f, 5.0f }; // Camera position
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60);

    // Load custom shaders
    Shader shader = LoadShader("resources/shaders/default.vert", "resources/shaders/default.frag");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        BeginShaderMode(shader);
        DrawCube({0, 0, 0}, 2.0f, 2.0f, 2.0f, RED);
        EndShaderMode();

        EndMode3D();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}