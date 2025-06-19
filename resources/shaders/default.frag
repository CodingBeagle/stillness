#version 330

// Input
flat in vec3 FragNormal;
in vec3 FragPosition;  // Fragment position in world space

// Uniforms
uniform vec3 lightPos; // Light position in world space

// Output
out vec4 FragColor;

void main()
{
    // Hardcoded base color (light blue)
    vec3 baseColor = vec3(0.2, 0.4, 0.8);

    // Calculate light direction from fragment to light (dynamic)
    vec3 lightDir = normalize(lightPos - FragPosition);

    // Simple diffuse lighting calculation
    float diffuse = max(dot(normalize(FragNormal), lightDir), 0.0);

    // Add ambient light to prevent fully black areas
    float ambient = 0.3;

    // Final color calculation with lighting
    vec3 finalColor = baseColor * (ambient + diffuse);

    // Output color
    FragColor = vec4(finalColor, 1.0);
}