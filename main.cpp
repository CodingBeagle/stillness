#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include "Camera.h"
#include "CubeMesh.h"

int main() {
    const int screenWidth = 2560;
    const int screenHeight = 1440;

    InitWindow(screenWidth, screenHeight, "Stillness");

    // Create our camera
    GameCamera camera(0.0f, 2.5f, 5.0f);
    camera.SetMovementSpeed(0.1f);
    camera.SetMouseSensitivity(0.1f);

    SetTargetFPS(120);

    // Load custom shaders
    Shader shader = LoadShader("resources/shaders/default.vert", "resources/shaders/default.frag");

    // Get shader locations
    int lightPosLoc = GetShaderLocation(shader, "lightPos");

    // Generate our cube mesh
    Mesh cube = GenerateCubeMesh();

    // Create a material for our cube
    Material material = LoadMaterialDefault();
    material.shader = shader;
    material.maps[MATERIAL_MAP_DIFFUSE].color = RED;

    // Define light position in world space
    Vector3 lightPos = { 3.0f, 2.0f, 3.0f };
    float lightMovementRadius = 4.0f;
    float lightMovementSpeed = 1.0f;

    while (!WindowShouldClose()) {
        // Update camera
        camera.Update();

        // Move light in a circular pattern (orbit around Y axis)
        float time = GetTime() * lightMovementSpeed;
        lightPos.x = cosf(time) * lightMovementRadius;
        lightPos.z = sinf(time) * lightMovementRadius;

        // Update light position uniform in shader
        SetShaderValue(shader, lightPosLoc, &lightPos, SHADER_UNIFORM_VEC3);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera.GetCamera());

        // Create model matrix for the cube
        Matrix modelMatrix = MatrixIdentity();

        // Set the model matrix uniform for proper normal transformation
        int modelLoc = GetShaderLocation(shader, "matModel");
        SetShaderValueMatrix(shader, modelLoc, modelMatrix);

        // Draw the cube using our custom mesh and material
        DrawMesh(cube, material, modelMatrix);

        // Draw a grid to help with orientation
        DrawGrid(100, 1.0f);

        // Draw a small sphere at the light position for visual reference
        DrawSphere(lightPos, 0.2f, YELLOW);

        EndMode3D();

        // Display controls and light info
        DrawText("WASD to move, Mouse to look", 10, 10, 20, BLACK);
        DrawText("SPACE to reset camera, ESC to toggle cursor", 10, 40, 20, BLACK);
        DrawText(TextFormat("Light position: %.2f, %.2f, %.2f", lightPos.x, lightPos.y, lightPos.z), 10, 70, 20, BLACK);

        // Display FPS counter in the top-right corner
        DrawFPS(screenWidth - 100, 10);

        EndDrawing();
    }

    // Unload resources - fix the order of deallocation
    // First, unload the mesh
    UnloadMesh(cube);

    // Then unload material but don't unload the shader through the material
    // Create a copy of the material to unload, with shader set to NULL
    Material tempMaterial = material;
    tempMaterial.shader = { 0 }; // Clear the shader reference in the material
    UnloadMaterial(tempMaterial);

    // Finally, unload the shader separately
    UnloadShader(shader);

    CloseWindow();

    return 0;
}