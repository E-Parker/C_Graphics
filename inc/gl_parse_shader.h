#pragma once

#ifdef __cplusplus
extern "C" {
#endif

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
#define GL_SHADER_SOURCE_SIZE 0xffff 
#endif

typedef struct {
    // Struct to store shader reference, type, and file path. 
    // if path is empty, the shader is null. 
    GLuint shader;
    GLint type;
    char path[GL_SHADER_PATH_SIZE];
} ShaderDescriptor;

// Macro to compile and link any number of shaders.
// stinky smelly do-while false in macro lol.
#define Shader_CompileProgram(program, ...) do { ShaderDescriptor args[] = { __VA_ARGS__, {0, 0, ""} }; program = internal_Shader_CompileProgram(&args); } while(0)

char* internal_ReadShaderSource(const char* path);
//char* internal_LoadShaderIncludes(const char* path);

void internal_CompileShader(GLuint* shader, GLint type, char* path);
GLuint internal_Shader_CompileProgram(const ShaderDescriptor* args);


#ifdef __cplusplus
}
#endif

