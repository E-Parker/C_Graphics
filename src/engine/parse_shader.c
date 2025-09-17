#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "engine_core/string.h"
#include "engine_core/list.h"
#include "engine/parse_shader.h"

// Shader descriptor buffer for run-time shader compiling.
ShaderDescriptor descriptorBuffer[GL_SHADER_MAX_DESCRIPTORS]; 

// shader source buffer
char srcBuffer[GL_SHADER_SOURCE_BUFFER_SIZE];
bool srcBufferWall = false;
const GLchar* gl_srcBuffer = (GLchar*)srcBuffer;


void internal_ReadShaderSource(const char* path) {
   
    // Check that the last call to this function was handled correctly
    if (srcBufferWall) {
        printf("Shader load warning: Last source file was not handled / recived correctly.");
        srcBufferWall = false;
    }
    
    FILE* file = fopen(path, "r");
    
    // Return if the file could not be found.
    if (!file) {
        printf("Shader load error: source file \"%s\" not found or inaccessible.", path);
        srcBufferWall = false;
        return;
    }
    
    // Get the size of the file.
    fseek(file, 0, SEEK_END);
    uint64_t bufferSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Return early if the buffer isn't big enough to fit the file.
    if (bufferSize > GL_SHADER_SOURCE_BUFFER_SIZE) {
        printf("Shader load error: source file \"%s\" was too large to fit in a buffer. (%u),", path, GL_SHADER_SOURCE_BUFFER_SIZE);
        fclose(file);
        return;
    }
    
    fread((char*)&srcBuffer, 1, bufferSize, file);
    int errorCode = ferror(file);
    fclose(file);
    
    // Force the last index to null so open gl doesnt explode trying to parse it later.
    srcBuffer[bufferSize] = '\0';

    // Return the error if there was an stdio error reading the file.
    if (errorCode) {
        printf("Shader load error: Something went wrong reading file, \"%s\". Discarding shader. Error code was, (%d)", path, errorCode);
        return;
    }
    
    // Only give the rest of the program the go-ahead if the shader was loaded correctly.
    srcBufferWall = true;
}


void internal_CompileShader(GLuint* shader, GLint type, const char* path) {
    // Takes an empty shader*, type of shader, and path to source file. 
    // Reads the path to a c-string and compiles the shader for OpenGL.
    // 
    
    internal_ReadShaderSource(path);
    
    // if reading the shader failed, return early.
    if (!srcBufferWall) {
        shader = GL_NONE;
        return;
    }

    // Compile text as a shader
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &gl_srcBuffer, NULL);
    glCompileShader(*shader);
    
    // File was handled sucessfully so update wall.
    srcBufferWall = false;
}


bool Shader_CompileProgramValidate(ShaderDescriptor* args) {
    // Step through args untill the max descriptors is hit, or untill the end of the list is found.
    
    for (int i = 0; i <= GL_SHADER_MAX_DESCRIPTORS; i++) {
        if((args[i]).path[0] == '\0') {
            return true;  
        }
    }
    
    // Create a log that is big enough to hold the longest possible file path for all descriptors. 
    char log[(GL_SHADER_MAX_DESCRIPTORS * (GL_SHADER_PATH_SIZE + 1)) + 1];
    
    for (int i = 0; i <= GL_SHADER_MAX_DESCRIPTORS; i++) {
        strcat(log, (args[i]).path);
        strcat(log, "\n");
    }
    
    printf("\n======================= START OF LOG =======================\n\
            Shader compile error: A ShaderDiscriptor list was too large or malformed!\n\
            The args provided were:\n%s\
            \n======================== END OF LOG ========================\n\
            ", log);
    return false;
}


GLuint Shader_CompileProgramDynamic(ShaderDescriptor* args, int argsCount) {
    // Expects an array of arguments, adds the null terminator depending on argsCount.
    
    // Check that 
    if(argsCount <= 0) return GL_NONE;
    if(argsCount > GL_SHADER_MAX_DESCRIPTORS) argsCount = GL_SHADER_MAX_DESCRIPTORS; 
    
    memcpy(args, descriptorBuffer, argsCount);
    
    descriptorBuffer[argsCount - 1].shader = 0;
    descriptorBuffer[argsCount - 1].type = 0;
    descriptorBuffer[argsCount - 1].path = "";

    return internal_Shader_CompileProgram((ShaderDescriptor*)&descriptorBuffer);
}

// TODO: remove this function. it has literally one caller. See two lines up ^^^ 
GLuint internal_Shader_CompileProgram(ShaderDescriptor* args) {
    // Expects an array of arguments with a null terminated end. 
    
    // Validate args:
    if(!Shader_CompileProgramValidate(args)) return GL_NONE;

    // Set up a new shader program and compile it.
    GLuint program = glCreateProgram(); // Create a new empty program.

    // Iterate until end of args. Compile and attach each shader.
    for (ShaderDescriptor* it = args; it->path[0] != '\0'; it++) {
        internal_CompileShader(&it->shader, it->type, it->path);
        glAttachShader(program, it->shader);
    }

    glLinkProgram(program);             // Run the linking step.

    // Find out if there were errors linking the shader program.
    int success;
    int s_success;

    int dummyLength;
    char log[GL_ERROR_LOG_SIZE] = {'\0'};
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    
    // No errors, delete all the temp shaders and return the program. 
    if (success) {
        for (ShaderDescriptor* it = (ShaderDescriptor*)args; it->path[0] != '\0'; it++) {
            glDeleteShader(it->shader);
        }
        return program;
    }

    // Shader program compile / link failed for some reason, clear the program and log the error.
    glGetProgramInfoLog(program, GL_ERROR_LOG_SIZE, &dummyLength, log);

    printf("\n======================= START OF LOG =======================\n\
            Shader compile error: Could not link shader program.\n%s\n\
            Log for individual shader(s):\n",\
            log);
    
    // Debug the individual shaders.
    for (ShaderDescriptor* it = (ShaderDescriptor*)args; it->path[0] != '\0'; it++) {
        glGetShaderiv(it->shader, GL_COMPILE_STATUS, &s_success);
        
        // If this shader passed, free it and continue loop early.
        if(s_success) {
            glDeleteShader(it->shader);
            continue;
        }

        // Otherwise, clear the log, get the log from OpenGL and write it to the output. 
        memset(&log, '\0', GL_ERROR_LOG_SIZE);
        glGetShaderInfoLog(it->shader, GL_ERROR_LOG_SIZE, &dummyLength, log);
        
        // Display the shader path, type, and error log for the shader which failed.
        printf("Shader Path:\n%s\n", it->path);

        switch (it->type) {
            case GL_VERTEX_SHADER: printf("Failure in Vertex shader:\n%s", log);
            case GL_TESS_CONTROL_SHADER: printf("Failure in Tessellation shader:\n%s", log);
            case GL_TESS_EVALUATION_SHADER: printf("Failure in Evaluation shader:\n%s", log);
            case GL_GEOMETRY_SHADER: printf("Failure in Geometry shader:\n%s", log);
            case GL_FRAGMENT_SHADER: printf("Failure in Fragment shader:\n%s", log);
            case GL_COMPUTE_SHADER: printf("Failure in Compute shader:\n%s", log);
            default: printf("Unknown shader type.\n%s", log);
        }
    }
    printf("\n======================== END OF LOG ========================\n");
    return GL_NONE;
}


