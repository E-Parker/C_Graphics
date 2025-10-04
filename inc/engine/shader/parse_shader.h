#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define GL_SHADER_MAX_DESCRIPTORS 0x40 
#define GL_ERROR_LOG_SIZE 0x1000
#define GL_SHADER_PATH_SIZE 0x80 
#define GL_SHADER_SOURCE_BUFFER_SIZE 0x8000

typedef struct ShaderDescriptor {
    const char* path;
    GLint type;
    GLuint shader; // TODO: See about getting rid of this. it's a temp value that is overwritten by the compile process.
} ShaderDescriptor;

// Macro to compile and link any number of shaders. Use this for compile-time constant since this guarantees there is no size miss-match.
//#define ShaderProgram_CompileProgram(program, ...) do { ShaderDescriptor args[] = { __VA_ARGS__, {"", 0, 0} }; program = internal_ShaderProgram_CompileProgram(args); } while(0)

// Compile and link any number of shaders Dynamically. This function can be used at run-time.
GLuint ShaderProgram_CompileProgramDynamic(ShaderDescriptor* args, int argsCount);

void internal_ReadShaderProgramSource(const char* path);
void internal_CompileShaderProgram(GLuint* shader, GLint type, const char* path);
GLuint internal_ShaderProgram_CompileProgram(ShaderDescriptor* args);

