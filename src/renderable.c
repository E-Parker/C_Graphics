#include "glad/glad.h"

#include "stddef.h"

#include "engine_core/hash_table.h"
#include "engine/math.h"
#include "material.h"
#include "texture.h"
#include "renderable.h"

void FreeMesh(MeshRender* mesh) {

    if (mesh->ElementBufferObject != GL_NONE) {
        glDeleteBuffers(1, &(mesh->ElementBufferObject));
        mesh->ElementBufferObject = GL_NONE;
    }

    if (mesh->TextureCoordBufferObject != GL_NONE) {
        glDeleteBuffers(1, &(mesh->TextureCoordBufferObject));
        mesh->TextureCoordBufferObject = GL_NONE;
    }

    if (mesh->NormalBufferObject != GL_NONE) {
        glDeleteBuffers(1, &(mesh->NormalBufferObject));
        mesh->NormalBufferObject = GL_NONE;
    }

    if (mesh->VertexBufferObject != GL_NONE) {
        glDeleteBuffers(1, &(mesh->VertexBufferObject));
        mesh->VertexBufferObject = GL_NONE;
    }

    if (mesh->VertexAttributeObject != GL_NONE) {
        glDeleteBuffers(1, &(mesh->VertexAttributeObject));
        mesh->VertexAttributeObject = GL_NONE;
    }
}


void FreeSubMesh(MeshRender* mesh) {
    /* Use this to free a mesh that was created by copying from another. */

    if (mesh->ElementBufferObject != GL_NONE) {
        glDeleteBuffers(1, &(mesh->ElementBufferObject));
        mesh->ElementBufferObject = GL_NONE;
    }

    if (mesh->VertexAttributeObject != GL_NONE) {
        glDeleteBuffers(1, &(mesh->VertexAttributeObject));
        mesh->VertexAttributeObject = GL_NONE;
    }

}


void UploadMesh(MeshRender* mesh, const uint32_t* indicesArray, const GLfloat* vertexBufferArray, const GLfloat* normalBufferArray, const GLfloat* tCoordArray, const uint64_t indices, const uint64_t vertecies) {
    /* Uploading mesh to GPU. points and normalBuffer must exist for the upload to work.
    tCoord data and face data is optional. */

    uint64_t vertexBytes = vertecies * sizeof(vec3);
    uint64_t tCoordBytes = vertecies * sizeof(vec2);
    uint64_t indexBytes = indices * sizeof(uint32_t);
    uint64_t normalBytes = vertexBytes;

    mesh->indices = indices;

    // Create a Vertex Attribute Object. This is kind of like a container for the buffer objects.              
    if (mesh->VertexAttributeObject == GL_NONE) { glGenVertexArrays(1, &(mesh->VertexAttributeObject)); }
    glBindVertexArray(mesh->VertexAttributeObject);

    // This buffer is bound to the 0th attribute, it stores the points of the mesh.
    if (mesh->VertexBufferObject == GL_NONE) { glGenBuffers(1, &(mesh->VertexBufferObject)); }
    glBindBuffer(GL_ARRAY_BUFFER, (mesh->VertexBufferObject));
    glBufferData(GL_ARRAY_BUFFER, vertexBytes, vertexBufferArray, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), NULL);
    glEnableVertexAttribArray(0);

    // This buffer is bound to the 1st Attribute, it stores the normal vectors for each point.
    if (mesh->NormalBufferObject == GL_NONE) { glGenBuffers(1, &(mesh->NormalBufferObject)); }
    glBindBuffer(GL_ARRAY_BUFFER, mesh->NormalBufferObject);
    glBufferData(GL_ARRAY_BUFFER, normalBytes, normalBufferArray, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), NULL);
    glEnableVertexAttribArray(1);

    // First we check if the mesh has texture coordinates, then we add a buffer and assign it.
    if (tCoordArray) {
        if (mesh->TextureCoordBufferObject == GL_NONE) { glGenBuffers(1, &(mesh->TextureCoordBufferObject)); }
        glBindBuffer(GL_ARRAY_BUFFER, mesh->TextureCoordBufferObject);
        glBufferData(GL_ARRAY_BUFFER, tCoordBytes, tCoordArray, GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), NULL);
        glEnableVertexAttribArray(2);
    }

    // First check if there are face indicies, then make an element array for them.
    if (indicesArray) {
        if (mesh->ElementBufferObject == GL_NONE) { glGenBuffers(1, &(mesh->ElementBufferObject)); }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ElementBufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBytes, indicesArray, GL_STATIC_DRAW);
    }

    glBindVertexArray(GL_NONE);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);

}

void UploadSubMesh(MeshRender* mesh, MeshRender* source, const uint32_t* indicesArray, const uint32_t indices) {
    /* variant of UploadMesh for meshes that share vertices but have a different element buffer. */

    uint64_t indexBytes = indices * sizeof(uint32_t);
    mesh->indices = indices;

    if (mesh->VertexAttributeObject == GL_NONE) {
        glGenVertexArrays(1, &(mesh->VertexAttributeObject));
    }
    glBindVertexArray(mesh->VertexAttributeObject);

    mesh->VertexBufferObject = source->VertexBufferObject;
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VertexBufferObject);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), NULL);
    glEnableVertexAttribArray(0);

    mesh->NormalBufferObject = source->NormalBufferObject;
    glBindBuffer(GL_ARRAY_BUFFER, mesh->NormalBufferObject);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), NULL);
    glEnableVertexAttribArray(1);

    mesh->TextureCoordBufferObject = source->TextureCoordBufferObject;
    glBindBuffer(GL_ARRAY_BUFFER, mesh->TextureCoordBufferObject);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), NULL);
    glEnableVertexAttribArray(2);

    if (mesh->ElementBufferObject == GL_NONE) { glGenBuffers(1, &(mesh->ElementBufferObject)); }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ElementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBytes, indicesArray, GL_STATIC_DRAW);

    glBindVertexArray(GL_NONE);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);

}


void DrawRenderable(const MeshRender* mesh, const Material* material, const mat4 transform) {
    // Bind the material's shader program and textures.

    BindMaterial(material);

    // Get the uniform from the shader.
    GLint u_mvp = glGetUniformLocation(material->Program, "u_mvp");

    // Bind the VAO and draw the elements.
    glBindVertexArray(mesh->VertexAttributeObject);
    glUniformMatrix4fv(u_mvp, 1, GL_FALSE, transform);
    glDrawElements(GL_TRIANGLES, mesh->indices, GL_UNSIGNED_INT, 0);

    // unbind the VAO.
    glBindVertexArray(GL_NONE);

}
