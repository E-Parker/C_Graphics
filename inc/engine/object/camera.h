#pragma once

#include "engine/math.h"
#include "engine/object.h"

typedef struct Camera {
    OBJECT_BODY();
    mat4 ViewMatrix;
    quaternion Rotation;
    vec3 Velocity;

    double DragCoefficient;
    double MoveSpeed;
    double Fov;
    double NearClip;
    double FarClip;
    double Sensitivity;

} Camera;


Camera* Object_Camera_create();
void Object_Camera_destroy(void* camera);

u8 Object_Camera_update_noclip(void* object, const double deltaTime);
void Object_Camera_recalulate_view(Camera* camera);

