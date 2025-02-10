#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <cassert>
#include <cstring>
#include <iostream>
#include <vector>

#include "gl_parse_shader.h"
#include "hash_table.h"
#include "material.h"
#include "texture.h"


Material::Material(const char* vertexProgramPath, const char* fragmentProgramPath, const uint32_t numberOfTextures, const GLenum cullFuncton, const GLenum depthFunction) {
    TexturesUsed = numberOfTextures;
    CullFunction = cullFuncton;
    DepthFunction = depthFunction;
    
    Textures = nullptr;
    
    if(TexturesUsed != 0) {
        Textures = new Texture*[TexturesUsed]{nullptr};
    }

    GLuint VertexProgram = GL_NONE;
    GLuint FragmentProgram = GL_NONE;

    // compile the shader programs
    CompileShader(&VertexProgram, GL_VERTEX_SHADER, vertexProgramPath);
    CompileShader(&FragmentProgram, GL_FRAGMENT_SHADER, fragmentProgramPath);
    Program = CompileShaderProgram(VertexProgram, FragmentProgram);

}


Material::~Material() {
    
    if (this == nullptr) {
        std::cout << "Error deleting Material: Material is null!!! " << std::endl;
        return;
    }

    if (TexturesUsed != 0) {

        for (int i = 0; i < TexturesUsed; i++) {
            if (Textures[i] != nullptr) {
                DeleteTexture(Textures[i]->alias);
            }
        }
        delete[] Textures;
    }

    glDeleteProgram(Program);
    Program = GL_NONE;
}


void SetTextureFromPointer(const Material* material, Texture* texture, uint32_t index){
    /* Manually set a texture from a texture pointer. AVOID USING!!!
    The textures set this way will be UNAMANGED and must be freed MANUALLY. */

    if (material == nullptr) {
        std::cout << "Error setting Material Texture: Material is null!!! " << std::endl;
        return;
    }
    
    if (index >= material->TexturesUsed) {
        std::cout << "Warning: Material Texture index out of range. Discarding texture. " << std::endl;
        return;
    }

    if (texture == nullptr) {
        std::cout << "Error setting Material Texture: Texture must not be null." << std::endl;
        return;
    }

    // If a texture already exists in that slot, try to delete it.
    if (material->Textures[index] != nullptr) {
        DeleteTexture(material->Textures[index]->alias);
    }

    material->Textures[index] = texture;
    texture->references++;
}

void SetTextureFromAlias(const Material* material, const char* alias, uint32_t index) {
    /* Set a material's texture at the given index, by the texture's alias. */

    if (material == nullptr) {
        std::cout << "Error setting Material Texture: Material is null!!! " << std::endl;
        return;
    }

    if (index >= material->TexturesUsed) {
        std::cout << "Warning: Material Texture index out of range. Discarding texture. " << std::endl;
        return;
    }

    Texture* texture = nullptr;
    FindTexture(alias, &texture);

    if (texture == nullptr) {
        std::cout << "Error setting Material Texture: \"" << alias << "\" At index: " << index << ". The texture could not be found." << std::endl;
        return;
    }

    // If a texture already exists in that slot, try to delete it.
    if (material->Textures[index] != nullptr) {
        DeleteTexture(material->Textures[index]->alias);
    }

    material->Textures[index] = texture;
    texture->references++;
}


void BindMaterial(const Material* material){
    /* Set up the material for rendering. */

    if (material == nullptr) {
        return;
    }

    // Set the shader program and get the uniform from the shader.
    glUseProgram(material->Program);
    glCullFace(material->CullFunction);
    glDepthFunc(material->DepthFunction);

    // Set the active texture for each texture in the material.
    for (uint32_t i = 0; i < material->TexturesUsed; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        if (material->Textures[i] != nullptr) {
            glBindTexture(material->Textures[i]->type, material->Textures[i]->ID);
        }
    }
}

