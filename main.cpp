#include <iostream>
#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include "Camera.h"

// Function to generate a cube mesh with vertex positions and normals
Mesh GenerateCubeMesh()
{
    Mesh mesh = { 0 };

    mesh.triangleCount = 12; // 6 faces, 2 triangles per face
    mesh.vertexCount = 24;   // 4 vertices per face, 6 faces (duplicated vertices for flat shading)

    // Allocate memory for vertex positions (3 floats per vertex: x, y, z)
    mesh.vertices = (float *)MemAlloc(mesh.vertexCount * 3 * sizeof(float));

    // Allocate memory for normals (3 floats per vertex: nx, ny, nz)
    mesh.normals = (float *)MemAlloc(mesh.vertexCount * 3 * sizeof(float));

    // Allocate memory for indices (3 indices per triangle)
    mesh.indices = (unsigned short *)MemAlloc(mesh.triangleCount * 3 * sizeof(unsigned short));

    // Cube of size 1.0f positioned at origin (0, 0, 0)
    // Vertex positions organized per face for clarity
    // Each face has 4 vertices (to support flat shading, vertices are duplicated)
    // Format: x, y, z for each vertex
    float vertices[] = {
        // Front face (+z)
        -0.5f, -0.5f,  0.5f,  // bottom-left
         0.5f, -0.5f,  0.5f,  // bottom-right
         0.5f,  0.5f,  0.5f,  // top-right
        -0.5f,  0.5f,  0.5f,  // top-left

        // Back face (-z)
        -0.5f, -0.5f, -0.5f,  // bottom-left
        -0.5f,  0.5f, -0.5f,  // top-left
         0.5f,  0.5f, -0.5f,  // top-right
         0.5f, -0.5f, -0.5f,  // bottom-right

        // Top face (+y)
        -0.5f,  0.5f, -0.5f,  // back-left
        -0.5f,  0.5f,  0.5f,  // front-left
         0.5f,  0.5f,  0.5f,  // front-right
         0.5f,  0.5f, -0.5f,  // back-right

        // Bottom face (-y)
        -0.5f, -0.5f, -0.5f,  // back-left
         0.5f, -0.5f, -0.5f,  // back-right
         0.5f, -0.5f,  0.5f,  // front-right
        -0.5f, -0.5f,  0.5f,  // front-left

        // Right face (+x)
         0.5f, -0.5f,  0.5f,  // front-bottom
         0.5f, -0.5f, -0.5f,  // back-bottom
         0.5f,  0.5f, -0.5f,  // back-top
         0.5f,  0.5f,  0.5f,  // front-top

        // Left face (-x)
        -0.5f, -0.5f, -0.5f,  // back-bottom
        -0.5f, -0.5f,  0.5f,  // front-bottom
        -0.5f,  0.5f,  0.5f,  // front-top
        -0.5f,  0.5f, -0.5f   // back-top
    };

    // Define the normals for each face
    float normals[] = {
        // Front face normals (all pointing in +z direction)
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        // Back face normals (all pointing in -z direction)
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        // Top face normals (all pointing in +y direction)
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        // Bottom face normals (all pointing in -y direction)
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

        // Right face normals (all pointing in +x direction)
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        // Left face normals (all pointing in -x direction)
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f
    };

    // Define indices for the triangles
    // Each face has 2 triangles, each triangle has 3 indices
    unsigned short indices[] = {
        // Front face
        0, 1, 2,  0, 2, 3,
        // Back face
        4, 5, 6,  4, 6, 7,
        // Top face
        8, 9, 10,  8, 10, 11,
        // Bottom face
        12, 13, 14,  12, 14, 15,
        // Right face
        16, 17, 18,  16, 18, 19,
        // Left face
        20, 21, 22,  20, 22, 23
    };

    // Copy vertex positions, normals, and indices to the mesh
    memcpy(mesh.vertices, vertices, mesh.vertexCount * 3 * sizeof(float));
    memcpy(mesh.normals, normals, mesh.vertexCount * 3 * sizeof(float));
    memcpy(mesh.indices, indices, mesh.triangleCount * 3 * sizeof(unsigned short));

    // Upload mesh data to GPU
    UploadMesh(&mesh, false);

    return mesh;
}

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
        DrawGrid(10, 1.0f);

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