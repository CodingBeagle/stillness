#version 330

// Input
flat in vec3 surfaceNormal;
in vec3 vertexPositionInWorldSpace;

// Uniforms
uniform vec3 lightPos;

// Output
out vec4 FragColor;

void main()
{
    // Hardcoded base color (light blue)
    // #TODO: Make it a uniform input instead of hardcoded value
    vec3 baseColor = vec3(0.2, 0.4, 0.8);

    // Calculate light direction from fragment to light (dynamic)
    vec3 lightDir = normalize(lightPos - vertexPositionInWorldSpace);

    // Simple diffuse lighting calculation
    // Diffuse lighting is based on the angle between the light direction and the surface normal
    // It represents a surface reflection that scatters incoming light equally in all directions.
    // Surfaces with diffuse reflection appear matte and do not have a shiny appearance.
    // The dot product between the surface normal and the light direction can be understood as follows:
    // N * L = cos(theta)
    // where N is the surface normal, L is the light direction, and theta is the angle between them.
    // When the surface normal and light direction point in the same direction, the dot product is 1 (maximum brightness). Theta is 0 degrees.
    // When they are perpendicular, the dot product is 0 (no light contribution). Theta is 90 degrees.
    // When they point in opposite directions, the dot product is < 0 and no light should be applied.
    // max is used to do the clamping, ensuring that negative values are set to 0.
    float diffuse = max(dot(normalize(surfaceNormal), lightDir), 0.0);

    // Add ambient light to prevent fully black areas
    // That is, in case diffuse lightning is 0, we still want to see the object with ambient light.
    float ambient = 0.3;

    // Final color calculation with lighting
    vec3 finalColor = baseColor * (ambient + diffuse);

    // Output color
    FragColor = vec4(finalColor, 1.0);
}