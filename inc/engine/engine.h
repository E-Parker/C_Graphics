#pragma once

#include "GLFW/glfw3.h"
#include "engine_core/engine_types.h"
#include "engine_core/list.h"

typedef struct FrameData {

    // Frame
    double time;
    double lastTime;
    double deltaTime;

    // Window
    int WindowWidth;
    int WindowHeight;
    double aspectRatio;
    float AmbientColor[3];
    bool WindowShouldClose;

    // Cursor
    bool rawInputAvailable;
    bool rawInputEnabled;
    bool captureCursor;
    double xScrollOffset;
    double yScrollOffset;
    double xPosDelta;
    double yPosDelta;
    double xPos;
    double yPos;
    double lastxPos;
    double lastyPos;
    
    // Keyboard
    bool KeyPressesPrevious[GLFW_KEY_LAST];
    bool KeyPressesCurrent[GLFW_KEY_LAST];
    bool keyPressesRepeat[GLFW_KEY_LAST];

    List TerminationFunctions;
    GLFWwindow* ActiveWindow;

} FrameData;

bool    Engine_initialize(const int width, const int height, const char* tittle);
void    Engine_terminate ();
void    Engine_add_termination_function (Function_Void_NoParam function);

bool    Engine_execute_tick ();
void    Engine_set_ambient_color (float r, float g, float b);
void    internal_Engine_clear_mouse_data ();

bool    isActive ();
double  Time ();
double  DeltaTime ();
double  AspectRatio ();
int     WindowHeight ();
int     WindowWidth ();

bool    IsKeyRepeat (int key);
bool    IsKeyPressed (int key);
bool    IsKeyDown (int key);
bool    IsKeyUp (int key);

void    SetUseRawInput (const bool useRawInput);
void    SetCaptureCursor (const bool captureCursor);
void    GetCursorPositionDelta (double* xPos, double* yPos);
void    GetCursorPosition (double* xPos, double* yPos);

void APIENTRY internal_Engine_mouse_callback (GLFWwindow* window, double xPos, double yPos);
void APIENTRY internal_Engine_scroll_callback (GLFWwindow* window, double xoffset, double yoffset);
void APIENTRY internal_Engine_key_callback (GLFWwindow* window, int key, int scancode, int action, int mods);
void APIENTRY internal_Engine_debug_callback (GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);

void Engine_exit_forced(int errorcode);
void internal_Engine_validate(bool check, int errorcode);

// TODO: implement error handling.
