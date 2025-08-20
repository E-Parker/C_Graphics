#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string.h>

#include "gl_utilities.h"
#include "gl_math.h"
#include "camera.h"

void NoClipCameraUpdate(Camera* camera, const double deltaTime, const double ratio) {
    /* Update function for a camera with no-clip. */

    vec3 desiredMovement = {0.0f, 0.0f, 0.0f};

    // Handle input:
    if(IsKeyDown(GLFW_KEY_W)) {
        vec3_add(desiredMovement, V3_FORWARD, desiredMovement);
    }

    if(IsKeyDown(GLFW_KEY_S)) {
        vec3_sub(desiredMovement, V3_FORWARD, desiredMovement);
    }

    if(IsKeyDown(GLFW_KEY_D)) {
        vec3_sub(desiredMovement, V3_RIGHT, desiredMovement);
    }

    if(IsKeyDown(GLFW_KEY_A)) {
        vec3_add(desiredMovement, V3_FORWARD, desiredMovement);
    }

    if(IsKeyDown(GLFW_KEY_SPACE)) {
        vec3_sub(desiredMovement, V3_UP, desiredMovement);
    }

    if(IsKeyDown(GLFW_KEY_LEFT_SHIFT)) {
        vec3_add(desiredMovement, V3_UP, desiredMovement);
    }
    
    // Get the cursor position and generate a rotation matrix from it.
    double CursorPositionX, CursorPositionY;
    GetCursorPositionDelta(&CursorPositionX, &CursorPositionY);
    
}

Camera* Object_Camera_create() {
    
    Camera* camera = (Camera*)malloc(sizeof(Camera));
    
    OBJECT_CREATE_BODY(camera, NULL, Object_TypeCamera);

    camera->MoveSpeed = 1.0f;
    camera->Acceleration = 0.4f;
    camera->Fov = 60.0f;
    camera->NearClip = 0.001f;
    camera->FarClip = 1024.0f;
    camera->Sensitivity = 1.0f;

    return camera;
}


void Object_Camera_destroy(Camera* camera) {
    OBJECT_DESTROY_BODY(camera);
}

