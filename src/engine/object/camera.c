#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

#include "engine/engine.h"
#include "engine/object/camera.h"
#include "engine/math.h"



Camera* Object_Camera_create() {
    OBJECT_CREATE_BODY(Camera, NULL, Object_TypeCamera);

    mat4_copy(MAT4_IDENTITY, object->Transform);
    mat4_copy(MAT4_IDENTITY, object->ViewMatrix);
    quaternion_copy(V4_IDENTIY, object->Rotation);
    vec3_copy(V3_ZERO, object->Velocity);

    object->DragCoefficient = 1.5f;
    object->MoveSpeed = 1.0f;
    object->Fov = 60.0f;
    object->NearClip = 0.001f;
    object->FarClip = 1024.0f;
    object->Sensitivity = 0.2f;

    object->Tick = Object_Camera_update_noclip;
    object->Destroy = Object_Camera_destroy;
    return object;
}


void Object_Camera_destroy(void* camera) {
    OBJECT_DESTROY_BODY(camera);
}


void Object_Camera_recalulate_view(Camera* camera) {

    double CursorPositionX = 0.0;
    double CursorPositionY = 0.0;
    GetCursorPositionDelta(&CursorPositionX, &CursorPositionY);

    quaternion rotationPitch = quaternion_def_from_axis(V3_RIGHT, camera->Sensitivity * PI * -CursorPositionY);
    quaternion rotationYaw = quaternion_def_from_axis(V3_UP, camera->Sensitivity * PI * -CursorPositionX);

    quaternion_multiply(rotationPitch, camera->Rotation, camera->Rotation);
    quaternion_normalize(camera->Rotation);

    quaternion_multiply(camera->Rotation, rotationYaw, camera->Rotation);
    quaternion_normalize(camera->Rotation);

    mat4 rotation;
    mat4_from_quaternion(camera->Rotation, rotation);

    mat4 projection;
    //mat4_projection_orthographic(-5.0, 5.0, 5.0, -5.0, -5.0, 5.0, projection);
    mat4_projection_perspective(camera->Fov, AspectRatio(), camera->NearClip, camera->FarClip, projection);

    mat4_multi_multiply(4, &camera->Transform, &rotation, &projection, &camera->ViewMatrix);

}

u8 Object_Camera_update_noclip(void* object, const double deltaTime) {

    Camera* camera = (Camera*)object;

    vec3 movement = { 0.0f, 0.0f, 0.0f };
    

    if(IsKeyDown(GLFW_KEY_W)) {
        vec3_add(movement, V3_FORWARD, movement);
    }

    if(IsKeyDown(GLFW_KEY_S)) {
        vec3_add(movement, V3_BACKWARD, movement);
    }

    if(IsKeyDown(GLFW_KEY_D)) {
        vec3_sub(movement, V3_RIGHT, movement);
    }

    if(IsKeyDown(GLFW_KEY_A)) {
        vec3_sub(movement, V3_LEFT, movement);
    }

    if(IsKeyDown(GLFW_KEY_SPACE)) {
        vec3_sub(movement, V3_UP, movement);
    }

    if(IsKeyDown(GLFW_KEY_LEFT_SHIFT)) {
        vec3_sub(movement, V3_DOWN, movement);
    }

    vec3_normalize(movement);
    vec3_scale(movement, (float)(camera->MoveSpeed * deltaTime));

    quaternion rotation = vec4_def_copy(camera->Rotation);
    
    quaternion_invert(rotation);
    vec3_rotate(movement, rotation, movement);

    //vec3 Acceleration = { 0.0f, 0.0f, 0.0f };
    //vec3 drag = { 0.0f, 0.0f, 0.0f };
    //
    //vec3_copy(camera->Velocity, drag);
    //vec3_normalize(drag);
    //vec3_scale(drag, -camera->DragCoefficient * vec3_sqr_magnitude(camera->Velocity));
    //
    //vec3_add(Acceleration, movement, Acceleration);
    //vec3_add(Acceleration, drag, Acceleration);
    //vec3_scale(Acceleration, deltaTime * deltaTime);
    //
    //vec3_add(camera->Velocity, Acceleration, camera->Velocity);

    vec3_copy(movement, camera->Velocity);

    mat4 translation;
    mat4_translate(camera->Velocity, translation);
    mat4_multiply(camera->Transform, translation, camera->Transform);

    Object_Camera_recalulate_view(camera);
    return 0;
}
