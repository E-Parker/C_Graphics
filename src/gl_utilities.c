#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "list.h"
#include "gl_utilities.h"

static InstanceInformation* internalInstanceInfo;

double Time() {
    return internalInstanceInfo->time;
}

double DeltaTime() {
    return internalInstanceInfo->deltaTime;
}

double AspectRatio() {
    return internalInstanceInfo->aspectRatio;
}

int WindowHeight() {
    return internalInstanceInfo->WindowHeight;
}

int WindowWidth() {
    return internalInstanceInfo->WindowWidth;
}

void SetCaptureCursor(const bool captureCursor) {
    // Globally accessible function to tell GKFW what the cursor settings should be.
    // if captureCursor is set, the xPos and yPos from GetCursorPosition will act like a delta.
    internalInstanceInfo->captureCursor = captureCursor;
}

void GetCursorPositionDelta(double* xPos, double* yPos) {
    // Globally accessible function to get the cursor position.
    *xPos = internalInstanceInfo->xPosDelta / (double)internalInstanceInfo->WindowHeight;
    *yPos = internalInstanceInfo->yPosDelta / (double)internalInstanceInfo->WindowHeight;

    // This is not great but I cant think of a better way to make sure the delta get set back to zero once after camera gets updated. 
    internalInstanceInfo->xPosDelta = 0.0;
    internalInstanceInfo->yPosDelta = 0.0;
}

void GetCursorPosition(double* xPos, double* yPos) {
    // Globally accessible function to get the cursor position.
    *xPos = internalInstanceInfo->xPos / (double)internalInstanceInfo->WindowHeight;
    *yPos = internalInstanceInfo->yPos / (double)internalInstanceInfo->WindowHeight;
}

GLFWwindow* Initialize(const int width, const int height, const char* tittle) {
    /* Initialize a GLFW window. */
    
    assert(glfwInit() == GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    
    #ifdef NDEBUG
    #else
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    #endif

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(width, height, tittle, NULL, NULL);
    glfwMakeContextCurrent(window);
    assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    #ifdef NDEBUG
    #else
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(glDebugOutput, NULL);
    #endif
    
    glfwSetWindowSize(window, width, height);

    // Misc OpenGL settings.
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);

    // Initialize InstanceInfo:
    internalInstanceInfo = (InstanceInformation*)calloc(1, sizeof(InstanceInformation));
    assert(internalInstanceInfo);

    internalInstanceInfo->gKeysCurr = (unsigned int*)calloc(GLFW_KEY_LAST, sizeof(unsigned int));
    internalInstanceInfo->gKeysPrev = (unsigned int*)calloc(GLFW_KEY_LAST, sizeof(unsigned int));
    assert(internalInstanceInfo->gKeysCurr);
    assert(internalInstanceInfo->gKeysPrev);
    
    internalInstanceInfo->WindowHeight = height;
    internalInstanceInfo->WindowWidth = width;
    
    internalInstanceInfo->TerminationFunctions = List_create(Function_Void_NoParam, 16);

    return window;
}

void glUtilTerminate() {
    /* This function executes each function in the list of termination functions. */
    if (List_isEmpty((List*)internalInstanceInfo->TerminationFunctions)) {
        return;
    }

    // For each termination function added to the list, call it.
    for List_iterator(Function_Void_NoParam, internalInstanceInfo->TerminationFunctions) {
        (*it)();
    }
}

void glUtilAddTerminationFunction(Function_Void_NoParam function) {
    List_push_back(internalInstanceInfo->TerminationFunctions, function);
}


void glUtilSetAmbientColor(float r, float g, float b) {
    internalInstanceInfo->AmbientColor[0] = r;
    internalInstanceInfo->AmbientColor[1] = g;
    internalInstanceInfo->AmbientColor[2] = b;
}

void glUtilInitializeFrame(GLFWwindow* window){
    
    internalInstanceInfo->time = glfwGetTime();
    internalInstanceInfo->deltaTime = internalInstanceInfo->time - internalInstanceInfo->lastTime;

    // Resize the view to match the window.
    glfwGetFramebufferSize(window, &(internalInstanceInfo->WindowWidth), &(internalInstanceInfo->WindowHeight));
    internalInstanceInfo->aspectRatio = (double)internalInstanceInfo->WindowWidth / (double)internalInstanceInfo->WindowHeight;
    glViewport(0, 0, internalInstanceInfo->WindowWidth, internalInstanceInfo->WindowHeight);
    
    // Clear the screen buffer.
    glClearColor(internalInstanceInfo->AmbientColor[0], internalInstanceInfo->AmbientColor[1], internalInstanceInfo->AmbientColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
}

static void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
    /* Get the mouse position from the window.*/

    // truncate it down to an int to make handling it easier.
    internalInstanceInfo->xPosDelta = internalInstanceInfo->xPos - xPos;
    internalInstanceInfo->yPosDelta = internalInstanceInfo->yPos - yPos;
    internalInstanceInfo->xPos = xPos;
    internalInstanceInfo->yPos = yPos;

    // If the mouse should be captured, set it as such.
    if(internalInstanceInfo->captureCursor) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    // otherwise set the cursor back to normal.
    else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    /* Update the keyboard input lists from GLFW. */

    // Leave early if the key is repeated, to "disable" repeat events so that keys are either up or down
    if (action == GLFW_REPEAT) {
        return;
    }

    // Leave early if the exit key is pressed.
    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }
    
    internalInstanceInfo->gKeysCurr[key] = action;

}

void glUtilPollEvents() {
    /* Handle keyboard polling, update the internal timer, and call glfwPollEvents. */

    // Copy this frame's key presses into the "Previous" buffer.
    memcpy(internalInstanceInfo->gKeysPrev, internalInstanceInfo->gKeysCurr, GLFW_KEY_LAST * sizeof(int));
    glfwPollEvents();
    
    internalInstanceInfo->lastTime = internalInstanceInfo->time;
}

bool IsKeyDown(int key) {
    return internalInstanceInfo->gKeysCurr[key] == GLFW_PRESS;
}

bool IsKeyUp(int key) {
    return internalInstanceInfo->gKeysCurr[key] == GLFW_RELEASE;
}

bool IsKeyPressed(int key) {
    return internalInstanceInfo->gKeysPrev[key] == GLFW_PRESS && internalInstanceInfo->gKeysCurr[key] == GLFW_RELEASE;
}

// Graphics debug callback
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    printf("---------------\nDebug message (%u): %s\n", id, message);

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             printf("Source: API");
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   printf("Source: Window System");
    case GL_DEBUG_SOURCE_SHADER_COMPILER: printf("Source: Shader Compiler");
    case GL_DEBUG_SOURCE_THIRD_PARTY:     printf("Source: Third Party");
    case GL_DEBUG_SOURCE_APPLICATION:     printf("Source: Application");
    case GL_DEBUG_SOURCE_OTHER:           printf("Source: Other");
    }

    printf("\n");

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               printf("Type: Error");
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: printf("Type: Deprecated Behaviour");
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  printf("Type: Undefined Behaviour");
    case GL_DEBUG_TYPE_PORTABILITY:         printf("Type: Portability");
    case GL_DEBUG_TYPE_PERFORMANCE:         printf("Type: Performance");
    case GL_DEBUG_TYPE_MARKER:              printf("Type: Marker");
    case GL_DEBUG_TYPE_PUSH_GROUP:          printf("Type: Push Group");
    case GL_DEBUG_TYPE_POP_GROUP:           printf("Type: Pop Group");
    case GL_DEBUG_TYPE_OTHER:               printf("Type: Other");
    }
    
    printf("\n");

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         printf("Severity: high");
    case GL_DEBUG_SEVERITY_MEDIUM:       printf("Severity: medium");
    case GL_DEBUG_SEVERITY_LOW:          printf("Severity: low");
    case GL_DEBUG_SEVERITY_NOTIFICATION: printf("Severity: notification");
    } 

    printf("\n"); 

}




