#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "list.h"
#include "gl_utilities.h"


typedef struct {
    /* Struct to hold data for relevant to the state of the program. Updated every frame. */ 
    
    // Frame delta:
    double time;
    double lastTime;
    double deltaTime;

    // Window Data:
    int WindowWidth;
    int WindowHeight;
    double aspectRatio;
    
    // Cursor Data:
    bool captureCursor;
    double xPosDelta;
    double yPosDelta;
    double xPos;
    double yPos;
    double lastxPos;
    double lastyPos;

    // Graphics Data:
    float AmbientColor[3];
    
    // Internal lists:
    int* gKeysPrev;
    int* gKeysCurr;

    // List of Termination Functions:
    List* TerminationFunctions;

} FrameData;


// Internal instance of frameData.
static FrameData* internal_FrameData;


double Time() {
    return internal_FrameData->time;
}


double DeltaTime() {
    return internal_FrameData->deltaTime;
}


double AspectRatio() {
    return internal_FrameData->aspectRatio;
}


int WindowHeight() {
    return internal_FrameData->WindowHeight;
}


int WindowWidth() {
    return internal_FrameData->WindowWidth;
}


void SetCaptureCursor(const bool captureCursor) {
    // Globally accessible function to tell GKFW what the cursor settings should be.
    // if captureCursor is set, the xPos and yPos from GetCursorPosition will act like a delta.
    internal_FrameData->captureCursor = captureCursor;
}


void GetCursorPositionDelta(double* xPos, double* yPos) {
    // Globally accessible function to get the cursor position.
    *xPos = internal_FrameData->xPosDelta / (double)internal_FrameData->WindowHeight;
    *yPos = internal_FrameData->yPosDelta / (double)internal_FrameData->WindowHeight;

    // This is not great but I cant think of a better way to make sure the delta get set back to zero once after camera gets updated. 
    internal_FrameData->xPosDelta = 0.0;
    internal_FrameData->yPosDelta = 0.0;
}


void GetCursorPosition(double* xPos, double* yPos) {
    // Globally accessible function to get the cursor position.
    *xPos = internal_FrameData->xPos / (double)internal_FrameData->WindowHeight;
    *yPos = internal_FrameData->yPos / (double)internal_FrameData->WindowHeight;
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
    internal_FrameData = (FrameData*)calloc(1, sizeof(FrameData));
    assert(internal_FrameData);

    internal_FrameData->gKeysCurr = (unsigned int*)calloc(GLFW_KEY_LAST, sizeof(unsigned int));
    internal_FrameData->gKeysPrev = (unsigned int*)calloc(GLFW_KEY_LAST, sizeof(unsigned int));
    assert(internal_FrameData->gKeysCurr);
    assert(internal_FrameData->gKeysPrev);
    
    internal_FrameData->WindowHeight = height;
    internal_FrameData->WindowWidth = width;
    
    internal_FrameData->TerminationFunctions = List_create(Function_Void_NoParam, 16);

    return window;
}


void glUtilTerminate() {
    /* This function executes each function in the list of termination functions. */
    if (List_isEmpty((List*)internal_FrameData->TerminationFunctions)) {
        return;
    }

    // For each termination function added to the list, call it.
    for List_iterator(Function_Void_NoParam, internal_FrameData->TerminationFunctions) {
        (*it)();
    }
}

void glUtilAddTerminationFunction(Function_Void_NoParam function) {
    List_push_back(internal_FrameData->TerminationFunctions, function);
}


void glUtilSetAmbientColor(float r, float g, float b) {
    internal_FrameData->AmbientColor[0] = r;
    internal_FrameData->AmbientColor[1] = g;
    internal_FrameData->AmbientColor[2] = b;
}


void glUtilInitializeFrame(GLFWwindow* window){
    
    internal_FrameData->time = glfwGetTime();
    internal_FrameData->deltaTime = internal_FrameData->time - internal_FrameData->lastTime;

    // Resize the view to match the window.
    glfwGetFramebufferSize(window, &(internal_FrameData->WindowWidth), &(internal_FrameData->WindowHeight));
    internal_FrameData->aspectRatio = (double)internal_FrameData->WindowWidth / (double)internal_FrameData->WindowHeight;
    glViewport(0, 0, internal_FrameData->WindowWidth, internal_FrameData->WindowHeight);
    
    // Clear the screen buffer.
    glClearColor(internal_FrameData->AmbientColor[0], internal_FrameData->AmbientColor[1], internal_FrameData->AmbientColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


static void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
    /* Get the mouse position from the window.*/

    // truncate it down to an int to make handling it easier.
    internal_FrameData->xPosDelta = internal_FrameData->xPos - xPos;
    internal_FrameData->yPosDelta = internal_FrameData->yPos - yPos;
    internal_FrameData->xPos = xPos;
    internal_FrameData->yPos = yPos;

    // If the mouse should be captured, set it as such.
    if(internal_FrameData->captureCursor) {
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
    
    internal_FrameData->gKeysCurr[key] = action;

}


void glUtilPollEvents() {
    /* Handle keyboard polling, update the internal timer, and call glfwPollEvents. */

    // Copy this frame's key presses into the "Previous" buffer.
    memcpy(internal_FrameData->gKeysPrev, internal_FrameData->gKeysCurr, GLFW_KEY_LAST * sizeof(int));
    glfwPollEvents();
    
    internal_FrameData->lastTime = internal_FrameData->time;
}


bool IsKeyDown(int key) {
    return internal_FrameData->gKeysCurr[key] == GLFW_PRESS;
}


bool IsKeyUp(int key) {
    return internal_FrameData->gKeysCurr[key] == GLFW_RELEASE;
}


bool IsKeyPressed(int key) {
    return internal_FrameData->gKeysPrev[key] == GLFW_PRESS && internal_FrameData->gKeysCurr[key] == GLFW_RELEASE;
}


// Graphics debug callback
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam) {

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


