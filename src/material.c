#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"

#include "stdio.h"
#include "string.h"

#include "engine_core/hash_table.h"
#include "engine/parse_shader.h"
#include "texture.h"
#include "material.h"


// Macro defs:
#ifndef MATERIAL_BUFFER_SIZE
#define MATERIAL_BUFFER_SIZE 0xff
#endif


Material* Material_create(char* vertexProgramPath, char* fragmentProgramPath, const uint32_t numberOfTextures, const GLenum cullFuncton, const GLenum depthFunction) {

    Material* newMaterial = (Material*)malloc(sizeof(Material));
    if (!newMaterial) return NULL;

    newMaterial->TexturesUsed = numberOfTextures;
    newMaterial->CullFunction = cullFuncton;
    newMaterial->DepthFunction = depthFunction;
    
    newMaterial->Textures = NULL;
    
    if(numberOfTextures != 0) {
        newMaterial->Textures = (Texture**)calloc(numberOfTextures, sizeof(Texture*));
    }

    // compile the shader programs
    Shader_CompileProgram(newMaterial->Program, {0, GL_VERTEX_SHADER, vertexProgramPath}, {0, GL_FRAGMENT_SHADER, fragmentProgramPath} );
    
    return newMaterial;

}


void Material_destroy(Material** material) {
    if (!material || !(*material)) {
        printf("Error deleting Material: Material is null!");
        return;
    }

    if ((*material)->TexturesUsed != 0) {

        for (int i = 0; i < (*material)->TexturesUsed; i++) {
            if ((*material)->Textures[i]) {
                DeleteTexture((*material)->Textures[i]->alias);
            }
        }
        free((*material)->Textures);
    }

    glDeleteProgram((*material)->Program);
    (*material)->Program = GL_NONE;
    free(*material);
    (*material) = NULL;
}


void SetTextureFromPointer(const Material* material, Texture* texture, uint32_t index){
    /* Manually set a texture from a texture pointer. AVOID USING!!!
    The textures set this way will be UNAMANGED and must be freed MANUALLY. */

    if (!material) {
        printf("Error setting Material Texture: Material is null.\n");
        return;
    }
    
    if (index >= material->TexturesUsed) {
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

void SetTextureFromAlias(const Material* material, const char* alias, uint32_t index) {
    /* Set a material's texture at the given index, by the texture's alias. */

    if (!material) {
        printf("Error setting Material Texture: Material is null.\n");
        return;
    }

    if (index >= material->TexturesUsed) {
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
    glUseProgram(material->Program);
    glCullFace(material->CullFunction);
    glDepthFunc(material->DepthFunction);

    // Set the active texture for each texture in the material.
    for (uint32_t i = 0; i < material->TexturesUsed; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        if (material->Textures[i]) {
            glBindTexture(material->Textures[i]->type, material->Textures[i]->ID);
        }
    }
}

