#include "Camera.h"
#include <raylib.h>
#include <raymath.h>

GameCamera::GameCamera(float posX, float posY, float posZ) {
    // Initialize camera
    camera.position = { posX, posY, posZ };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // Initialize control variables
    moveSpeed = 0.1f;
    mouseSensitivity = 0.1f;
    cursorEnabled = false;

    // Calculate screen center
    screenCenter = { (float)GetScreenWidth()/2.0f, (float)GetScreenHeight()/2.0f };

    // Initialize mouse position to screen center to avoid initial camera jump
    previousMousePos = screenCenter;
    SetMousePosition(screenCenter.x, screenCenter.y);

    // Disable cursor by default for camera look control
    DisableCursor();

    // Force an initial reset to ensure correct positioning
    Reset();
}

void GameCamera::Update() {
    // Get frame time for smooth movement
    float frameTime = GetFrameTime();

    // Update movement based on keyboard input
    UpdateMovement(frameTime);

    // Only update rotation if cursor is disabled
    if (!cursorEnabled) {
        Vector2 mousePosition = GetMousePosition();
        Vector2 mouseDelta = {
            mousePosition.x - previousMousePos.x,
            mousePosition.y - previousMousePos.y
        };

        // Update camera rotation based on mouse movement
        UpdateRotation(mouseDelta);

        // Reset mouse position to center if it's at screen edge
        if (mousePosition.x < 50 || mousePosition.x > GetScreenWidth() - 50 ||
            mousePosition.y < 50 || mousePosition.y > GetScreenHeight() - 50) {
            SetMousePosition(screenCenter.x, screenCenter.y);
            mousePosition = screenCenter;
        }
        previousMousePos = mousePosition;
    }

    // Handle cursor toggle with ESC key
    if (IsKeyPressed(KEY_ESCAPE)) {
        EnableMouseLook(!cursorEnabled);
    }

    // Reset camera with SPACE key
    if (IsKeyPressed(KEY_SPACE)) {
        Reset();
    }
}

void GameCamera::UpdateMovement(float deltaTime) {
    // Calculate forward and right vectors
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));

    // Calculate movement speed scaled by frame time
    float speed = moveSpeed * deltaTime * 60.0f;

    // Forward/backward movement
    if (IsKeyDown(KEY_W)) {
        camera.position = Vector3Add(camera.position, Vector3Scale(forward, speed));
        camera.target = Vector3Add(camera.target, Vector3Scale(forward, speed));
    }
    if (IsKeyDown(KEY_S)) {
        camera.position = Vector3Subtract(camera.position, Vector3Scale(forward, speed));
        camera.target = Vector3Subtract(camera.target, Vector3Scale(forward, speed));
    }

    // Left/right movement
    if (IsKeyDown(KEY_A)) {
        camera.position = Vector3Subtract(camera.position, Vector3Scale(right, speed));
        camera.target = Vector3Subtract(camera.target, Vector3Scale(right, speed));
    }
    if (IsKeyDown(KEY_D)) {
        camera.position = Vector3Add(camera.position, Vector3Scale(right, speed));
        camera.target = Vector3Add(camera.target, Vector3Scale(right, speed));
    }
}

void GameCamera::UpdateRotation(Vector2 mouseDelta) {
    if (mouseDelta.x != 0 || mouseDelta.y != 0) {
        // Get direction vector from camera position to target
        Vector3 toTarget = Vector3Subtract(camera.target, camera.position);

        // Get right vector for vertical rotation
        Vector3 right = Vector3Normalize(Vector3CrossProduct(toTarget, camera.up));

        // Apply yaw rotation (horizontal look)
        if (mouseDelta.x != 0) {
            toTarget = Vector3RotateByAxisAngle(toTarget, camera.up, -mouseDelta.x * mouseSensitivity * DEG2RAD);
        }

        // Apply pitch rotation (vertical look)
        if (mouseDelta.y != 0) {
            Vector3 newTarget = Vector3RotateByAxisAngle(toTarget, right, -mouseDelta.y * mouseSensitivity * DEG2RAD);

            // Calculate angle to avoid over-rotation (prevent camera flipping)
            float angle = Vector3Angle(newTarget, camera.up);
            if (angle > 0.1f && angle < PI - 0.1f) {
                toTarget = newTarget;
            }
        }

        // Update camera target based on rotations
        camera.target = Vector3Add(camera.position, toTarget);
    }
}

void GameCamera::EnableMouseLook(bool enable) {
    cursorEnabled = !enable;
    if (enable) {
        DisableCursor();
        previousMousePos = GetMousePosition();
    } else {
        EnableCursor();
    }
}

void GameCamera::SetPosition(Vector3 position) {
    // Calculate the offset between current position and target
    Vector3 offset = Vector3Subtract(camera.target, camera.position);

    // Set the new position
    camera.position = position;

    // Update target to maintain the same offset
    camera.target = Vector3Add(position, offset);
}

void GameCamera::SetTarget(Vector3 target) {
    camera.target = target;
}

void GameCamera::Reset() {
    camera.position = { 0.0f, 2.5f, 5.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
}
