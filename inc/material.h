#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


//Forward Declarations
struct HashTable;
struct Texture;

typedef struct Material {

    uint32_t TexturesUsed;
    Texture** Textures;

    HashTable* Uniforms;
    
    GLuint Program;
    GLenum CullFunction;
    GLenum DepthFunction;

} Material;



Material* Material_create(const char* vertexProgramPath, const char* fragmentProgramPath, const uint32_t numberOfTextures, const GLenum cullFuncton, const GLenum depthFunction);
void Material_destroy(Material** material);


void SetTextureFromPointer(const Material* material, Texture* texture, uint32_t index);
void SetTextureFromAlias(const Material* material, const char* alias, uint32_t index);
void BindMaterial(const Material* material);

#ifdef __cplusplus
}
#endif