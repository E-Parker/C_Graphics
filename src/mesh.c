#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"

#include "engine_core/string.h"
#include "engine_core/list.h"


#include "file_reader.h"
#include "renderable.h"
#include "mesh.h"

char segmentBuffer[0x100];

#define BUFFER_MAX_SIZE 0x1000

#define PackByte_uint16(a, b) ( ((uint16_t)b << 8) | (uint16_t)a )
#define PackByte_uint32(a, b, c, d) ( ((uint32_t)d << 24) | ((uint32_t)c << 16) | ((uint32_t)b << 8) | (uint32_t)a )
#define PackByte_uint64(a, b, c, d, e, f, g, h) ( ((uint64_t)h << 56) | ((uint64_t)g << 48) | ((uint64_t)f << 40) | ((uint64_t)e << 32) | ((uint64_t)d << 24) | ((uint64_t)c << 16) | ((uint64_t)b << 8) | (uint64_t)a )

// Supported File Formats:
#define ObjFile 0x6F002020206A626F      // .obj     - WaveFront
#define GlbFile 0x6700202020626C67      // .glb     - Graphics Library Transmission Format, as binary file
#define GltfFile 0x6700202066746C67     // .gltf    - Graphics Library Transmission Format
#define BinFile 0x000000006e69622e      // .bin     - Raw binary file
#define BlendFile 0x620020646E656C62    // .blend   - Blender project file
#define U3dFile 0x7500202020643375      // .u3d     - Universal 3D
#define UsdFile 0x7500202020647375      // .usd     - Universal Scene Descriptor
#define FbxFile 0x6600202020786266      // .fbx     - "Filmbox" format, Maya version.



void Object_StaticMesh_destroy(StaticMesh* mesh) {
    OBJECT_DESTROY_BODY(mesh);

    for (List_iterator(MeshRender, &mesh->meshRenders)) {
        FreeMesh(it);
    }

    List_deinitialize(&mesh->meshRenders);
    List_deinitialize(&mesh->materials);    // Materials are managed externally.
}


StaticMesh* Object_StaticMesh_create_empty(void* parent) {
    OBJECT_CREATE_BODY(StaticMesh, parent, Object_TypeStaticMesh);

    List_initialize(Material*, &object->materials, 1);
    List_initialize(MeshRender, &object->meshRenders, 1);

    Object_set_alias(object, "StaticMesh");
    object->Draw = Object_StaticMesh_Draw;
    object->Destroy = Object_StaticMesh_destroy;

    return object;
}


/*
void vec2FromString(String data) {

    std::stringstream dataStream(data);
    std::string segment;
    std::vector<std::string> segmentList;

    while (std::getline(dataStream, segment, ' ')) {
        if (!segment.empty()) {
            segmentList.push_back(segment);
        }
    }

    Vector2 vector{ 0.0f, 0.0f };

    vector.x = std::stof(segmentList[0]);
    vector.y = std::stof(segmentList[1]);

    return vector;

}

Vector3 Vector3FromString(const std::string data) {

    std::stringstream dataStream(data);
    std::string segment;
    std::vector<std::string> segmentList;

    while (std::getline(dataStream, segment, ' ')) {
        if (!segment.empty()) {
            segmentList.push_back(segment);
        }
    }

    Vector3 vector{ 0.0f, 0.0f, 0.0f };

    vector.x = std::stof(segmentList[0]);
    vector.y = std::stof(segmentList[1]);
    vector.z = std::stof(segmentList[2]);

    return vector;
}
*/

int parseSplits(char* line, uint16_t MaxSegments, String* segments) {
    
    uint16_t splits = 0;
    char* currentChar = line;

    for (uint16_t i = 0; i < 0xffff; ++i, ++currentChar) {
        // Check the current character.
        switch ((int)(*currentChar)) {
        case '\0': return 1;// End of file reached early.
        case '\n':          // End of line reached. 
            segments[splits].end = currentChar - 1;
            i = 0xfffe;     // Set "i" to max short - 1 so for loop exits.
            break;
        case ' ':           // Separator is found. Segment start and end must be set before incrementing splits.
            if (segments[splits].start) segments[splits].end = currentChar - 1; // SegmentStart cannot be set on iteration 0 so this is fine.
            else                        segments[splits].start = currentChar + 1;
            break;
        default: continue; break;
        }

        // Increment splits counter if this segment's start and end are set.
        if (segments[splits].start && segments[splits].end) {
            ++splits;
        }

        // break from loop if out of segments.
        if (splits >= MaxSegments) {
            break;
        }
    }
    return 0;
}

int parseFaceIndicies(List* vi, List* ti, List* ni, char* line) {
    // This function parses a wavefront file face data, adding it to the list of vertex indices, texture indicies, and normal indicies. 
    // Returns the number of vertices added. 

    if (line == NULL) return 0;
    
    String segments[0x6];
    
    parseSplits(line, 0x6, (String*)&segments);

    for (uint8_t i = 0; i < 0x6; ++i) {

    }
}


void Object_StaticMesh_set_Material(StaticMesh* staticMesh, const uint32_t subMesh, const Material* material) {
    if (!staticMesh) {
        return;
    }
    
    MeshRender* mesh = (MeshRender*)List_at(&staticMesh->meshRenders, subMesh);
    
    if (!mesh) {
        return;
    }

    uint32_t materialCount = List_count(&staticMesh->materials);
    
    if (subMesh > materialCount) {
        return;
    }

    long int indexOf;
    if (List_contains_item(&staticMesh->materials, material, &indexOf)) {
        mesh->materialIndex = indexOf;
    }
    else {
        mesh->materialIndex = materialCount;
    }

    List_push_back(&staticMesh->materials, material);
}


StaticMesh* Object_StaticMesh_create_from_raw_data(const char* path, void* parent) {
    StaticMesh* staticMesh = NULL;
    
    uint64_t bufferSizes[4] = { 0, 0, 0, 0 };
    
    FILE* file = fopen(path, "r");
    fread(bufferSizes, 8, 4, file);
    
    if (ferror(file)) {
        goto ReturnMesh;
    }

    fseek(file, sizeof(bufferSizes), SEEK_SET);
    uint64_t startOfData = ftell(file);
    fseek(file, 0, SEEK_END);
    uint64_t endOfData = ftell(file);
    fseek(file, sizeof(bufferSizes), SEEK_SET);

    uint64_t bytesOfData = endOfData - startOfData;
    uint64_t expectedBytes = bufferSizes[0] + bufferSizes[1] + bufferSizes[2] + bufferSizes[3];

    uint64_t indexSize = bufferSizes[0] / sizeof(uint32_t);
    uint64_t vertexSize = bufferSizes[1] / sizeof(vec3);
    uint64_t normalSize = bufferSizes[2] / sizeof(vec3);
    uint64_t tCoordSize = bufferSizes[3] / sizeof(vec2);

    if (vertexSize != normalSize || normalSize != tCoordSize) {
        goto ReturnMesh;
    }

    //if (expectedBytes != bytesOfData) {
    //    goto ReturnMesh;
    //}

    uint32_t* indexBuffer = (uint32_t*)malloc(bufferSizes[0]);
    GLfloat* vertexBuffer = (GLfloat*)malloc(bufferSizes[1]);
    GLfloat* normalBuffer = (GLfloat*)malloc(bufferSizes[2]);
    GLfloat* tCoordBuffer = (GLfloat*)malloc(bufferSizes[3]);

    fread(indexBuffer,  sizeof(uint32_t), indexSize, file);
    fread(vertexBuffer, sizeof(vec3), vertexSize, file);
    fread(normalBuffer, sizeof(vec3), normalSize, file);
    fread(tCoordBuffer, sizeof(vec2), tCoordSize, file);
    
    MeshRender mesh = {.materialIndex = 0};
    UploadMesh(&mesh, indexBuffer, vertexBuffer, normalBuffer, tCoordBuffer, indexSize, vertexSize);

    free(indexBuffer);
    free(vertexBuffer);
    free(normalBuffer);
    free(tCoordBuffer);

    staticMesh = Object_StaticMesh_create_empty(parent);
    List_push_back(&staticMesh->meshRenders, mesh);

    ReturnMesh:
    fclose(file);
    return staticMesh;
}


StaticMesh* Object_StaticMesh_create_from_wave_front(const char* path, void* parent) {
    return NULL;
}


StaticMesh* Object_StaticMesh_create(const char* path, void* parent) {
    
    // Find the file extension.
    char* buffer = (char*)path;
    char* ext = NULL;
    while (*buffer) {
        if (*buffer == '.') {
            ext = buffer;
        }
        buffer++;
    }

    if (!ext) {
        return NULL;
    }

    uint64_t fileExtentionPacked = 0;
    String fileExtentionPackedBytes = String_from_chars(fileExtentionPacked);
    String fileExtention = String_from_ptr(ext);
    
    String_clone_substring(fileExtention, fileExtentionPackedBytes, 0, String_length(fileExtention) - 1);
    String_as_lower(fileExtentionPackedBytes);
    
    switch (fileExtentionPacked) {
    case ObjFile:       return Object_StaticMesh_create_from_wave_front(path, parent);
    case GlbFile:       return NULL;
    case GltfFile:      return NULL;
    case BinFile:       return Object_StaticMesh_create_from_raw_data(path, parent);
    case BlendFile:     return NULL;
    case U3dFile:       return NULL;
    case UsdFile:       return NULL;
    case FbxFile:       return NULL;
    default:            return Object_StaticMesh_create_empty(parent);
    };
    return NULL;
}



void Object_StaticMesh_Draw(void* object) {
    StaticMesh* staticMesh = (StaticMesh*)object;
    for (List_iterator(MeshRender, &staticMesh->meshRenders)) {
        DrawRenderable(it, List_at(&staticMesh->materials, it->materialIndex), staticMesh->Transform);
    }
}
