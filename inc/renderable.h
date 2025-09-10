#pragma once

#include <glad/glad.h>
#include <stdint.h>

#include "gl_math.h"

typedef struct Material Material;

typedef struct Mesh {
    /* This is the core structure of a mesh, it does not have any ability to manage itself at all. */

    uint64_t indexBytes;

    // Define GPU buffer objects:
    GLuint VertexAttributeObject;       // Vertices with attributes that might be in different locations in the VBO. bind this to point to this mesh.
    GLuint VertexBufferObject;          // raw vertex buffer.
    GLuint NormalBufferObject;          // raw Normal buffer.
    GLuint TextureCoordBufferObject;    // raw UV buffer.
    GLuint ElementBufferObject;         // index of each vertex constructing faces. allows for all this to be done in one draw pass.

} Mesh;

void Mesh_Draw(const Mesh* mesh, const Material* material, const mat4 transform);
void Mesh_Free(Mesh* mesh);
void Mesh_FreeSubmesh(Mesh* mesh);
void Mesh_Upload(Mesh* mesh, const uint32_t* indeciesArray, const GLfloat* vertexBufferArray, const  GLfloat* normalBufferArray, const GLfloat* tCoordArray, const  uint64_t indecies, const uint64_t vertecies);
void Mesh_UploadSubmesh(Mesh* mesh, Mesh* source, const uint32_t* indeciesArray, const uint32_t indecies);
