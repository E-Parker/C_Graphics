#include "stdio.h"
#include "stdlib.h"

#include "engine_core/engine_types.h"
#include "engine_core/string.h"
#include "engine_core/list.h"
#include "engine/object/mesh.h"

#include "engine/shader/renderable.h"


StaticMesh* Object_StaticMesh_create_from_raw_data(const char* path, void* parent) {
    StaticMesh* staticMesh = NULL;

    u64 bufferSizes[4] = { 0, 0, 0, 0 };

    FILE* file = fopen(path, "rb");
    fread(bufferSizes, 8, 4, file);

    if (ferror(file)) {
        goto ReturnMesh;
    }

    fseek(file, 0x20, SEEK_SET);
    u64 startOfData = ftell(file);
    fseek(file, 0, SEEK_END);
    u64 endOfData = ftell(file);
    fseek(file, 0x20, SEEK_SET);

    u64 bytesOfData = endOfData - startOfData;
    u64 expectedBytes = bufferSizes[0] + bufferSizes[1] + bufferSizes[2] + bufferSizes[3];

    u64 indexSize = bufferSizes[0] / sizeof(u32);
    u64 vertexSize = bufferSizes[1] / sizeof(vec3);
    u64 normalSize = bufferSizes[2] / sizeof(vec3);
    u64 tCoordSize = bufferSizes[3] / sizeof(vec2);

    if (vertexSize != normalSize || normalSize != tCoordSize) {
        goto ReturnMesh;
    }

    u32* indexBuffer = (u32*)malloc(bufferSizes[0]);
    GLfloat* vertexBuffer = (GLfloat*)malloc(bufferSizes[1]);
    GLfloat* normalBuffer = (GLfloat*)malloc(bufferSizes[2]);
    GLfloat* tCoordBuffer = (GLfloat*)malloc(bufferSizes[3]);

    u64 errorCode;

    fread(indexBuffer, sizeof(u32), indexSize, file);
    fread(vertexBuffer, sizeof(vec3), vertexSize, file);
    fread(normalBuffer, sizeof(vec3), normalSize, file);
    fread(tCoordBuffer, sizeof(vec2), tCoordSize, file);

    if (ferror(file)) {
        goto DestroyBuffersAndReturnMesh;
    }

    staticMesh = Object_StaticMesh_create_empty(parent);
    MeshRender mesh = { .materialIndex = 0 };
    UploadMesh(&mesh, indexBuffer, vertexBuffer, normalBuffer, tCoordBuffer, indexSize, vertexSize);
    List_push_back(&staticMesh->meshRenders, mesh);

DestroyBuffersAndReturnMesh:
    free(indexBuffer);
    free(vertexBuffer);
    free(normalBuffer);
    free(tCoordBuffer);

ReturnMesh:
    fclose(file);
    return staticMesh;
}
