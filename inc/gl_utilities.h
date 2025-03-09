#pragma once

#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "bool.h"

typedef struct List;
typedef void (*Function_Void_NoParam)();

typedef struct {
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

} InstanceInformation;

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);
struct GLFWwindow* Initialize(const int width, const int height, const char* tittle);

void glUtilTerminate();
void glUtilAddTerminationFunction(Function_Void_NoParam function);

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

#ifdef __cplusplus
}
#endif
