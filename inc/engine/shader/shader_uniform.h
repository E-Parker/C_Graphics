#pragma once

#include "glad/glad.h"

// Forward declarations:
typedef struct HashTable HashTable;

// simple macro to get a pointer to the end of the alias.
#define Uniform_alias_end(uniform) (uniform->Alias + uniform->AliasLength + 1)

// using #define here to keep it the same as OpenGL.
#define UNIFORM_TYPE_GENERIC 0x0001
#define UNIFORM_TYPE_SINGLE 0x0002
#define UNIFORM_TYPE_SHARED 0x0003
#define UNIFORM_TYPE_STRUCT 0x0004
#define UNIFORM_TYPE_BUFFER 0x0005
#define UNIFORM_TYPE_SAMPLER 0x0006

typedef struct UniformGeneric {
    // Template body for any uniform type
    #define UNIFORM_BODY()\
    char* Alias;\
    uint16_t AliasLength;\
    uint16_t UniformType;\
    GLint Location;\
    
    UNIFORM_BODY()

} UniformGeneric;

typedef struct UniformInformation {
    UNIFORM_BODY()
    GLenum Type;
    GLint Elements;
    GLint BlockOffset;  // byte offset of the item in the block. (-1 if not in a block.)
 
} UniformInformation;

//  SHADER UNIFORM
//
//

typedef struct Uniform {
    // Structure to store an OpenGL Uniform. Same size as UniformBuffer struct
    UNIFORM_BODY()
    uint32_t Type;      // GLenum type of the uniform. Needed to do upload.
    uint32_t Offset;    // Only meaningful for uniforms that are part of structures.
    uint32_t Elements;  // value of 1 means it is not an array.
    uint64_t Size;      // byte size of each element.
    uint64_t Stride;    // zero unless the uniform is part of a structure.
    void* Data;
} Uniform;

Uniform* internal_Uniform_create_shared(const UniformInformation* info, void* sharedBuffer);
Uniform* internal_Uniform_create(const UniformInformation* info);

void internal_Uniform_set_data(Uniform* uniform, void* data);
void internal_Uniform_set_at(Uniform* uniform, int i, void* data);

#define Uniform_exec_if_type(UniformType, ...) ( if((Uniform*)uniform->UniformType == UniformType){ __VA_ARGS__ })
#define internal_Uniform_get_data(T, uniform) ((T*)((Uniform*)uniform->Data))
#define internal_Uniform_get_data_size(uniform) (uint64_t)(uniform->Size * uniform->Elements)
#define Uniform_set_data(uniform, data) (internal_Uniform_set_data(uniform, (void*)data))
#define Uniform_set_at(uniform, i, data) (internal_Uniform_set_at(uniform, i, (void*)data))

typedef struct UniformStruct {
    // similar to Uniform buffers, used to store an openGL struct.
    UNIFORM_BODY()
    uint32_t Offset;
    uint32_t Elements;
    uint64_t Size;      // total size of the struct.
    HashTable* Members; 
    void* Data;
} UniformStruct;

UniformStruct* internal_UniformStruct_create(char* alias, const uint16_t aliasLength, const UniformInformation* info, const uint16_t memberCount, const uint64_t elements, void* shared);

void UniformStruct_get_member(UniformStruct* uniformStruct, const char* alias, Uniform** outVal);
void UniformStruct_set_member_at(UniformStruct* uniformStruct, const char* alias, uint64_t i, void* data);
void UniformStruct_set_member(UniformStruct* uniformStruct, const char* alias, void* data);

//  SHADER UNIFORM BUFFER
//
//

typedef struct UniformBuffer {
    // Structure to store an OpenGL buffer. Same size as Uniform struct
    UNIFORM_BODY()
    uint64_t Size;          // Total byte size of the buffer.
    GLint BufferObject;
    uint32_t BindingIndex;  // binding index of the buffer on the GPU.
    uint32_t References;
    uint32_t ChangesMade;
    HashTable* Uniforms;
    HashTable* UniformStructs;
} UniformBuffer;

void UniformBuffer_destroy(UniformBuffer** buffer);
void internal_UniformBuffer_set_region(const UniformBuffer* buffer, const uint64_t byteIndex, const uint64_t regionSizeInBytes, const void* data);
void internal_UniformBuffer_set_all(const UniformBuffer* buffer, const void* data);
void internal_UniformBuffer_set(UniformBuffer* buffer, const char* alias, void* data);
void internal_UniformBuffer_buffer(const UniformBuffer* buffer);

void UniformBuffer_get_Uniform(const UniformBuffer* buffer, const char* alias, Uniform** outVal);
void UniformBuffer_get_Struct(const UniformBuffer* buffer, const char* alias, UniformStruct** outVal);

void internal_UniformBuffer_set_Struct(UniformBuffer* buffer, const char* alias, const char* memberAlias, void* data);
void internal_UniformBuffer_set_Struct_at(UniformBuffer* buffer, const char* alias, const char* memberAlias, int i, void* data);

UniformBuffer* UniformBuffer_get_self(const char* alias);
void UniformBuffer_update_all();

#define UniformBuffer_get_shared(buffer) ((void*)((uint8_t*)buffer + sizeof(UniformBuffer)))
#define UniformBuffer_set(buffer, alias, Value) (internal_UniformBuffer_set(buffer, alias, (void*)Value))
#define UniformBuffer_set_Global(bufferAlias, alias, Value) (UniformBuffer_set(UniformBuffer_get_self(bufferAlias), alias, Value))
#define UniformBuffer_set_Struct_Global(bufferAlias, structAlias, memberAlias, Value) (internal_UniformBuffer_set_Struct(UniformBuffer_get_self(bufferAlias), structAlias, memberAlias, Value))
#define UniformBuffer_set_Struct_at_Global(bufferAlias, structAlias, memberAlias, index, Value) (internal_UniformBuffer_set_Struct_at(UniformBuffer_get_self(bufferAlias), structAlias, memberAlias, index, Value))

