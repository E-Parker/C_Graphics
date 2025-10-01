#pragma once

#include "glad/glad.h"

// Forward declarations:
typedef struct HashTable HashTable;

// simple macro to get a pointer to the end of the alias.
#define Uniform_alias_end(uniform) (uniform->Alias + uniform->AliasLength + 1)

// using #define here to keep it the same as OpenGL.
#define UNIFORM_TYPE_GENERIC 1
#define UNIFORM_TYPE_SINGLE 2
#define UNIFORM_TYPE_SHARED 3
#define UNIFORM_TYPE_STRUCT 4
#define UNIFORM_TYPE_BUFFER 5
#define UNIFORM_TYPE_SAMPLER 6

typedef struct UniformGeneric {
    // Template body for any uniform type
    #define UNIFORM_BODY()\
    String Alias;\
    GLint Location;\
    u8 UniformType;\
    
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
    u32 Type;      // GLenum type of the uniform. Needed to do upload.
    u32 Offset;    // Only meaningful for uniforms that are part of structures.
    u32 Elements;  // value of 1 means it is not an array.
    u64 Size;      // byte size of each element.
    u64 Stride;    // zero unless the uniform is part of a structure.
    void* Data;
} Uniform;

Uniform* internal_Uniform_create_shared(const UniformInformation* info, void* sharedBuffer);
Uniform* internal_Uniform_create(const UniformInformation* info);

void internal_Uniform_set_data(Uniform* uniform, void* data);
void internal_Uniform_set_at(Uniform* uniform, int i, void* data);

#define Uniform_exec_if_type(UniformType, ...) ( if((Uniform*)uniform->UniformType == UniformType){ __VA_ARGS__ })
#define internal_Uniform_get_data(T, uniform) ((T*)((Uniform*)uniform->Data))
#define internal_Uniform_get_data_size(uniform) (u64)(uniform->Size * uniform->Elements)
#define Uniform_set_data(uniform, data) (internal_Uniform_set_data(uniform, (void*)data))
#define Uniform_set_at(uniform, i, data) (internal_Uniform_set_at(uniform, i, (void*)data))

typedef struct UniformStruct {
    // similar to Uniform buffers, used to store an openGL struct.
    UNIFORM_BODY()
    u32 Offset;
    u32 Elements;
    u64 Size;      // total size of the struct.
    HashTable* Members; 
    void* Data;
} UniformStruct;

UniformStruct* internal_UniformStruct_create(String alias, const UniformInformation* info, const u16 memberCount, const u64 elements, void* shared);

void UniformStruct_get_member(UniformStruct* uniformStruct, const char* alias, Uniform** outVal);
void UniformStruct_set_member_at(UniformStruct* uniformStruct, const char* alias, u64 i, void* data);
void UniformStruct_set_member(UniformStruct* uniformStruct, const char* alias, void* data);

//  SHADER UNIFORM BUFFER
//
//

typedef struct UniformBuffer {
    // Structure to store an OpenGL buffer. Same size as Uniform struct
    UNIFORM_BODY()
    u64 Size;          // Total byte size of the buffer.
    GLint BufferObject;
    u32 BindingIndex;  // binding index of the buffer on the GPU.
    u32 References;
    u32 ChangesMade;
    HashTable* Uniforms;
    HashTable* UniformStructs;
    u8* buffer;
} UniformBuffer;

void UniformBuffer_deinitialize(UniformBuffer* buffer);
void internal_UniformBuffer_set_region(const UniformBuffer* buffer, const u64 byteIndex, const u64 regionSizeInBytes, const void* data);
void internal_UniformBuffer_set_all(const UniformBuffer* buffer, const void* data);
void internal_UniformBuffer_set(UniformBuffer* buffer, const char* alias, void* data);
void internal_UniformBuffer_buffer(const UniformBuffer* buffer);

void UniformBuffer_get_Uniform(const UniformBuffer* buffer, const char* alias, Uniform** outVal);
void UniformBuffer_get_Struct(const UniformBuffer* buffer, const char* alias, UniformStruct** outVal);

void internal_UniformBuffer_set_Struct(UniformBuffer* buffer, const char* alias, const char* memberAlias, void* data);
void internal_UniformBuffer_set_Struct_at(UniformBuffer* buffer, const char* alias, const char* memberAlias, u64 i, void* data);

UniformBuffer* UniformBuffer_get_self(const char* alias);
void UniformBuffer_update_all();

#define UniformBuffer_set(buffer, alias, Value) (internal_UniformBuffer_set(buffer, alias, (void*)Value))
#define UniformBuffer_set_Global(bufferAlias, alias, Value) (UniformBuffer_set(UniformBuffer_get_self(bufferAlias), alias, Value))
#define UniformBuffer_set_Struct_Global(bufferAlias, structAlias, memberAlias, Value) (internal_UniformBuffer_set_Struct(UniformBuffer_get_self(bufferAlias), structAlias, memberAlias, Value))
#define UniformBuffer_set_Struct_at_Global(bufferAlias, structAlias, memberAlias, index, Value) (internal_UniformBuffer_set_Struct_at(UniformBuffer_get_self(bufferAlias), structAlias, memberAlias, index, Value))

