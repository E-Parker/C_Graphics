#pragma once

#include <glad/glad.h>

struct Material;
struct Matrix;

typedef struct Mesh {
    /* This is the core structure of a mesh, it does not have any ability to manage itself at all. */

    size_t indexBytes = 0;

    // Define GPU buffer objects:
    GLuint VertexAttributeObject = GL_NONE;       // Vertices with attributes that might be in different locations in the VBO. bind this to point to this mesh.
    GLuint VertexBufferObject = GL_NONE;          // raw vertex buffer.
    GLuint NormalBufferObject = GL_NONE;          // raw Normal buffer.
    GLuint TextureCoordBufferObject = GL_NONE;    // raw UV buffer.
    GLuint ElementBufferObject = GL_NONE;         // index of each vertex constructing faces. allows for all this to be done in one draw pass.

} Mesh;

void DrawRenderable(const Mesh* mesh, const Material* material, const Matrix* transform);
void FreeMesh(Mesh* mesh);
void FreeSubMesh(Mesh* mesh);
void UploadMesh(Mesh* mesh, const  uint32_t* indeciesArray, const  Vector3* vertexBufferArray, const  Vector3* normalBufferArray, const Vector2* tCoordArray, const  size_t indecies, const size_t vertecies);
void UploadSubMesh(Mesh* mesh, Mesh* source, const uint32_t* indeciesArray, const uint32_t indecies);
