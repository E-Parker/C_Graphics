#pragma once

#include "stdint.h"

// Forward Declarations
typedef struct HashTable HashTable;
typedef struct Texture Texture;
typedef struct Shader Shader;

typedef struct Material {
    String*     TextureAliases;
    String      ShaderAlias;
    u64         TextureCount;
    GLenum      CullFunction;
    GLenum      DepthFunction;
} Material;

typedef struct MaterialDescriptor {
    GLenum      cullFunction;
    GLenum      depthFunction;
    u64         textureCount;
    char*       alias;
    char**      textures;
} MaterialDescriptor;

Material* Material_create (const MaterialDescriptor descriptor);
void Material_destroy (Material** material);

Shader* Material_bind (const Material* material);


