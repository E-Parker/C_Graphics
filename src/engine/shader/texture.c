#include "glad/glad.h"
#include "stb_image.h"

#include "stdio.h"
#include "string.h"

#include "engine_core/engine_types.h"
#include "engine_core/engine_error.h"
#include "engine_core/hash_table.h"
#include "engine/shader/texture.h"


HashTable TextureTable;


bool Texture_get(const char* alias, Texture** outVal) {
    return HashTable_find_reference(&TextureTable, (String) String_from_ptr(alias), outVal);
}

bool Texture_get_String(String alias, Texture** outVal) {
    return HashTable_find_reference(&TextureTable, alias, outVal);
}


void InitTextures() {
    stbi_set_flip_vertically_on_load(true); // For some reason, if you flip the order, it corrupts TextureTable. EVIL. BAD. BAD FUNCTION. 
    HashTable_initialize(Texture, &TextureTable, 512);
}

ecode DereferenceTextures() {
    // Call this function at the end of your program to ensure all tracked textures are properly cleaned up.
    for (HashTable_array_iterator(&TextureTable)) {
        Texture* texture = HashTable_array_at(Texture, &TextureTable, i);
        if (texture->ID != GL_NONE) {
            glDeleteTextures(1, &(texture->ID));
        }
    }
    HashTable_deinitialize(&TextureTable);
    return 0;
}


void internal_Texture_extend_alias (const char* alias, String* out) {
    String aliasAsString = String_from_ptr(alias);
    u64 aliasLength = String_length(aliasAsString);
    char* extendedAliasBuffer = (char*)calloc(aliasLength + 65, 1);
    Engine_validate(extendedAliasBuffer, ENOMEM);
    String_clone_to_ptr(aliasAsString, extendedAliasBuffer);

    aliasAsString.start = extendedAliasBuffer;
    aliasAsString.end = extendedAliasBuffer + aliasLength;
    char* extendedAlias = extendedAliasBuffer + aliasLength;

    Texture texture;
    Texture* textureptr = &texture;
    u64 count = 0;
    while (HashTable_find(&TextureTable, aliasAsString, texture)) {
        count++;
        sprintf(extendedAlias, "(%llu)\0", count);
        aliasAsString.end = FindBufferEnd(aliasAsString.start) - 1;
    }

    out->start = aliasAsString.start;
    out->end = aliasAsString.end;
}

void Texture_create(const char* alias, const TextureDescriptor descriptor) {
    // If only one texture is specified, no need to specify a path count. This way, if you forget at least something will happen.
    u32 pathCount = descriptor.pathCount;
    if (!pathCount && descriptor.paths) {
        pathCount = 1;
    }

    // Create and upload the textures.
    int width, height, channels;
    Texture texture = { .Type = descriptor.textureType, .ID = GL_NONE, .references = 0 };
    String aliasString;
    internal_Texture_extend_alias(alias, &aliasString);

    for (u64 i = 0; i < pathCount; ++i) {
        void* data = stbi_load(descriptor.paths[i], &width, &height, &channels, 0);

        if (!data) {
            String_free_dirty(&aliasString);
            return;
        }

        Internal_Texture_upload(&texture, descriptor, width, height, channels, pathCount, data);
        stbi_image_free(data);
    }
    HashTable_insert(&TextureTable, aliasString, &texture);
    String_free_dirty(&aliasString);
}


void Texture_delete(const char* alias) {
    Texture_delete_String((String) String_from_ptr(alias));
}

void Texture_delete_String(String alias) {
    Texture* texture;

    if (!HashTable_find_reference(&TextureTable, alias, &texture)) {
        Engine_log("Error deleting Texture: \"%s\". No Texture with that name found.\n", alias.start);
        return;
    }

    if (texture->references > 1) {
        texture->references--;
        return;
    }

    if (texture->ID != GL_NONE) {
        glDeleteTextures(1, &(texture->ID));
    }

    texture->ID = GL_NONE;
    HashTable_remove(&TextureTable, alias);
}


void Internal_Texture_upload (Texture* texture, const TextureDescriptor descriptor, int width, int height, int channels, u32 pathCount, void* data) {
    switch (descriptor.textureType) {
        case GL_TEXTURE_BUFFER: return;
        case GL_TEXTURE_RECTANGLE: return;
        case GL_TEXTURE_2D_MULTISAMPLE: return;
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY: return;
        case GL_TEXTURE_3D: return;
    }

    if (texture->ID == GL_NONE) {
        glGenTextures(pathCount, &(texture->ID));
    }

    glBindTexture(descriptor.textureType, texture->ID);
    glTextureParameteri(texture->ID, GL_TEXTURE_WRAP_S, descriptor.wrapHorizontalType ? descriptor.wrapHorizontalType : GL_REPEAT);
    glTextureParameteri(texture->ID, GL_TEXTURE_WRAP_T, descriptor.wrapVerticalType ? descriptor.wrapVerticalType : GL_REPEAT);
    glTextureParameteri(texture->ID, GL_TEXTURE_MIN_FILTER, descriptor.filterType ? descriptor.filterType : GL_LINEAR);
    glTextureParameteri(texture->ID, GL_TEXTURE_MAG_FILTER, descriptor.filterType ? descriptor.filterType : GL_LINEAR);
    
    GLint externalFormat = GL_RGB;
    switch (channels) {
        case 1: externalFormat = GL_RED; break;
        case 2: externalFormat = GL_RG; break;
        case 3: externalFormat = GL_RGB; break;
        case 4: externalFormat = GL_RGBA; break;
    }

    switch (descriptor.textureType) {
        case GL_TEXTURE_1D:             glTexImage1D(descriptor.textureType, descriptor.mipLevel, descriptor.format, width, 0, externalFormat, GL_UNSIGNED_BYTE, data); break;
        case GL_TEXTURE_1D_ARRAY:       glTexImage1D(descriptor.textureType, descriptor.mipLevel, descriptor.format, width, 0, externalFormat, GL_UNSIGNED_BYTE, data); break;
        case GL_TEXTURE_2D:             glTexImage2D(descriptor.textureType, descriptor.mipLevel, descriptor.format, width, height, 0, externalFormat, GL_UNSIGNED_BYTE, data); break;
        case GL_TEXTURE_2D_ARRAY:       glTexImage2D(descriptor.textureType, descriptor.mipLevel, descriptor.format, width, height, 0, externalFormat, GL_UNSIGNED_BYTE, data); break;
        case GL_TEXTURE_CUBE_MAP:       glTexImage2D(descriptor.textureType, descriptor.mipLevel, descriptor.format, width, height, 0, externalFormat, GL_UNSIGNED_BYTE, data); break;
        case GL_TEXTURE_CUBE_MAP_ARRAY: glTexImage2D(descriptor.textureType, descriptor.mipLevel, descriptor.format, width, height, 0, externalFormat, GL_UNSIGNED_BYTE, data); break;
    }


    if (descriptor.filterType == GL_NEAREST_MIPMAP_LINEAR || descriptor.filterType == GL_NEAREST_MIPMAP_NEAREST) {
        glGenerateMipmap(descriptor.textureType);
    }

}

