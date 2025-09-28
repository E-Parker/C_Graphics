#pragma once

#include "stdint.h"

// Forward Declarations
typedef struct HashTable HashTable;
typedef struct Texture Texture;
typedef struct Shader Shader;

typedef struct Material {
    //HashTable* Uniforms;
    Texture** Textures;
    Shader* ShaderProgram;
    uint32_t TextureCount;
    GLenum CullFunction;
    GLenum DepthFunction;
} Material;

Material* Material_create(const Shader* shader, const uint32_t numberOfTextures, const GLenum cullFuncton, const GLenum depthFunction);
void Material_destroy(Material** material);

void SetTextureFromPointer(const Material* material, Texture* texture, uint32_t index);
void SetTextureFromAlias(const Material* material, const char* alias, uint32_t index);
void BindMaterial(const Material* material);


