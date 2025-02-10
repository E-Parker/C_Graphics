#pragma once

#include <stdbool.h>

// Forward Declarations:
struct GLFWwindow;

typedef void (*Function_Void_NoParam)();


struct InstanceInformation {
    /* Struct to hold data relevant to the state of the instance. */ 
    
    // Frame delta:
    double time;
    double lastTime;
    double deltaTime;

    // Window Data:
    int WindowWidth;
    int WindowHeight;
    double aspectRatio;
    
    // Cursor Data:
    bool captureCursor = false;
    double xPosDelta = 0.0;
    double yPosDelta = 0.0;
    double xPos = 0.0;
    double yPos = 0.0;
    double lastxPos = 0.0;
    double lastyPos = 0.0;

    // Graphics Data:
    float AmbientColor[3];
    
    // Internal lists:
    int* gKeysPrev;
    int* gKeysCurr;

    // List of Termination Functions:
    Function_Void_NoParam* TerminationFunctions;
};

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);
GLFWwindow* Initialize(const int width, const int height, const char* tittle);

void glUtilTerminate();
void glUtilAddTerminationFunction(TerminateFunction function);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_callback(GLFWwindow* window, double xPos, double yPos);

void glUtilSetAmbientColor(float r, float g, float b);
void glUtilInitializeFrame(GLFWwindow* window);
void glUtilPollEvents();

void SetCaptureCursor(const bool captureCursor);
void GetCursorPositionDelta(double* xPos, double* yPos);
void GetCursorPosition(double* xPos, double* yPos);

double Time();
double DeltaTime();
double AspectRatio();
int WindowHeight();
int WindowWidth();

bool IsKeyPressed(int key);
bool IsKeyDown(int key);
bool IsKeyUp(int key);
