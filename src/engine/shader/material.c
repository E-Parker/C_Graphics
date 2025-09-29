#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "stdlib.h"
#include "string.h"
#include "stdio.h"

#include "engine_core/hash_table.h"
#include "engine_core/engine_shader.h"


#define MATERIAL_BUFFER_SIZE 0x100


Material* Material_create(const Shader* shader, const u32 textureCount, const GLenum cullFuncton, const GLenum depthFunction) {
    
    if (!shader) {
        printf("Material Error: Shader was null!\n");
        return NULL;
    }

    Material* newMaterial = (Material*)malloc(sizeof(Material));
    
    if (!newMaterial) {
        return NULL;
    }

    newMaterial->ShaderProgram = shader;
    newMaterial->TextureCount = textureCount;
    newMaterial->CullFunction = cullFuncton;
    newMaterial->DepthFunction = depthFunction;
    
    if(textureCount != 0) {
        newMaterial->Textures = (Texture**)calloc(textureCount, sizeof(Texture*));
    }

    else {
        newMaterial->Textures = NULL;
    }


    return newMaterial;

}


void Material_destroy(Material** material) {
    if (!material || !(*material)) {
        printf("Error deleting Material: Material is null!");
        return;
    }

    if ((*material)->TextureCount != 0) {

        for (int i = 0; i < (*material)->TextureCount; i++) {
            if ((*material)->Textures[i]) {
                DeleteTexture((*material)->Textures[i]->alias);
            }
        }
        free((*material)->Textures);
    }

    Shader_destroy(&(*material)->ShaderProgram);

    free(*material);
    (*material) = NULL;
}


void SetTextureFromPointer(const Material* material, Texture* texture, u32 index){
    /* Manually set a texture from a texture pointer. AVOID USING!!!
    The textures set this way will be UNAMANGED and must be freed MANUALLY. */

    if (!material) {
        printf("Error setting Material Texture: Material is null.\n");
        return;
    }
    
    if (index >= material->TextureCount) {
        printf("Warning: Material Texture index out of range. Discarding texture.\n");
        return;
    }

    if (!texture) {
        printf("Error setting Material Texture: Texture must not be null.\n");
        return;
    }

    // If a texture already exists in that slot, try to delete it.
    if (material->Textures[index]) {
        DeleteTexture(material->Textures[index]->alias);
    }

    material->Textures[index] = texture;
    texture->references++;
}

void SetTextureFromAlias(const Material* material, const char* alias, u32 index) {
    /* Set a material's texture at the given index, by the texture's alias. */

    if (!material) {
        printf("Error setting Material Texture: Material is null.\n");
        return;
    }

    if (index >= material->TextureCount) {
        printf("Warning: Material Texture index out of range. Discarding texture.\n");
        return;
    }

    Texture* texture = NULL;
    FindTexture(alias, &texture);

    if (!texture) {
        printf("Error setting Material Texture: \"%s\" At index: %d. The texture could not be found.\n", alias, index);
        return;
    }

    // If a texture already exists in that slot, try to delete it.
    if (material->Textures[index]) {
        DeleteTexture(material->Textures[index]->alias);
    }

    material->Textures[index] = texture;
    texture->references++;
}


void BindMaterial(const Material* material){
    /* Set up the material for rendering. */

    if (!material) {
        return;
    }

    // Set the shader program and get the uniform from the shader.
    glUseProgram(material->ShaderProgram->program);
    glCullFace(material->CullFunction);
    glDepthFunc(material->DepthFunction);

    // Set the active texture for each texture in the material.
    for (u32 i = 0; i < material->TextureCount; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        if (material->Textures[i]) {
            glBindTexture(material->Textures[i]->type, material->Textures[i]->ID);
        }
    }
}

