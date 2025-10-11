#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "stdio.h"

#include "engine_core/configuation.h"
#include "engine_core/engine_error.h"
#include "engine/engine.h"


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

bool InitEngine(const int width, const int height, const char* tittle) {
    
    Engine_validate(sizeof(i8) == sizeof(u8) && sizeof(u8) == 1, ERROR_INTSIZE);
    Engine_validate(sizeof(i16) == sizeof(u16) && sizeof(u16) == 2, ERROR_INTSIZE);
    Engine_validate(sizeof(i32) == sizeof(u32) && sizeof(u32) == 4, ERROR_INTSIZE);
    Engine_validate(sizeof(i64) == sizeof(u64) && sizeof(u64) == 8, ERROR_INTSIZE);

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

    List_initialize(Function_Errorcode_NoParam, &(frame.TerminationFunctions), 16);

    frame.rawInputAvailable = glfwRawMouseMotionSupported();

    return true;

InitFail_DestroyWindowAndTerminate:
    glfwDestroyWindow(frame.ActiveWindow);
    
InitFail_Terminate:
    glfwTerminate();

InitFail:
    return false;
}


void Engine_add_termination_function(Function_Errorcode_NoParam function) {
    List_push_back(&(frame.TerminationFunctions), function);
}

void Engine_terminate () {
    for (List_iterator(Function_Errorcode_NoParam, &(frame.TerminationFunctions))) {
        ecode errorcode = (*it)();

    }

    List_deinitialize(&(frame.TerminationFunctions));
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

#ifdef ENGINE_DEBUG
void APIENTRY internal_Engine_debug_callback (GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam) {

    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    Engine_log("---------------\nDebug message (%u): %s\n", id, message);

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             Engine_log("Source: API");
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   Engine_log("Source: Window System");
    case GL_DEBUG_SOURCE_SHADER_COMPILER: Engine_log("Source: Shader Compiler");
    case GL_DEBUG_SOURCE_THIRD_PARTY:     Engine_log("Source: Third Party");
    case GL_DEBUG_SOURCE_APPLICATION:     Engine_log("Source: Application");
    case GL_DEBUG_SOURCE_OTHER:           Engine_log("Source: Other");
    }

    Engine_log("\n");

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               Engine_log("Type: Error");
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: Engine_log("Type: Deprecated Behaviour");
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  Engine_log("Type: Undefined Behaviour");
    case GL_DEBUG_TYPE_PORTABILITY:         Engine_log("Type: Portability");
    case GL_DEBUG_TYPE_PERFORMANCE:         Engine_log("Type: Performance");
    case GL_DEBUG_TYPE_MARKER:              Engine_log("Type: Marker");
    case GL_DEBUG_TYPE_PUSH_GROUP:          Engine_log("Type: Push Group");
    case GL_DEBUG_TYPE_POP_GROUP:           Engine_log("Type: Pop Group");
    case GL_DEBUG_TYPE_OTHER:               Engine_log("Type: Other");
    }
    
    Engine_log("\n");

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         Engine_log("Severity: high");
    case GL_DEBUG_SEVERITY_MEDIUM:       Engine_log("Severity: medium");
    case GL_DEBUG_SEVERITY_LOW:          Engine_log("Severity: low");
    case GL_DEBUG_SEVERITY_NOTIFICATION: Engine_log("Severity: notification");
    } 
    Engine_log("\n"); 
}
#endif

void Engine_exit(ecode errorcode) {
    Engine_log_errorcode(errorcode);
    Engine_terminate();
    exit(errorcode);
}

void Engine_exit_forced(ecode errorcode) {
    Engine_log_errorcode(errorcode);
    exit(errorcode);
}

#ifdef ENGINE_DEBUG
void internal_Engine_validate(bool check, ecode errorcode) {
    if (!check) {
        return;
    }
    Engine_exit_forced(errorcode);
}
#endif
