#pragma once


#ifdef __cplusplus
extern "C" {
#endif

#include "bool.h"

// Type Definitions:
// 
//

typedef void (*Function_Void_NoParam)();

// GLFW Functions:
//
//

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);
struct GLFWwindow* Initialize(const int width, const int height, const char* tittle);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_callback(GLFWwindow* window, double xPos, double yPos);

void glUtilTerminate();
void glUtilAddTerminationFunction(Function_Void_NoParam function);

void glUtilSetAmbientColor(float r, float g, float b);
void glUtilInitializeFrame(GLFWwindow* window);
void glUtilPollEvents();

// Functions bound to static InstanceInfo:
//
//

double Time();
double DeltaTime();
double AspectRatio();
int WindowHeight();
int WindowWidth();

bool IsKeyPressed(int key);
bool IsKeyDown(int key);
bool IsKeyUp(int key);

void SetCaptureCursor(const bool captureCursor);
void GetCursorPositionDelta(double* xPos, double* yPos);
void GetCursorPosition(double* xPos, double* yPos);

#ifdef __cplusplus
}
#endif
