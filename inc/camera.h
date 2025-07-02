#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "object.h"
#include "gl_math.h"

typedef struct Camera {
    OBJECT_BODY();
    mat4 ViewMatrix;
    
    float MoveSpeed;
    float Acceleration;
    float Fov;
    float NearClip;
    float FarClip;
    float Sensitivity;

} Camera;

void Camera_NoClip_Update(Camera* camera, const double deltaTime);

Camera* Object_Camera_create();


#ifdef __cplusplus
}
#endif