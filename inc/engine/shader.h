#pragma once

#include "glad/glad.h"

#include "engine/shader/parse_shader.h"

#include "engine_core/string.h"

// Forward declarations:
typedef struct HashTable HashTable;
typedef struct Uniform Uniform;
typedef struct UniformBuffer UniformBuffer;

typedef struct Shader {
    GLuint program;             // Location of the shader program on the GPU.
    HashTable* uniforms;        // Table of uniforms.
    HashTable* uniformBuffers;  // Table of uniform buffers.
    u64 references;
} Shader;

void InitShaders();
void DereferenceShaders();

void Shader_use(const Shader* shader);

GLint internal_Program_uniform_count(const GLuint program);
GLint internal_Program_buffer_count(const GLuint program);
void internal_Program_uniform_parse(const GLuint program, HashTable* table);
void internal_Program_buffer_parse(const GLuint program, HashTable* table);
void internal_Program_buffer_uniform_parse(const GLuint program, const u16 uniformCount, const GLint* indicies, UniformBuffer* uniformBuffer);
void internal_program_uniformStruct_parse(const GLuint program, const u16 uniformCount, GLint* indicies, UniformBuffer* uniformBuffer);

#define Shader_create(alias, ...)  do { ShaderDescriptor args[] = { __VA_ARGS__, {"", 0, 0} }; internal_Shader_create(internal_ShaderProgram_CompileProgram(args), alias); } while(false) 

void internal_Shader_create(const GLuint program, const char* alias);
void Shader_delete (const char* alias);
void Shader_delete_String (String alias);

void Shader_deinitialize (Shader* shader);

Shader* Shader_get (const char* alias);
Shader* Shader_get_String (String alias);

void Shader_set_uniform(const Shader* shader, const char* alias, void* data);
void Shader_set_uniformBuffer(const Shader* shader, const char* alias, void* data);

void Shader_get_uniform(const Shader* shader, const char* alias, Uniform** outVal);
void Shader_get_uniformBuffer(const Shader* shader, const char* alias, UniformBuffer** outVal);

#define Shader_get_uniform_count(shader) (shader->Uniforms->SlotsUsed)
#define Shader_get_buffer_count(shader) (shader->UniformBuffers->SlotsUsed)

void Shader_debug(const GLuint program);
