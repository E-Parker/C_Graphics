#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>

typedef struct HashTable Hashtable;

typedef struct Texture {
    // Struct to hold graphics data for a texture. Avoid using this manually since the system cannot track it.

    char* alias;
    GLuint ID;
    GLenum type;
    uint64_t references;
    int width;
    int height;
    int filterType;
    int channels;

} Texture;

#define internal_Texture_create(texture, gl_type, filter_type)\
do {\
texture = (Texture*)calloc(1, sizeof(Texture));\
texture->ID = GL_NONE;\
texture->type = gl_type;\
texture->filterType = filter_type;\
} while(false)\

void InitTextures();

void InternalUploadTexture(Texture* texture, uint8_t* data, GLenum internalFormat, GLenum format, GLenum uploadType);
void InternalUploadTextureMimmap(Texture* texture, uint8_t* data, GLenum internalFormat, GLenum format, GLenum uploadType);
void InternalDeleteTexture(Texture* texture);
void InternalCreateTexture(Texture* texture, const bool isManaged, const char* alias, const GLenum internalFormat, const GLenum format, uint8_t* data, const bool useMipmaps);
void CreateRawTexture(const char* path, Texture* texture, GLenum internalFormat, GLenum format, bool flipVertical, bool flipHorizontal, bool useMipmaps, int filterType);

bool FindTexture(const char* alias, Texture** outValue);


void DereferenceTextures();
void DeleteTexture(const char* alias);
Texture* CreateTexture(const char* path, const char* alias, GLenum internalFormat, bool flipVertical, bool flipHorizontal, bool useMipmaps, int filterType);
Texture* CreateCubemapTexture(const char* texturePaths[6], const char* alias, GLenum internalFormat, bool flipVertical, bool flipHorizontal, bool useMipmaps, int filterType);


#ifdef __cplusplus
}
#endif
