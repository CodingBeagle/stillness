//
// Created by katte on 20/06/2025.
//

#include "MarchingCubes.h"

std::vector<Triangle> MarchingCubes::Polygonise(const GridCell &gridCell, double isoLevel) const {
    // Determine the index into the edge table which
    // tells us which vertices are inside of the surface
    int cubeIndex = 0;

    if (gridCell.densities[0] < isoLevel) {
        cubeIndex |= 1;
    }

    if (gridCell.densities[1] < isoLevel) {
        cubeIndex |= 2;
    }

    if (gridCell.densities[2] < isoLevel) {
        cubeIndex |= 4;
    }

    if (gridCell.densities[3] < isoLevel) {
        cubeIndex |= 8;
    }

    if (gridCell.densities[4] < isoLevel) {
        cubeIndex |= 16;
    }

    if (gridCell.densities[5] < isoLevel) {
        cubeIndex |= 32;
    }

    if (gridCell.densities[6] < isoLevel) {
        cubeIndex |= 64;
    }

    if (gridCell.densities[7] < isoLevel) {
        cubeIndex |= 128;
    }

    // If the cube is entirely in/out of the surface, then no triangles will be created
    if (cubeIndex == 0) {
        return {};
    }

    // Looking up the cube index in the edge table will give you a 12 bit index.
    // Each bit that is a 1 represents an edge that the isosurface intersects.
    // 0 if the edge isn't cut by the isosurface.
    int edgeTableResult = edgeTable[cubeIndex];

    // Find the vertices where the surface intersects the edges of the cube
    std::array<Vector3, 12> edgeVertices;

    if (edgeTableResult & 1) {
        edgeVertices[0] = VertexInterpolate(isoLevel, gridCell.vertices[0], gridCell.vertices[1], gridCell.densities[0], gridCell.densities[1]);
    }

    if (edgeTableResult & 2) {
        edgeVertices[1] = VertexInterpolate(isoLevel, gridCell.vertices[1], gridCell.vertices[2], gridCell.densities[1], gridCell.densities[2]);
    }

    if (edgeTableResult & 4) {
        edgeVertices[2] = VertexInterpolate(isoLevel, gridCell.vertices[2], gridCell.vertices[3], gridCell.densities[2], gridCell.densities[3]);
    }

    if (edgeTableResult & 8) {
        edgeVertices[3] = VertexInterpolate(isoLevel, gridCell.vertices[3], gridCell.vertices[0], gridCell.densities[3], gridCell.densities[0]);
    }

    if (edgeTableResult & 16) {
        edgeVertices[4] = VertexInterpolate(isoLevel, gridCell.vertices[4], gridCell.vertices[5], gridCell.densities[4], gridCell.densities[5]);
    }

    if (edgeTableResult & 32) {
        edgeVertices[5] = VertexInterpolate(isoLevel, gridCell.vertices[5], gridCell.vertices[6], gridCell.densities[5], gridCell.densities[6]);
    }

    if (edgeTableResult & 64) {
        edgeVertices[6] = VertexInterpolate(isoLevel, gridCell.vertices[6], gridCell.vertices[7], gridCell.densities[6], gridCell.densities[7]);
    }

    if (edgeTableResult & 128) {
        edgeVertices[7] = VertexInterpolate(isoLevel, gridCell.vertices[7], gridCell.vertices[4], gridCell.densities[7], gridCell.densities[4]);
    }

    if (edgeTableResult & 256) {
        edgeVertices[8] = VertexInterpolate(isoLevel, gridCell.vertices[0], gridCell.vertices[4], gridCell.densities[0], gridCell.densities[4]);
    }

    if (edgeTableResult & 512) {
        edgeVertices[9] = VertexInterpolate(isoLevel, gridCell.vertices[1], gridCell.vertices[5], gridCell.densities[1], gridCell.densities[5]);
    }

    if (edgeTableResult & 1024) {
        edgeVertices[10] = VertexInterpolate(isoLevel, gridCell.vertices[2], gridCell.vertices[6], gridCell.densities[2], gridCell.densities[6]);
    }

    if (edgeTableResult & 2048) {
        edgeVertices[11] = VertexInterpolate(isoLevel, gridCell.vertices[3], gridCell.vertices[7], gridCell.densities[3], gridCell.densities[7]);
    }

    // Create the triangle
    std::vector<Triangle> triangles {};
    for (int i = 0; triTable[cubeIndex][i] != -1; i += 3) {
        Triangle triangle {};
        triangle.X = edgeVertices[triTable[cubeIndex][i    ]];
        triangle.Y = edgeVertices[triTable[cubeIndex][i + 1]];
        triangle.Z = edgeVertices[triTable[cubeIndex][i + 2]];
        triangles.push_back(triangle);
    }

    return triangles;
}

Vector3 MarchingCubes::VertexInterpolate(double isoLevel, Vector3 p1, Vector3 p2, double valp1, double valp2) {
    if (std::abs(isoLevel - valp1) < 0.00001) {
        return p1;
    }

    if (std::abs(isoLevel - valp2) < 0.00001) {
        return p2;
    }

    if (std::abs(valp1 - valp2) < 0.00001) {
        return p1;
    }

    double mu = (isoLevel - valp1) / (valp2 - valp1);

    Vector3 p {};
    p.x = p1.x + mu * (p2.x - p1.x);
    p.y = p1.y + mu * (p2.y - p1.y);
    p.z = p1.z + mu * (p2.z - p1.z);

    return p;
}
