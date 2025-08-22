#pragma once


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

void Camera_NoClip_Update(Camera* camera, const double deltaTime, const double aspect);

Camera* Object_Camera_create();
void Object_Camera_destroy(Camera* camera);


