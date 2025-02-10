#pragma once

//Forward Declarations
struct HashTable;
struct Texture;

typedef struct Material {

public:

    uint32_t TexturesUsed = 0;
    Texture** Textures;

    HashTable* Uniforms;
    
    GLuint Program = GL_NONE;
    GLenum CullFunction = GL_BACK;
    GLenum DepthFunction = GL_LESS;

    Material(const char* vertexProgramPath, const char* fragmentProgramPath, const uint32_t numberOfTextures, const GLenum cullFuncton, const GLenum depthFunction);
    ~Material();

} Material;

void SetTextureFromPointer(const Material* material, Texture* texture, uint32_t index);
void SetTextureFromAlias(const Material* material, const char* alias, uint32_t index);
void BindMaterial(const Material* material);
