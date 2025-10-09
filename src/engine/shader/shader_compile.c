#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "string.h"

#include "engine_core/configuation.h"
#include "engine_core/engine_types.h"
#include "engine_core/engine_error.h"
#include "engine_core/engine_io.h"
#include "engine_core/string.h"
#include "engine_core/list.h"

#include "engine/shader/shader_compile.h"

// Shader descriptor buffer for run-time shader compiling.
ShaderDescriptor descriptorBuffer[GL_SHADER_MAX_DESCRIPTORS]; 

// shader source buffer
char srcBuffer[GL_SHADER_SOURCE_BUFFER_SIZE];
bool srcBufferWall = false;
const GLchar* gl_srcBuffer = (GLchar*)srcBuffer;


void internal_ReadShaderProgramSource(const char* path) {
    if (srcBufferWall) {
        Engine_log("Shader load warning: Last source file was not handled / received correctly.\n");
    }
    
    FILE* file = fopen(path, "r");

    ecode errorCode = Engine_read((IODescriptor) {
        .file = file,
        .buffer = srcBuffer,
        .bufferSize = sizeof(srcBuffer) / sizeof(char),
        .flags = ENGINE_IO_ZERO_BUFFER | ENGINE_IO_ADD_NULL_TERMINATOR
    });

    if (file) {
        fclose(file);
    }
    
    if (!errorCode) {
        srcBufferWall = true;
        return;
    }

    srcBufferWall = false;
    switch (errorCode) {
    case ERROR_BADPOINTER: 
        Engine_log("Shader load error: source file \"%s\" not found or inaccessible.\n", path);
        break;
    
    case ERROR_IO_FILETOOBIG:
        Engine_log("Shader load error: source file \"%s\" was too large to fit in a buffer.\n", path);
        break;

    default:
        Engine_log("Shader load error: Something went wrong reading file, \"%s\". Discarding shader.\n", path);
        Engine_log_errorcode(errorCode);
        break;
    }
}


void internal_CompileShaderProgram(GLuint* shader, GLint type, const char* path) {
    // Takes an empty shader*, type of shader, and path to source file. 
    // Reads the path to a c-string and compiles the shader for OpenGL.
    // 
    
    internal_ReadShaderProgramSource(path);
    
    // if reading the shader failed, return early.
    if (!srcBufferWall) {
        *shader = GL_NONE;
        return;
    }

    // Compile text as a shader
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &gl_srcBuffer, NULL);
    glCompileShader(*shader);
    
    // File was handled successfully so update wall.
    srcBufferWall = false;
}


bool ShaderProgram_CompileProgramValidate(ShaderDescriptor* args) {
    // Step through args until the max descriptors is hit, or until the end of the list is found.
    for (u64 i = 0; i <= GL_SHADER_MAX_DESCRIPTORS; ++i) {
        if((args[i]).path[0] == '\0') {
            return true;  
        }
    }
    
    // Create a log that is big enough to hold the longest possible file path for all descriptors.
    char log[(GL_SHADER_MAX_DESCRIPTORS * (GL_SHADER_PATH_SIZE + 1)) + 1];
    
    for (u64 i = 0; i < GL_SHADER_MAX_DESCRIPTORS; ++i) {
        strcat(log, (args[i]).path);
        strcat(log, "\n");
    }
    
    Engine_log("\n======================= START OF LOG =======================\n\
            Shader compile error: A ShaderDiscriptor list was too large or malformed!\n\
            The args provided were:\n%s\
            \n======================== END OF LOG ========================\n\
            ", log);
    return false;
}


GLuint ShaderProgram_CompileProgramDynamic(ShaderDescriptor* args, int argsCount) {
    // Expects an array of arguments, adds the null terminator depending on argsCount.
    
    // Check that 
    if(argsCount <= 0) return GL_NONE;
    if(argsCount > GL_SHADER_MAX_DESCRIPTORS) argsCount = GL_SHADER_MAX_DESCRIPTORS; 
    
    memcpy(args, descriptorBuffer, argsCount);
    
    descriptorBuffer[argsCount - 1].path = "";
    descriptorBuffer[argsCount - 1].type = 0;
    descriptorBuffer[argsCount - 1].shader = 0;

    return internal_ShaderProgram_CompileProgram((ShaderDescriptor*)&descriptorBuffer);
}


GLuint internal_ShaderProgram_CompileProgram(ShaderDescriptor* args) {
    // Expects an array of arguments with a null terminated end. 
    
    // Validate args:
    if(!ShaderProgram_CompileProgramValidate(args)) return GL_NONE;

    // Set up a new shader program and compile it.
    GLuint program = glCreateProgram(); // Create a new empty program.

    // Iterate until end of args. Compile and attach each shader.
    for (ShaderDescriptor* it = args; it->path[0] != '\0'; it++) {
        internal_CompileShaderProgram(&it->shader, it->type, it->path);
        if (it->shader == GL_NONE) continue;
        glAttachShader(program, it->shader);
    }

    glLinkProgram(program);

    int success;
    int s_success;
    int dummyLength;
    char log[GL_ERROR_LOG_SIZE] = {'\0',};
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    
    // No errors, delete all the temp shaders and return the program. 
    if (success) {
        for (ShaderDescriptor* it = (ShaderDescriptor*)args; it->path[0] != '\0'; it++) {
            if (it->shader == GL_NONE) continue;
            glDeleteShader(it->shader);
        }
        return program;
    }

    // Shader program compile / link failed for some reason, clear the program and log the error.
    glGetProgramInfoLog(program, GL_ERROR_LOG_SIZE, &dummyLength, log);

    Engine_log("\n======================= START OF LOG =======================\n\
            Shader compile error: Could not link shader program.\n%s\n\
            Log for individual shader(s):\n",\
            log);
    
    // Debug the individual shaders.
    for (ShaderDescriptor* it = (ShaderDescriptor*)args; it->path[0] != '\0'; it++) {

        // Shader failed to load.
        if (it->shader == GL_NONE) {
            continue;
        }
        
        glGetShaderiv(it->shader, GL_COMPILE_STATUS, &s_success);
        
        // If this shader passed, free it and continue loop early.
        if(s_success) {
            glDeleteShader(it->shader);
            continue;
        }

        // Otherwise, get the log from OpenGL and write it to the output. 
        glGetShaderInfoLog(it->shader, GL_ERROR_LOG_SIZE, &dummyLength, log);
        
        // Display the shader path, type, and error log for the shader which failed.
        PrintLog:
        memset(&log, '\0', GL_ERROR_LOG_SIZE);
        Engine_log("Shader Path:\n%s\n", it->path);

        switch (it->type) {
            case GL_VERTEX_SHADER: Engine_log("Failure in Vertex shader:\n%s", log);
            case GL_TESS_CONTROL_SHADER: Engine_log("Failure in Tessellation shader:\n%s", log);
            case GL_TESS_EVALUATION_SHADER: Engine_log("Failure in Evaluation shader:\n%s", log);
            case GL_GEOMETRY_SHADER: Engine_log("Failure in Geometry shader:\n%s", log);
            case GL_FRAGMENT_SHADER: Engine_log("Failure in Fragment shader:\n%s", log);
            case GL_COMPUTE_SHADER: Engine_log("Failure in Compute shader:\n%s", log);
            default: Engine_log("Unknown shader type.\n%s", log);
        }
    }
    Engine_log("\n======================== END OF LOG ========================\n");
    return GL_NONE;
}


