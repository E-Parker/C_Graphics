#pragma once

typedef struct HashTable Hashtable;

typedef struct Texture {
    // Struct to hold graphics data for a texture. Avoid using this manually since the system cannot track it.
    char*   alias;
    GLuint  ID;
    GLenum  type;
    u64     references;
    i32     width;
    i32     height;
    i32     filterType;
    i32     channels;

} Texture;

void InitTextures();

void InternalUploadTexture(Texture* texture, u8* data, GLenum internalFormat, GLenum format, GLenum uploadType);
void InternalUploadTextureMimmap(Texture* texture, u8* data, GLenum internalFormat, GLenum format, GLenum uploadType);
void InternalDeleteTexture(Texture* texture);
void InternalCreateTexture(Texture* texture, const bool isManaged, const char* alias, const GLenum internalFormat, const GLenum format, u8* data, const bool useMipmaps);
void CreateRawTexture(const char* path, Texture* texture, GLenum internalFormat, GLenum format, bool flipVertical, bool flipHorizontal, bool useMipmaps, int filterType);

bool FindTexture(const char* alias, Texture** outValue);

void DereferenceTextures();
void DeleteTexture(const char* alias);
Texture* CreateTexture(const char* path, const char* alias, GLenum internalFormat, bool flipVertical, bool flipHorizontal, bool useMipmaps, int filterType);
Texture* CreateCubemapTexture(const char* texturePaths[6], const char* alias, GLenum internalFormat, bool flipVertical, bool flipHorizontal, bool useMipmaps, int filterType);

