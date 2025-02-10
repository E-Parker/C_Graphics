#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#ifdef GL_ERROR_LOG_SIZE
#else
#define GL_ERROR_LOG_SIZE 512
#endif

#ifdef GL_SHADER_PATH_SIZE
#else
#define GL_SHADER_PATH_SIZE 128 
#endif

#ifdef GL_SHADER_SOURCE_SIZE
#else
#define GL_ERROR_LOG_SIZE 0xffff 
#endif

typedef struct ShaderDescriptor {
    // Struct to store shader reference, type, and file path. 
    // if path is empty, the shader is null. 
    GLuint shader;
    GLint type;
    char path[GL_SHADER_PATH_SIZE];
} ShaderDescriptor;

// Macro to compile and link any number of shaders.
// sinky smelly do-while false in macro lol.
#define Shader_CompileProgram(...) do { ShaderDescriptor args[]{__VA_ARGS__, {0, 0, ""}}; internal_Shader_CompileProgram(&args); } while(0)

char* internal_ReadShaderSource(const char* path);
char* internal_LoadShaderIncludes(const char* path);

void internal_CompileShader(GLuint* shader, GLint type, const char* path);
GLuint internalShader_CompileProgram(ShaderDescriptor* args);


