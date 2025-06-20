#version 330

// Input vertex attributes
in vec3 vertexPosition;
in vec3 vertexNormal;

// Input uniform values
// Raylib will attempt to find the location of uniforms by name when loading the shader.
uniform mat4 mvp; // Model-View-Projection matrix
uniform mat4 matModel;    // Model matrix

// Output to fragment shader
flat out vec3 surfaceNormal;  // Capitalization fixed to match fragment shader
out vec3 vertexPositionInWorldSpace;     // Add fragment position in world space

void main()
{
    // Calculate the normal matrix - using only the model matrix for normals
    // TODO: This step should be done on the CPU as the operation is computationally expensive.
    mat3 normalMatrix = transpose(inverse(mat3(matModel)));

    // Calculate the vertex normal using the normal matrix
    surfaceNormal = normalize(normalMatrix * vertexNormal);

    // Calculate and pass the vertex position in world space
    // This vertex position is important for flat shading in the fragment shader.
    // It is used to determine the direction between an interpolated point (fragment) on the surface and the light source.
    vertexPositionInWorldSpace = vec3(matModel * vec4(vertexPosition, 1.0));

    // Transform vertex position with MVP matrix
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}