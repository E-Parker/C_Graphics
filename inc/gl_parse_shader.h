#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef GL_SHADER_MAX_DESCRIPTORS
#define GL_SHADER_MAX_DESCRIPTORS 64 
#endif

#ifndef GL_ERROR_LOG_SIZE
#define GL_ERROR_LOG_SIZE 512
#endif

#ifndef GL_SHADER_PATH_SIZE
#define GL_SHADER_PATH_SIZE 128 
#endif

#ifndef GL_SHADER_SOURCE_SIZE
#define GL_SHADER_SOURCE_SIZE 0xffff 
#endif

typedef struct {
    // Struct to store shader reference, type, and file path. 
    // if path is empty, the shader is null. 
    GLuint shader;
    GLint type;
    char path[GL_SHADER_PATH_SIZE];
} ShaderDescriptor;

// Macro to compile and link any number of shaders. Use this for compile-time constant since this guarentes there is no size miss-match.
#define Shader_CompileProgram(program, ...) do { ShaderDescriptor args[] = { __VA_ARGS__, {0, 0, ""} }; program = internal_Shader_CompileProgram(&args); } while(0)

// Compile and link any number of shaders Dynamicly. This function can be used at run-time.
GLuint Shader_CompileProgramDynamic(ShaderDescriptor* args, int argsCount);

char* internal_ReadShaderSource(const char* path);
void internal_CompileShader(GLuint* shader, GLint type, char* path);
GLuint internal_Shader_CompileProgram(const ShaderDescriptor* args);


#ifdef __cplusplus
}
#endif

