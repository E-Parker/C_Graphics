#pragma once

#include <glad/glad.h>
#include "gl_math.h"

struct Material;
struct Matrix;

typedef struct Mesh {
    /* This is the core structure of a mesh, it does not have any ability to manage itself at all. */

    size_t indexBytes;

    // Define GPU buffer objects:
    GLuint VertexAttributeObject;       // Vertices with attributes that might be in different locations in the VBO. bind this to point to this mesh.
    GLuint VertexBufferObject;          // raw vertex buffer.
    GLuint NormalBufferObject;          // raw Normal buffer.
    GLuint TextureCoordBufferObject;    // raw UV buffer.
    GLuint ElementBufferObject;         // index of each vertex constructing faces. allows for all this to be done in one draw pass.

} Mesh;

void DrawRenderable(const Mesh* mesh, const Material* material, const Matrix* transform);
void FreeMesh(Mesh* mesh);
void FreeSubMesh(Mesh* mesh);
void UploadMesh(Mesh* mesh, const uint32_t* indeciesArray, const GLfloat* vertexBufferArray, const  GLfloat* normalBufferArray, const GLfloat* tCoordArray, const  size_t indecies, const size_t vertecies);
void UploadSubMesh(Mesh* mesh, Mesh* source, const uint32_t* indeciesArray, const uint32_t indecies);
