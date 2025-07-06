#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "bool.h"
#include "cStringUtilities.h"
#include "gl_parse_shader.h"


char* internal_ReadShaderSource(const char* path) {
    
    FILE* file = fopen(path, "r");
    
    // Return null if the file could not be found.
    if (!file) {
        printf("Shader load error: source file \"%s\" not found or inaccessible.", path);
        return NULL;
    }
    
    // Get the raw file as a c-string to compile
    char* buffer = (char*)malloc(GL_SHADER_SOURCE_SIZE);  
    fgets(buffer, GL_SHADER_SOURCE_SIZE, file);
    int errorCode = ferror(file);
    fclose(file);
    file = NULL;

    // Return null if read error.
    if (errorCode) {
        printf("Shader load error: Something went wrong reading file, \"%s\". Discarding shader.", path);
        free(buffer);
        return NULL;
    }

    char* bufferEnd = FindBufferEnd(buffer);
    
    // Return null if the buffer was not large enough.
    if(!bufferEnd) {
        printf("Shader load error: source file \"%s\" was to big to fit in a buffer. (%u),", path, GL_SHADER_SOURCE_SIZE);
        free(buffer);
        return NULL;
    }
    
    // Copy the buffer to a smaller c-string just big enough to fit it.
    uint64_t bufferSize = (uint64_t)(bufferEnd - buffer) + 1; 
    char* src = (char*)malloc(bufferSize);
    memcpy(buffer, src, bufferSize);
    free(buffer);
    return src;
}


void internal_CompileShader(GLuint* shader, GLint type, char* path) {
    // Takes an empty shader*, type of shader, and path to source file. 
    // Reads the path to a c-string and compiles the shader for OpenGL.
    // 
    
    char* src = internal_ReadShaderSource(path);
    const GLchar* gl_src = (GLchar*)src;

    if(!src) {
        *shader = GL_NONE;
        return;
    }
    
    // Compile text as a shader
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &gl_src, NULL);
    glCompileShader(*shader);
    free(src);
    // Don't check for errors. Shader_CompileProgram handles displaying errors.
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
    
    ShaderDescriptor* TerminatedArgs = (ShaderDescriptor*)calloc(argsCount + 1, sizeof(ShaderDescriptor));
    memcpy(args, TerminatedArgs, argsCount);

    return internal_Shader_CompileProgram(TerminatedArgs);
}


GLuint internal_Shader_CompileProgram(const ShaderDescriptor* args) {
    // Expects an array of arguments with a null terminated end. 
    
    // Validate args:
    if(!Shader_CompileProgramValidate((ShaderDescriptor*)args)) return GL_NONE;

    // Set up a new shader program and compile it.
    GLuint program = glCreateProgram(); // Create a new empty program.

    // Iterate until end of args. Compile and attach each shader.
    for (ShaderDescriptor* it = (ShaderDescriptor*)args; it->path[0] != '\0'; it++) {
        internal_CompileShader((GLuint*)&it->shader, it->type, (char*)&it->path);
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
        
        // Display the shader type and error log for the shader which failed.
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


