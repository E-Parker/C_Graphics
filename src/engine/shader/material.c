#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "engine_core/hash_table.h"
#include "engine_core/engine_shader.h"


#define MATERIAL_BUFFER_SIZE 0x100


Material* Material_create (const MaterialDescriptor descriptor) {
    Shader* shader = Shader_get(descriptor.alias);

    if (!shader) {
        return NULL;
    }

    Material* newMaterial = (Material*)malloc(sizeof(Material));
    Engine_validate(newMaterial, ENOMEM);

     shader->references++;

    String aliasString = String_from_ptr(descriptor.alias);
    String_create_dirty(&aliasString, &newMaterial->ShaderAlias);

    newMaterial->TextureCount = descriptor.textureCount;
    newMaterial->CullFunction = descriptor.cullFunction;
    newMaterial->DepthFunction = descriptor.depthFunction;
    
    if(newMaterial->TextureCount != 0) {
        newMaterial->TextureAliases = (String*)calloc(descriptor.textureCount, sizeof(String));
        for (u64 i = 0; i < newMaterial->TextureCount; ++i) {
            String currentAlias = String_from_ptr(descriptor.textures[i]);
            String_create_dirty(&currentAlias, &newMaterial->TextureAliases[i]);
        }
    }
    else {
        newMaterial->TextureAliases = NULL;
    }

    return newMaterial;
}


void Material_destroy (Material** material) {
    if (!material || !(*material)) {
        printf("Error deleting Material: Material is null!");
        return;
    }

    for (u64 i = 0; i < (*material)->TextureCount; ++i) {
        Texture* texture;
        Texture_get_String((*material)->TextureAliases[i], &texture);
    }

    if ((*material)->TextureCount != 0) {

        for (int i = 0; i < (*material)->TextureCount; i++) {
            Texture_delete_String((*material)->TextureAliases[i]);
            String_free_dirty(&(*material)->TextureAliases[i]);
        }
        free((*material)->TextureAliases);
    }
    
    Shader_delete_String((*material)->ShaderAlias);
    String_free_dirty(&(*material)->ShaderAlias);

    free(*material);
    (*material) = NULL;
}


Shader* Material_bind (const Material* material) {
    /* Set up the material for rendering. */

    if (!material) {
        return NULL;
    }

    Shader* shader = Shader_get_String(material->ShaderAlias);

    if (!shader) {
        return NULL;
    }

    // Set the shader program and get the uniform from the shader.
    glUseProgram(shader->program);
    glCullFace(material->CullFunction);
    glDepthFunc(material->DepthFunction);

    // Set the active texture for each texture in the material.
    for (u32 i = 0; i < material->TextureCount; i++) {
        //TODO: Rework shader handling because this is bad. We don't know ahead of time what the binding index is, there might be data that isn't textures at the start.
        glActiveTexture(i + GL_TEXTURE0);
        Texture* texture;
        Texture_get_String(material->TextureAliases[i], &texture);
        if (texture) {
            glBindTexture(texture->Type, texture->ID);
        }
        else {
            printf("Missing Texture at index %u\n",i);
        }
    }
    return shader;
}

