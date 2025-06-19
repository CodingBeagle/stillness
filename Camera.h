#ifndef CAMERA_H
#define CAMERA_H

#include <raylib.h>
#include <raymath.h>

class GameCamera {
public:
    // Constructor
    GameCamera(float posX = 0.0f, float posY = 2.5f, float posZ = 5.0f);

    // Camera control methods
    void Update();  // Call this every frame to update camera position
    void SetPosition(Vector3 position);
    void SetTarget(Vector3 target);
    void Reset();   // Reset to initial position

    // Mouse and keyboard controls
    void UpdateMovement(float deltaTime);
    void UpdateRotation(Vector2 mouseDelta);
    void EnableMouseLook(bool enable);

    // Getter for the Raylib Camera3D
    const Camera3D& GetCamera() const { return camera; }

    // Configuration settings
    void SetMovementSpeed(float speed) { moveSpeed = speed; }
    void SetMouseSensitivity(float sensitivity) { mouseSensitivity = sensitivity; }

private:
    Camera3D camera;            // Internal Raylib camera
    bool cursorEnabled;         // Is cursor enabled
    float moveSpeed;            // Movement speed
    float mouseSensitivity;     // Mouse look sensitivity
    Vector2 previousMousePos;   // Previous mouse position for delta calculation
    Vector2 screenCenter;       // Center of the screen
};

#endif // CAMERA_H
