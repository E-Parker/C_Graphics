#pragma once

#include "engine_core/string.h"

typedef struct HashTable Hashtable;

typedef struct Texture {
    // Struct to hold graphics data for a texture. Avoid using this manually since the system cannot track it.
    GLuint  ID;
    GLenum  Type;
    u64     references;
} Texture;

typedef struct TextureDescriptor {
    bool flipVertical;
    bool flipHorizontal;
    GLenum textureType;
    GLenum filterType;
    GLenum mipLevel;
    GLenum format;
    GLenum wrapHorizontalType;
    GLenum wrapVerticalType;
    u32 pathCount;
    char** paths;
} TextureDescriptor;


void    InitTextures ();
ecode   DereferenceTextures ();

void    Texture_create(const char* alias, const TextureDescriptor descriptor);
void    Texture_delete(const char* alias);
void    Texture_delete_String(String alias);

void    internal_Texture_extend_alias (const char* alias, String* out);
void    Internal_Texture_upload (Texture* texture, const TextureDescriptor descriptor, int width, int height, int channels, u32 pathCount, void* data);

bool    Texture_get(const char* alias, Texture** outVal);
bool    Texture_get_String(String alias, Texture** outVal);
