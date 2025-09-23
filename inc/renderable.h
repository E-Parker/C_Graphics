#pragma once

#include "glad/glad.h"
#include "stdint.h"

#include "engine/math.h"

typedef struct Material Material;

typedef struct MeshRender {
    uint64_t indices;
    uint32_t materialIndex;
    // Define GPU buffer objects:
    GLuint VertexAttributeObject;       // Vertices with attributes that might be in different locations in the VBO. bind this to point to this mesh.
    GLuint VertexBufferObject;          // raw vertex buffer.
    GLuint NormalBufferObject;          // raw Normal buffer.
    GLuint TextureCoordBufferObject;    // raw UV buffer.
    GLuint ElementBufferObject;         // index of each vertex constructing faces. allows for all this to be done in one draw pass.

} MeshRender;

void DrawRenderable(const MeshRender* mesh, const Material* material, const mat4 transform);
void FreeMesh(MeshRender* mesh);
void FreeSubMesh(MeshRender* mesh);
void UploadMesh(MeshRender* mesh, const uint32_t* indicesArray, const GLfloat* vertexBufferArray, const  GLfloat* normalBufferArray, const GLfloat* tCoordArray, const  uint64_t indices, const uint64_t vertecies);
void UploadSubMesh(MeshRender* mesh, MeshRender* source, const uint32_t* indicesArray, const uint32_t indices);

