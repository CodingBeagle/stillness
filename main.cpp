#include <iostream>
#include <memory>

#include <raylib.h>
#include <raymath.h>

#include "Camera.h"
#include "CubeMesh.h"
#include "MarchingCubes.h"

// Function to create a standard cube GridCell centered at the origin
GridCell CreateStandardCube(float size = 1.0f) {
    GridCell cell;

    // Define the 8 vertices of a cube
    // Vertex ordering follows standard convention for marching cubes
    // Bottom face vertices (y = -0.5)
    cell.vertices[0] = {-size/2, -size/2, -size/2}; // Bottom-left-back
    cell.vertices[1] = { size/2, -size/2, -size/2}; // Bottom-right-back
    cell.vertices[2] = { size/2, -size/2,  size/2}; // Bottom-right-front
    cell.vertices[3] = {-size/2, -size/2,  size/2}; // Bottom-left-front

    // Top face vertices (y = 0.5)
    cell.vertices[4] = {-size/2,  size/2, -size/2}; // Top-left-back
    cell.vertices[5] = { size/2,  size/2, -size/2}; // Top-right-back
    cell.vertices[6] = { size/2,  size/2,  size/2}; // Top-right-front
    cell.vertices[7] = {-size/2,  size/2,  size/2}; // Top-left-front

    // Initialize densities to 0.0
    // You'll need to set these values based on your specific application
    for (int i = 0; i < 8; i++) {
        cell.densities[i] = 1.0;
    }

    return cell;
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
    // When calling LoadShader, Raylib will automatically attempt to find the location of uniforms and inputs with standard names.
    // Such as "matModel", "matView", "mvp", etc.
    Shader shader = LoadShader("resources/shaders/default.vert", "resources/shaders/default.frag");

    // Get shader locations
    int lightPosLoc = GetShaderLocation(shader, "lightPos");

    // Generate our cube mesh
    Mesh cube = GenerateCubeMesh();

    // Create a material for our cube
    Material material = LoadMaterialDefault();
    material.shader = shader;
    material.maps[MATERIAL_MAP_DIFFUSE].color = RED;

    // Initialize marching cubes algorithm
    std::unique_ptr<MarchingCubes> marchingCubes = std::make_unique<MarchingCubes>();

    // Create a standard cube with size 2.0
    GridCell gridCell = CreateStandardCube(2.0f);

    // Set up some density values for the cube vertices
    // This will create a simple diagonal cut through the cube
    gridCell.densities[0] = 0.0;  // Inside the surface
    gridCell.densities[1] = 0.0;  // Inside the surface

    // Set the isolevel for surface extraction (adjust this to see different results)
    double isoLevel = 0.99;

    // Generate triangles using the marching cubes algorithm
    std::vector<Triangle> triangles = marchingCubes->Polygonise(gridCell, isoLevel);

    // Create a Raylib mesh from the triangles
    Mesh gridCellMesh = { 0 };

    if (!triangles.empty()) {
        // Count the total number of vertices
        int numVertices = triangles.size() * 3;

        // Allocate memory for the mesh
        gridCellMesh.vertexCount = numVertices;
        gridCellMesh.triangleCount = triangles.size();
        gridCellMesh.vertices = (float*)MemAlloc(numVertices * 3 * sizeof(float));
        gridCellMesh.normals = (float*)MemAlloc(numVertices * 3 * sizeof(float));

        // Fill the mesh data
        for (int i = 0; i < triangles.size(); i++) {
            // Vertices for this triangle
            Vector3 v1 = triangles[i].X;
            Vector3 v2 = triangles[i].Y;
            Vector3 v3 = triangles[i].Z;

            // Calculate normal (counter-clockwise winding)
            Vector3 normal = Vector3Normalize(Vector3CrossProduct(
                Vector3Subtract(v2, v1),
                Vector3Subtract(v3, v1)
            ));

            // Copy vertex positions
            gridCellMesh.vertices[i*9 + 0] = v1.x;
            gridCellMesh.vertices[i*9 + 1] = v1.y;
            gridCellMesh.vertices[i*9 + 2] = v1.z;

            gridCellMesh.vertices[i*9 + 3] = v2.x;
            gridCellMesh.vertices[i*9 + 4] = v2.y;
            gridCellMesh.vertices[i*9 + 5] = v2.z;

            gridCellMesh.vertices[i*9 + 6] = v3.x;
            gridCellMesh.vertices[i*9 + 7] = v3.y;
            gridCellMesh.vertices[i*9 + 8] = v3.z;

            // Use the same normal for all vertices in this triangle
            for (int j = 0; j < 3; j++) {
                gridCellMesh.normals[i*9 + j*3 + 0] = normal.x;
                gridCellMesh.normals[i*9 + j*3 + 1] = normal.y;
                gridCellMesh.normals[i*9 + j*3 + 2] = normal.z;
            }
        }

        // Upload mesh data to GPU
        UploadMesh(&gridCellMesh, false);
    }

    // Define light position in world space
    Vector3 lightPos = {50.0f, 25.0f, 20.0f};

    while (!WindowShouldClose()) {
        // Update camera
        camera.Update();

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

        // Draw the generated marching cubes mesh (if it exists)
        if (gridCellMesh.vertexCount > 0) {
            DrawMesh(gridCellMesh, material, modelMatrix);
        }

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
    // First, unload the meshes
    UnloadMesh(cube);
    if (gridCellMesh.vertexCount > 0) {
        UnloadMesh(gridCellMesh);
    }

    // Then unload material but don't unload the shader through the material
    // Create a copy of the material to unload, with shader set to NULL
    Material tempMaterial = material;
    tempMaterial.shader = {0}; // Clear the shader reference in the material
    UnloadMaterial(tempMaterial);

    // Finally, unload the shader separately
    UnloadShader(shader);

    CloseWindow();

    return 0;
}
