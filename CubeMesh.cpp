#include "CubeMesh.h"
#include <raylib.h>
#include <rlgl.h>
#include <string.h> // For memcpy

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
