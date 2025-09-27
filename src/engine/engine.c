#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "stdio.h"

#include "engine/engine.h"

#define ENGINE_DEBUG

FrameData frame;


bool isActive() {
    return !glfwWindowShouldClose(frame.ActiveWindow);
}

double Time () {
    return frame.time;
}

double DeltaTime () {
    return frame.deltaTime;
}

double AspectRatio () {
    return frame.aspectRatio;
}

int WindowHeight () {
    return frame.WindowHeight;
}

int WindowWidth () {
    return frame.WindowWidth;
}


void SetUseRawInput(const bool useRawInput) {
    frame.rawInputEnabled = useRawInput;
}

void SetCaptureCursor (const bool captureCursor) {
    frame.captureCursor = captureCursor;
}

void Engine_set_ambient_color(float r, float g, float b) {
    frame.AmbientColor[0] = r;
    frame.AmbientColor[1] = g;
    frame.AmbientColor[2] = b;
}


void GetCursorPositionDelta (double* xPos, double* yPos) {
    *xPos = frame.xPosDelta / (double)frame.WindowHeight;
    *yPos = frame.yPosDelta / (double)frame.WindowHeight;
}

void GetCursorPosition (double* xPos, double* yPos) {
    *xPos = frame.xPos / (double)frame.WindowHeight;
    *yPos = frame.yPos / (double)frame.WindowHeight;
}


void initializeWindowHints () {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void initializeWindow (const int width, const int height, const char* tittle) {
    frame.WindowWidth = width;
    frame.WindowHeight = height;
    frame.ActiveWindow = glfwCreateWindow(width, height, tittle, NULL, NULL);
    glfwSetWindowSize(frame.ActiveWindow, width, height);
    glfwMakeContextCurrent(frame.ActiveWindow);
}

void initializeCallbacks (GLFWwindow* window) {
    glfwSetCursorPosCallback(window, internal_Engine_mouse_callback);
    glfwSetKeyCallback(window, internal_Engine_key_callback);
#ifdef ENGINE_DEBUG
    glDebugMessageCallback(internal_Engine_debug_callback, NULL);
    glEnable(GL_DEBUG_OUTPUT);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
}

void internal_Engine_clear_mouse_data () {
    glfwPollEvents();
    frame.xPos = 0.0f;
    frame.yPos = 0.0f;
    frame.xPosDelta = 0.0f;
    frame.yPosDelta = 0.0f;
}

bool Engine_initialize(const int width, const int height, const char* tittle) {
    if (!glfwInit()) {
        goto InitFail;
    }

    initializeWindowHints();
    initializeWindow(width, height, tittle);

    if (!frame.ActiveWindow) {
        goto InitFail_Terminate;
    }
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        goto InitFail_DestroyWindowAndTerminate;
    }
    
    initializeCallbacks(frame.ActiveWindow);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);

    List_initialize(Function_Void_NoParam, &(frame.TerminationFunctions), 16);

    frame.rawInputAvailable = glfwRawMouseMotionSupported();

    return true;

InitFail_DestroyWindowAndTerminate:
    glfwDestroyWindow(frame.ActiveWindow);
    
InitFail_Terminate:
    glfwTerminate();

InitFail:
    return false;
}


void Engine_add_termination_function(Function_Void_NoParam function) {
    List_push_back(&(frame.TerminationFunctions), function);
}

void Engine_terminate () {
    printf("Starting Engine termination ...\n");

    for (List_iterator(Function_Void_NoParam, &(frame.TerminationFunctions))) {
        (*it)();
    }

    List_deinitialize(&(frame.TerminationFunctions));

    printf("Engine terminated successfully.\n");
}


void InitializeFrame () {

    frame.lastTime = frame.time;
    frame.time = glfwGetTime();
    frame.deltaTime = frame.time - frame.lastTime;

    glfwGetFramebufferSize(frame.ActiveWindow, &(frame.WindowWidth), &(frame.WindowHeight));
    frame.aspectRatio = (double)frame.WindowWidth / (double)frame.WindowHeight;
    glViewport(0, 0, frame.WindowWidth, frame.WindowHeight);
    
    glClearColor(frame.AmbientColor[0], frame.AmbientColor[1], frame.AmbientColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PollEvents () {
    for (int i = 0; i < GLFW_KEY_LAST; ++i) {
        frame.KeyPressesPrevious[i] = frame.KeyPressesCurrent[i];
    }

    frame.xPosDelta = 0.0;
    frame.yPosDelta = 0.0;

    glfwPollEvents();
}

bool Engine_execute_tick () {
    glfwSwapBuffers(frame.ActiveWindow);
    PollEvents();

    if (frame.rawInputAvailable && frame.rawInputEnabled) {
        glfwSetInputMode(frame.ActiveWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    if (frame.captureCursor) {
        glfwSetInputMode(frame.ActiveWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    else {
        glfwSetInputMode(frame.ActiveWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    InitializeFrame();

    if (IsKeyPressed(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(frame.ActiveWindow, true);
    }

    // TODO: implement handling of queued events.

    return isActive();
}

bool IsKeyDown (int key) {
    return frame.KeyPressesCurrent[key] ? true : false ;
}

bool IsKeyUp (int key) {
    return !frame.KeyPressesCurrent[key] ? true : false ;
}

bool IsKeyPressed (int key) {
    return frame.KeyPressesPrevious[key] && !frame.KeyPressesCurrent[key] ? true : false ;
}

bool IsKeyRepeat(int key) {
    return frame.keyPressesRepeat[key];
}


void APIENTRY internal_Engine_mouse_callback(GLFWwindow* window, double xPos, double yPos) {
    frame.xPosDelta = frame.xPos - xPos;
    frame.yPosDelta = frame.yPos - yPos;
    frame.xPos = xPos;
    frame.yPos = yPos;
}

void APIENTRY internal_Engine_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    frame.xScrollOffset = xoffset;
    frame.yScrollOffset = yoffset;
}


void APIENTRY internal_Engine_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_REPEAT) {
        frame.keyPressesRepeat[key] = true;
    }

    else {
        frame.keyPressesRepeat[key] = false;
        frame.KeyPressesCurrent[key] = action ? true : false;
    }
}

void APIENTRY internal_Engine_debug_callback (GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam) {

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
