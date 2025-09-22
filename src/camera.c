#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stdlib.h"
#include "string.h"

#include "engine/engine.h"
#include "engine/math.h"
#include "camera.h"

#include "stdio.h"

void Camera_NoClip_Update(Camera* camera, const double deltaTime, const double ratio) {
    /* Update function for a camera with no-clip. */

    vec3 desiredMovement = {0.0f, 0.0f, 0.0f};

    // Handle input:
    if(IsKeyDown(GLFW_KEY_W)) {
        printf("forward\n");
        vec3_add(desiredMovement, V3_FORWARD, desiredMovement);
    }

    if(IsKeyDown(GLFW_KEY_S)) {
        printf("back\n");
        vec3_add(desiredMovement, V3_BACKWARD, desiredMovement);
    }

    if(IsKeyDown(GLFW_KEY_D)) {
        printf("right\n");
        vec3_add(desiredMovement, V3_RIGHT, desiredMovement);
    }

    if(IsKeyDown(GLFW_KEY_A)) {
        printf("left\n");
        vec3_add(desiredMovement, V3_LEFT, desiredMovement);
    }

    if(IsKeyDown(GLFW_KEY_SPACE)) {
        printf("up\n");
        vec3_add(desiredMovement, V3_UP, desiredMovement);
    }

    if(IsKeyDown(GLFW_KEY_LEFT_SHIFT)) {
        printf("down\n");
        vec3_add(desiredMovement, V3_DOWN, desiredMovement);
    }
    
    // Get the cursor position and generate a rotation matrix from it.
    double CursorPositionX, CursorPositionY;
    GetCursorPositionDelta(&CursorPositionX, &CursorPositionY);

    camera->Transform[12] += desiredMovement[0] * camera->MoveSpeed * deltaTime;
    camera->Transform[13] += desiredMovement[1] * camera->MoveSpeed * deltaTime;
    camera->Transform[14] += desiredMovement[2] * camera->MoveSpeed * deltaTime;

    vec3_rotate_axis(camera->Forward, V3_RIGHT, camera->Sensitivity * PI * CursorPositionY, camera->Forward);
    vec3_rotate_axis(V3_UP, camera->Forward, camera->Sensitivity * PI * (float)CursorPositionX, camera->Forward);
    
    vec3 cameraPos;
    mat4_get_translation(camera->Transform, cameraPos);

    vec3 cameraTarget = vec3_def_add(camera->Forward, cameraPos);

    mat4_get_translation(camera->Transform, cameraPos);

    mat4_lookat(cameraPos, cameraTarget, V3_UP, camera->ViewMatrix);



}

Camera* Object_Camera_create() {
    OBJECT_CREATE_BODY(Camera, NULL, Object_TypeCamera);

    mat4_copy(MAT4_IDENTITY, object->Transform);
    mat4_copy(MAT4_IDENTITY, object->ViewMatrix);
    vec3_copy(V3_FORWARD, object->Forward);

    object->MoveSpeed = 1.0f;
    object->Acceleration = 0.4f;
    object->Fov = 90.0f;
    object->NearClip = 0.001f;
    object->FarClip = 1024.0f;
    object->Sensitivity = 1.0f;
    
    object->Destroy = Object_Camera_destroy;
    return object;
}


void Object_Camera_destroy(void* camera) {
    OBJECT_DESTROY_BODY(camera);
}

