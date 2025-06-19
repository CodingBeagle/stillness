#include <iostream>
#include <raylib.h>
#include <rlgl.h>

#include "raymath.h"

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

    // Define camera
    Camera3D camera;
    camera.position = { 0.0f, 2.5f, 5.0f }; // Camera position
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Camera movement settings
    float cameraSpeed = 0.1f;
    float mouseSensitivity = 0.1f;
    Vector2 previousMousePosition = { 0 };
    Vector2 mousePosition = { 0 };

    // Define light position in world space
    Vector3 lightPos = { 3.0f, 2.0f, 3.0f };
    float lightMovementRadius = 4.0f;
    float lightMovementSpeed = 1.0f;

    // Disable cursor for camera look control
    DisableCursor();

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

    while (!WindowShouldClose()) {
        // Update camera position with WASD keys
        Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
        Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));

        // Scale movement based on frame time for consistent speed
        float frameTime = GetFrameTime();
        float moveSpeed = cameraSpeed * frameTime * 60.0f;

        // Forward/backward movement
        if (IsKeyDown(KEY_W)) {
            camera.position = Vector3Add(camera.position, Vector3Scale(forward, moveSpeed));
            camera.target = Vector3Add(camera.target, Vector3Scale(forward, moveSpeed));
        }
        if (IsKeyDown(KEY_S)) {
            camera.position = Vector3Subtract(camera.position, Vector3Scale(forward, moveSpeed));
            camera.target = Vector3Subtract(camera.target, Vector3Scale(forward, moveSpeed));
        }

        // Left/right movement
        if (IsKeyDown(KEY_A)) {
            camera.position = Vector3Subtract(camera.position, Vector3Scale(right, moveSpeed));
            camera.target = Vector3Subtract(camera.target, Vector3Scale(right, moveSpeed));
        }
        if (IsKeyDown(KEY_D)) {
            camera.position = Vector3Add(camera.position, Vector3Scale(right, moveSpeed));
            camera.target = Vector3Add(camera.target, Vector3Scale(right, moveSpeed));
        }

        // Mouse look control
        mousePosition = GetMousePosition();

        if (previousMousePosition.x != 0 || previousMousePosition.y != 0)
        {
            // Calculate mouse delta
            float deltaX = mousePosition.x - previousMousePosition.x;
            float deltaY = mousePosition.y - previousMousePosition.y;

            // Apply yaw rotation (horizontal look)
            if (deltaX != 0) {
                // Rotate around up vector
                Vector3 toTarget = Vector3Subtract(camera.target, camera.position);
                toTarget = Vector3RotateByAxisAngle(toTarget, camera.up, -deltaX * mouseSensitivity * DEG2RAD);
                camera.target = Vector3Add(camera.position, toTarget);
            }

            // Apply pitch rotation (vertical look)
            if (deltaY != 0) {
                // Rotate around right vector
                Vector3 toTarget = Vector3Subtract(camera.target, camera.position);
                Vector3 newTarget = Vector3RotateByAxisAngle(toTarget, right, -deltaY * mouseSensitivity * DEG2RAD);

                // Calculate angle to avoid over-rotation (prevent camera flipping)
                float angle = Vector3Angle(newTarget, camera.up);
                if (angle > 0.1f && angle < PI - 0.1f) {
                    camera.target = Vector3Add(camera.position, newTarget);
                }
            }
        }

        // Reset mouse position to center if it's at screen edge
        Vector2 screenCenter = { screenWidth/2.0f, screenHeight/2.0f };
        if (mousePosition.x < 50 || mousePosition.x > screenWidth - 50 ||
            mousePosition.y < 50 || mousePosition.y > screenHeight - 50) {
            SetMousePosition(screenCenter.x, screenCenter.y);
            mousePosition = screenCenter;
        }
        previousMousePosition = mousePosition;

        // Press SPACE to reset camera
        if (IsKeyPressed(KEY_SPACE)) {
            camera.position = { 0.0f, 2.5f, 5.0f };
            camera.target = { 0.0f, 0.0f, 0.0f };
            camera.up = { 0.0f, 1.0f, 0.0f };
        }

        // Toggle cursor visibility with ESCAPE key
        if (IsKeyPressed(KEY_ESCAPE)) {
            if (IsCursorHidden()) EnableCursor();
            else DisableCursor();
        }

        // Move light in a circular pattern (orbit around Y axis)
        float time = GetTime() * lightMovementSpeed;
        lightPos.x = cosf(time) * lightMovementRadius;
        lightPos.z = sinf(time) * lightMovementRadius;

        // Update light position uniform in shader
        SetShaderValue(shader, lightPosLoc, &lightPos, SHADER_UNIFORM_VEC3);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        // Create model matrix for the cube (identity matrix for now)
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