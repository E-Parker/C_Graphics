#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"

#include "engine_core/string.h"
#include "engine_core/list.h"

#include "file_reader.h"
#include "renderable.h"
#include "mesh.h"


StaticMesh* Object_StaticMesh_create_from_raw_data(const char* path, void* parent) {
    StaticMesh* staticMesh = NULL;

    uint64_t bufferSizes[4] = { 0, 0, 0, 0 };

    FILE* file = fopen(path, "rb");
    fread(bufferSizes, 8, 4, file);

    if (ferror(file)) {
        goto ReturnMesh;
    }

    fseek(file, 0x20, SEEK_SET);
    uint64_t startOfData = ftell(file);
    fseek(file, 0, SEEK_END);
    uint64_t endOfData = ftell(file);
    fseek(file, 0x20, SEEK_SET);

    uint64_t bytesOfData = endOfData - startOfData;
    uint64_t expectedBytes = bufferSizes[0] + bufferSizes[1] + bufferSizes[2] + bufferSizes[3];

    uint64_t indexSize = bufferSizes[0] / sizeof(uint32_t);
    uint64_t vertexSize = bufferSizes[1] / sizeof(vec3);
    uint64_t normalSize = bufferSizes[2] / sizeof(vec3);
    uint64_t tCoordSize = bufferSizes[3] / sizeof(vec2);

    if (vertexSize != normalSize || normalSize != tCoordSize) {
        goto ReturnMesh;
    }

    uint32_t* indexBuffer = (uint32_t*)malloc(bufferSizes[0]);
    GLfloat* vertexBuffer = (GLfloat*)malloc(bufferSizes[1]);
    GLfloat* normalBuffer = (GLfloat*)malloc(bufferSizes[2]);
    GLfloat* tCoordBuffer = (GLfloat*)malloc(bufferSizes[3]);

    uint64_t errorCode;

    fread(indexBuffer, sizeof(uint32_t), indexSize, file);
    fread(vertexBuffer, sizeof(vec3), vertexSize, file);
    fread(normalBuffer, sizeof(vec3), normalSize, file);
    fread(tCoordBuffer, sizeof(vec2), tCoordSize, file);

    if (ferror(file)) {
        goto DestroyBuffersAndReturnMesh;
    }

    staticMesh = Object_StaticMesh_create_empty(parent);
    MeshRender mesh = { .materialIndex = 0 };
    List_push_back(&staticMesh->meshRenders, mesh);

    UploadMesh((MeshRender*)List_at(&staticMesh->meshRenders, 0), indexBuffer, vertexBuffer, normalBuffer, tCoordBuffer, indexSize, vertexSize);

DestroyBuffersAndReturnMesh:
    free(indexBuffer);
    free(vertexBuffer);
    free(normalBuffer);
    free(tCoordBuffer);

ReturnMesh:
    fclose(file);
    return staticMesh;
}
