#pragma once

#include "stdint.h"

// Forward Declarations
typedef struct HashTable HashTable;
typedef struct Texture Texture;
typedef struct Shader Shader;

typedef struct Material {
    //HashTable* Uniforms;
    Texture**   Textures;
    Shader*     ShaderProgram;
    u32         TextureCount;
    GLenum      CullFunction;
    GLenum      DepthFunction;
} Material;

Material* Material_create(Shader* shader, const u32 numberOfTextures, const GLenum cullFuncton, const GLenum depthFunction);
void Material_destroy(Material** material);

void SetTextureFromPointer(const Material* material, Texture* texture, u32 index);
void SetTextureFromAlias(const Material* material, const char* alias, u32 index);
void BindMaterial(const Material* material);


