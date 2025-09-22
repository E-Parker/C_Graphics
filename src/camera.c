#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "math.h"

#include "stdlib.h"
#include "string.h"

#include "engine/engine.h"
#include "engine/math.h"
#include "camera.h"

void Camera_NoClip_Update(Camera* camera, const double deltaTime, const double ratio) {
    /* Update function for a camera with no-clip. */

    vec3 desiredMovement = {0.0f, 0.0f, 0.0f};

    // Handle input:
    if(IsKeyDown(GLFW_KEY_W)) {
        printf("forward\n");
        vec3_sub(desiredMovement, V3_FORWARD, desiredMovement);
    }

    if(IsKeyDown(GLFW_KEY_S)) {
        printf("back\n");
        vec3_sub(desiredMovement, V3_BACKWARD, desiredMovement);
    }

    if(IsKeyDown(GLFW_KEY_D)) {
        printf("right\n");
        vec3_sub(desiredMovement, V3_RIGHT, desiredMovement);
    }

    if(IsKeyDown(GLFW_KEY_A)) {
        printf("left\n");
        vec3_sub(desiredMovement, V3_LEFT, desiredMovement);
    }

    if(IsKeyDown(GLFW_KEY_SPACE)) {
        printf("up\n");
        vec3_sub(desiredMovement, V3_UP, desiredMovement);
    }

    if(IsKeyDown(GLFW_KEY_LEFT_SHIFT)) {
        printf("down\n");
        vec3_sub(desiredMovement, V3_DOWN, desiredMovement);
    }
    
    // Get the cursor position and generate a rotation matrix from it.
    double CursorPositionX, CursorPositionY;
    GetCursorPositionDelta(&CursorPositionX, &CursorPositionY);

    vec3_scale(desiredMovement, camera->MoveSpeed * deltaTime);

    mat4 translation;
    mat4_translate(desiredMovement, translation);
    mat4_multiply(camera->Transform, translation, camera->Transform);

    vec3 cameraPos;
    vec3 cameraForward;
    vec3 cameraUp;

    mat4_get_translation(camera->Transform, cameraPos);
    mat4_get_forward(camera->Transform, cameraForward);
    mat4_get_up(camera->Transform, cameraForward);

    vec3_rotate_axis(cameraForward, V3_RIGHT, camera->Sensitivity * PI * CursorPositionY, cameraForward);
    vec3_rotate_axis(V3_UP, cameraForward, camera->Sensitivity * PI * (float)CursorPositionX, cameraForward);

    vec3 cameraTarget = vec3_def_add(cameraForward, cameraPos);

    mat4 projection;
    mat4_projection_perspective(camera->Fov, AspectRatio(), camera->NearClip, camera->FarClip, projection);

    mat4_multiply(camera->Transform, projection, camera->ViewMatrix);

    mat4_print(projection);
    mat4_print(camera->Transform);
    mat4_print(camera->ViewMatrix);

}

Camera* Object_Camera_create() {
    OBJECT_CREATE_BODY(Camera, NULL, Object_TypeCamera);

    mat4_copy(MAT4_IDENTITY, object->Transform);
    mat4_copy(MAT4_IDENTITY, object->ViewMatrix);

    object->MoveSpeed = 1.0f;
    object->Acceleration = 0.4f;
    object->Fov = 60.0f;
    object->NearClip = 0.001f;
    object->FarClip = 1024.0f;
    object->Sensitivity = 1.0f;
    
    object->Destroy = Object_Camera_destroy;
    return object;
}


void Object_Camera_destroy(void* camera) {
    OBJECT_DESTROY_BODY(camera);
}

