#pragma once

#include "object.h"

// Forward Declarations:
struct Camera;

void DefaultCameraInit(Camera* camera);
void NoClipCameraUpdate(Camera* camera, const double deltaTime, const double aspectRatio);


typedef struct Camera {
    OBJECT_BODY;
    Quaternion Rotation;
    Matrix ViewMatrix;
    
    float MoveSpeed = 1.0f;
    float Acceleration = 0.4f;
    float Fov = 60.0f;
    float NearClip = 0.001f;
    float FarClip = 1024.0f;
    float Sensitivity = 1.0f;

} Camera;


Object_Camera_create(Camera* camera) {
    OBJECT_CREATE_BODY()

}
