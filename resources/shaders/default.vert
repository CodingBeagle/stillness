#version 330

// Input vertex attributes
in vec3 vertexPosition;
in vec3 vertexNormal;

// Input uniform values
uniform mat4 mvp;
uniform mat4 matModel;    // Model matrix for correct normal transformation

// Output to fragment shader
flat out vec3 FragNormal;  // Capitalization fixed to match fragment shader
out vec3 FragPosition;     // Add fragment position in world space

void main()
{
    // Calculate the normal matrix - using only the model matrix for normals
    mat3 normalMatrix = transpose(inverse(mat3(matModel)));

    // Calculate the vertex normal with proper transformation
    vec3 normal = normalize(normalMatrix * vertexNormal);

    // Pass the normal to the fragment shader
    FragNormal = normal;

    // Calculate and pass the fragment position in world space
    FragPosition = vec3(matModel * vec4(vertexPosition, 1.0));

    // Transform vertex position with MVP matrix
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}