#pragma once

#include <stdint.h>

// Macro defs:
#ifndef MATERIAL_BUFFER_SIZE
#define MATERIAL_BUFFER_SIZE 0xff
#endif

// Forward Declarations
typedef struct HashTable HashTable;
typedef struct Texture Texture;

typedef struct Material {
    HashTable* Uniforms;
    Texture** Textures;
    uint32_t TexturesUsed;
    GLuint Program;
    GLenum CullFunction;
    GLenum DepthFunction;
} Material;

Material* Material_create(char* vertexProgramPath, char* fragmentProgramPath, const uint32_t numberOfTextures, const GLenum cullFuncton, const GLenum depthFunction);
void Material_destroy(Material** material);

void SetTextureFromPointer(const Material* material, Texture* texture, uint32_t index);
void SetTextureFromAlias(const Material* material, const char* alias, uint32_t index);
void BindMaterial(const Material* material);


