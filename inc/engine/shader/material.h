#pragma once

#include "engine_core/engine_types.h"
#include "engine_core/string.h"

// Forward Declarations
typedef struct HashTable HashTable;
typedef struct Texture Texture;
typedef struct Shader Shader;

typedef struct Material {
    u64     TextureCount;
    String* TextureAliases;
    String  ShaderAlias;
    u32     CullFunction;
    u32     DepthFunction;
} Material;

typedef struct MaterialDescriptor {
    u32     cullFunction;
    u32     depthFunction;
    u64     textureCount;
    char*   alias;
    char**  textures;
} MaterialDescriptor;

Material* Material_create (const MaterialDescriptor descriptor);
void Material_destroy (Material** material);

Shader* Material_bind (const Material* material);


