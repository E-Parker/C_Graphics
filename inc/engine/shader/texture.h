#pragma once

#include "glad/glad.h"

#include "engine_core/engine_types.h"
#include "engine_core/string.h"

typedef struct Texture {
    // Struct to hold graphics data for a texture. Avoid using this manually since the system cannot track it.
    u32 ID;
    u32 Type;
    u64 references;
} Texture;

typedef struct TextureDescriptor {
    bool flipVertical;
    bool flipHorizontal;
    u32 textureType;
    u32 filterType;
    u32 mipLevel;
    u32 format;
    u32 wrapHorizontalType;
    u32 wrapVerticalType;
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
