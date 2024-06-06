#pragma once

#include "raylib.h"
#include "rcamera.h"


void UpdateCameraControl(Camera3D& camera, const Vector3 default_camera_position)
{     
    if (IsCursorHidden())
    {
        const Vector2 mouseDelta = GetMouseDelta();
        constexpr float camera_rotate_speed = 0.1f;
        CameraYaw(&camera, GetFrameTime() * mouseDelta.x * -camera_rotate_speed, true);
    }

    // Toggle camera controls
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        if (IsCursorHidden())EnableCursor();
        else DisableCursor();
    }

    const float mouseZoomDelta = GetMouseWheelMove();
    CameraMoveToTarget(&camera,-mouseZoomDelta);

    if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
    {
        camera.target = Vector3{0.0f, 0.0f, 0.0f};
        camera.position = default_camera_position;
    }
}